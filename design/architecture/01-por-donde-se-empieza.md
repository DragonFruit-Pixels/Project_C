# 🧭 Por dónde se empieza

← [Índice](README.md)

Un proyecto profesional de Unreal no empieza por una mecánica. Empieza por **decidir quién es
dueño de qué**, y por dejar el esqueleto del framework armado y vacío antes de que exista la
primera regla de juego.

El orden de abajo no es una preferencia: está ordenado por **costo de cambiarlo después**. El
paso 0 es gratis hoy y carísimo en octubre; el paso 6 se puede rehacer todas las semanas.

---

## Paso 0 — Convenciones y estructura de carpetas · **ya hecho**

Prefijos de asset, charset, layout de `Content/Project_C/`. Está en
[`docs/unreal/project-context.md`](../../docs/unreal/project-context.md).

**Por qué va primero.** Renombrar o mover un asset en Unreal deja un *redirector*, y los
redirectors sin arreglar son la causa habitual de referencias que se rompen al empaquetar.
Peor en este proyecto: los `.uasset` son binarios y viven en LFS, así que mover assets es la
operación que **más conflictos de merge genera** y la que menos se puede resolver a mano.

Una sola cosa falta y es de una línea: **`Game Default Map` apunta a un mapa del *engine***
(`/Engine/Maps/Templates/OpenWorld`). Es el default de la plantilla y la causa clásica de un
build empaquetado en negro. Se arregla cuando exista el primer mapa del proyecto — antes de
Packaging, que es clase 13.

## Paso 0.5 — El módulo de C++ · **mientras `Content/` esté vacío**

Es el paso más nuevo y el más urgente, porque su costo **sube con cada Blueprint que exista**.
Reparentar un Blueprint ya creado a una base de C++ puede perder variables y referencias; hoy
no hay ninguno que reparentar.

Qué implica, en detalle, en
[`06-limite-cpp-blueprint.md`](06-limite-cpp-blueprint.md). El resumen operativo:

1. **Falta el toolchain.** Visual Studio 2022 está instalado pero **sin el compilador**: hay que
   agregar el workload *Game development with C++* desde el Visual Studio Installer. Sin eso el
   proyecto no compila, y lo necesita cada persona que abra el proyecto.
2. Se crea el módulo `ProjectC` con sus `Target.cs` y su `Build.cs`.
3. El `.gitignore` ya está bien: ignora `Binaries/`, `Intermediate/` y `Build/`, versiona
   `Source/`. Verificado, no hay que tocarlo.

## Paso 1 — El mapa de autoridad

**Este es el primer acto de arquitectura de verdad**, y se hace en una tabla antes de abrir el
editor. Por cada pieza de estado del juego: quién la escribe, quién la lee, cuánto vive.

Se hace primero porque es lo único de esta lista que **no se puede refactorizar**: mover una
regla de un Blueprint a otro es trabajo mecánico, pero descubrir en la semana 6 que tres
Blueprints escriben el mismo contador no se arregla moviendo nodos, se arregla rediseñando.

La tabla completa está en [`04-mapa-de-clases.md`](04-mapa-de-clases.md). El criterio para
llenarla:

| Pregunta | Si la respuesta es… | El dueño es |
|---|---|---|
| ¿Sobrevive al cambio de nivel? | sí | `GameInstance` |
| ¿Es estado de la partida que todos leen? | sí | `GameState` |
| ¿Es una regla o un árbitro? | sí | `GameMode` |
| ¿Es la voluntad del jugador (input, cámara, UI)? | sí | `PlayerController` |
| ¿Es una propiedad de una cosa que está en el mundo? | sí | un **Component** de ese Actor |
| ¿Es un número de balance? | sí | un **Data Asset** de configuración |

## Paso 2 — El esqueleto del Gameplay Framework, vacío pero cableado

Cinco clases de C++ que no hacen nada todavía, cada una con su subclase Blueprint, más el
`Project Settings → Maps & Modes` que enchufa **las subclases**. Es clase 3 del temario y es el
paso que más se saltea, porque no se ve nada al terminarlo.

> **Se enchufan las Blueprint, no las de C++.** Es lo que deja cambiar un default sin
> recompilar, y es el patrón central de [D-13](../gdd/06-decisiones/registro.md): base en C++,
> hijo en Blueprint.

Unreal ya tiene una arquitectura, y **cada clase tiene un rol asignado por el engine**. Pelear
contra eso es la fuente número uno de proyectos que no escalan:

| Clase | Cuánto vive | Para qué es | Qué le toca en este juego | Qué **no** va acá |
|---|---|---|---|---|
| `GameInstance` | todo el proceso, sobrevive cambios de nivel | servicios de vida larga | RNG con semilla, save en vuelo, estado entre misiones | reglas de la misión |
| `GameMode` | por nivel | **el árbitro**: reglas, spawn, victoria y derrota | director de turno, resolución de tiradas, chequeo de fin de partida | estado que otros necesiten leer |
| `GameState` | por nivel | **estado compartido y legible por todos** | `Doom Track`, ronda, personaje activo, mazos, figuras vivas | decisiones; solo guarda |
| `PlayerController` | por jugador | la voluntad del jugador: input, cámara, dueño de la UI | selección de personaje, posesión, widgets del HUD | reglas del juego |
| `Pawn` / `Character` | mientras exista la figura | **el cuerpo**: mesh, colisión, locomoción | el `Character` y sus barras, como componentes | reglas globales |

Dos detalles que ahorran discusiones:

- **`GameState` existe igual en single player.** No es una clase de multiplayer: es el lugar
  correcto para el estado compartido, y usarla bien es lo que deja la puerta abierta si algún
  día hay red. Gratis hoy.
- **`Character` (no `Pawn`) para las figuras del jugador**, porque trae
  `CharacterMovementComponent` y cápsula, que es lo que alimenta el Blendspace de locomoción
  de la clase 6. Un `Pawn` pelado obliga a escribir el movimiento a mano.

**Trampa del mismo paso:** *nada* de lógica en el **Level Blueprint**. No se puede reusar, no
se puede heredar, y en un trabajo grupal es un archivo binario que todos necesitan tocar el
mismo día. Lo que parece pertenecerle al nivel casi siempre pertenece al `GameMode` o a un
Actor colocado en el nivel.

## Paso 3 — El director de turno como máquina de estados explícita

El juego es por turnos, así que **el corazón del proyecto no es el movimiento: es una máquina
de estados de fases**. Vive en el `GameMode` y las fases son datos, no comentarios:

```
StartOfRound -> CharacterTurn(i) -> Actions(x3) -> PressureCard -> Reckoning
             -> EndOfTurnEffects -> Hazard -> ClockCheck -> ManifestationCheck
             -> AdversaryEndOfTurn -> [siguiente personaje] -> EndOfRound -> ...
```

Los últimos cinco son la fase 4 del GDD, abierta en sus pasos. Están explícitos porque dejarlos
implícitos ya costó un bug: `ClockCheck` había quedado al nivel de ronda, y el GDD lo pone en
**cada turno**.

Dos razones para hacerlo explícito desde el principio, y las dos son del temario:

- La secuencia de turno del GDD ([`turno.md`](../gdd/03-resolucion/turno.md)) tiene pasos
  obligatorios que se saltean fácil si el orden está implícito en el orden de los nodos.
- **Un juego por turnos casi no necesita `Event Tick`.** Si las fases son explícitas, cada cosa
  pasa porque una fase la disparó. Si no lo son, aparecen los `Tick` que preguntan "¿ya
  terminó?", y eso es exactamente lo que después hay que desarmar en clase 13 con el Profiler.

## Paso 4 — Las capacidades son componentes, no herencia

Cuando algo es "una cosa que un Actor tiene", va en un **Actor Component**: `Wounds`,
`Reserve`, `Ratchet`, `Skills`, inventario. No en la clase base.

Es la respuesta de Unreal a composición sobre herencia, y acá tiene un beneficio concreto:
`Wounds` lo tienen los personajes **y** los enemigos **y** los `Ally`, que no comparten clase
padre. Con herencia habría que subir `Wounds` a una base común, y esa base termina siendo la
clase que sabe todo. Es clase 5, diseño de clases.

Beneficio lateral para el trabajo grupal: **un componente es un archivo chico**. Dos personas
tocando `Ratchet` y `Skills` el mismo día no chocan; dos personas tocando la misma clase madre
sí. Y si esa clase madre es C++, al menos el conflicto se mergea; si es un `.uasset`, alguien
pierde el trabajo.

## Paso 5 — Las costuras de datos, antes de tener datos

La capa de datos es clase 12, después del 1er parcial. Pero la **costura** se pone ahora, y es
casi gratis: donde va un número, va una variable que sale de un Data Asset de configuración
con el valor puesto a mano.

Ver la regla 3 en el [índice](README.md), y el detalle en
[`05-temario-como-orden-de-construccion.md`](05-temario-como-orden-de-construccion.md).

Dos cosas se pueden empezar hoy sin pagar nada:

- Los **Gameplay Tags** no son un plugin, son un módulo del engine y están disponibles ya
  —verificado en `Engine/Source/Runtime/GameplayTags`. Nombrar los tipos de enemigo y las
  fuentes de efecto con tags desde el primer día no adelanta la clase 12, pero evita el enum que
  después hay que reemplazar. Con C++ además se declaran nativos, así que autocompletan y no se
  escriben mal.
- El `USTRUCT` de configuración y la clase de Data Asset. Declararlos cuesta veinte líneas y
  hace que la clase 12 sea llenar filas en vez de migrar.

## Paso 6 — Recién acá, la primera rebanada jugable

Con los pasos 0 a 5 hechos, la primera feature es chica y no discute con nada: **un `Character`
en un `Space`, que se mueve a un `Space` vecino consumiendo una acción, con el `GameMode`
contando las 3 acciones.** Eso ya ejercita framework, componentes, interfaces, el grafo y la
máquina de fases; y todo lo que venga después se cuelga del mismo esqueleto.

Lo que **no** conviene hacer primero, aunque sea lo más tentador: los dados. Es el sistema más
vistoso y el que más decisiones arrastra — ver la costura física/lógica en
[`04-mapa-de-clases.md`](04-mapa-de-clases.md).

Y lo que conviene hacer **junto** con esa primera rebanada: sus tests. El BFS del grafo y el
conteo de acciones son las dos primeras cosas testeables del proyecto, y escribir el test
mientras el sistema es chico es lo que hace que la costumbre exista después. Ver
[`06-limite-cpp-blueprint.md`](06-limite-cpp-blueprint.md).

---

## Lo que hay que revisar antes de que `Content/` deje de estar vacío

Dos cosas heredadas de la plantilla que hoy son gratis de cambiar y en octubre no:

- **Substrate está habilitado** (`r.Substrate=True`). Es un compromiso profundo de rendering:
  volver atrás implica reautorar materiales. Llegó con la plantilla, no de una decisión.
- **El baseline es ray tracing + Lumen + Virtual Shadow Maps en "Maximum"**, y el juego es por
  turnos con cámara controlada. Es bastante más de lo que hace falta, y se paga en el Profiler
  de la clase 13.

Las dos están registradas como preguntas abiertas en
[`project-context.md`](../../docs/unreal/project-context.md).

## Dependencias

- Consume: [`../gdd/03-resolucion/turno.md`](../gdd/03-resolucion/turno.md),
  [`../gdd/01-fundamentos/mapa-y-espacios.md`](../gdd/01-fundamentos/mapa-y-espacios.md)
- Alimenta: [`04-mapa-de-clases.md`](04-mapa-de-clases.md),
  [`05-temario-como-orden-de-construccion.md`](05-temario-como-orden-de-construccion.md)
