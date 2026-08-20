# 📚 El temario como orden de construcción

← [Índice](README.md)

[`course-alignment.md`](../course-alignment.md) responde **"¿está cubierto?"** — es la matriz de
cobertura y es el contrato de la materia. Este documento responde otra pregunta:
**"¿en qué orden se construye, y qué se rompe si un tema llega tarde?"**

---

## La observación que cambia cómo se lee el temario

El temario **no es una lista de features: es el orden de bring-up de un proyecto profesional**,
y está bien ordenado.

```
framework  ->  input y posesión  ->  diseño de clases y comunicación  ->  niveles
           ->  animación  ->  UI  ->  IA  ->  datos  ->  optimización  ->  C++
```

Ese es exactamente el orden en que se funda un proyecto de Unreal: primero quién es dueño de
qué, después cómo se hablan, después cómo se ven, después cómo piensan, y **recién al final** de
dónde salen los números y cuánto cuesta todo.

Consecuencia práctica: **seguir el temario en orden no es una concesión a la materia, es la
forma correcta de construir.** No hace falta pelear entre "hacer lo que pide la cursada" y
"hacer el juego bien". Con dos excepciones, que son las dos de abajo.

## Excepción 1: C++ se adelanta del final al principio

El temario pone C++ en la **clase 14**, después del 2do parcial. Eso es correcto como orden de
*enseñanza* y equivocado como orden de *construcción*: el módulo de C++ es lo primero que se
crea, porque su costo sube con cada Blueprint que exista antes
([D-13](../gdd/06-decisiones/registro.md),
[`06-limite-cpp-blueprint.md`](06-limite-cpp-blueprint.md)).

**Esto no le quita nada a la cobertura del temario, al contrario.** Los temas de Blueprint se
siguen demostrando, y en su forma profesional: las subclases de contenido, los Animation
Blueprints, los widgets, los Behaviour Trees y los actores colocados en el nivel **son**
Blueprint por diseño. Lo que cambia es que dejan de ser el único lugar posible, y el tema de la
clase 14 pasa de ser un apéndice opcional a estar demostrado en todo el proyecto.

## Excepción 2: la capa de datos llega tarde, y eso cuesta plata

**La capa de datos es clase 12 (22/10), o sea después del 1er parcial (24/9).**

Eso significa que el parcial 1 se entrega con las reglas escritas y los números… en algún lado. Y ahí está la trampa: si los números están tipeados dentro de los grafos, la clase 12 no
es "migrar a Data Tables", es **volver a escribir todo lo que ya funcionaba**, con el juego
entero encima.

La defensa cuesta casi nada y es la regla 3 del [índice](README.md): las reglas **leen**
`DA_MissionConfig` desde el día 1, aunque ese Data Asset tenga los valores a mano y una sola
fila. La clase 12 pasa a ser cambiar de dónde sale el valor.

Con C++ la defensa es todavía más barata: el `USTRUCT` de configuración se declara una vez, en
veinte líneas, y el Data Asset se autorea en el editor. La costura deja de depender de la
disciplina y pasa a ser un tipo que el compilador chequea.

Lo mismo con dos temas más chicos:

- **Gameplay Tags** no son un plugin: son un módulo del engine, disponibles hoy. Nombrar los
  tipos de enemigo y las fuentes de efecto con tags desde el principio evita el `enum` que
  después hay que reemplazar en todos los grafos que lo comparan.
- **Save Game** también es clase 12, pero *qué* hay que guardar lo define el mapa de autoridad.
  Si el estado está desparramado, el save es una arqueología; si vive en `GameState` y
  componentes, es una lista.

## Clase por clase: qué artefacto produce

`P1` = tiene que estar para el 24/9. `P2` = para el 12/11.

### Fundamentos

| Clase | Tema | Artefacto de arquitectura | Dónde cae | Si llega tarde |
|---|---|---|---|---|
| 1 | Estructura de proyecto, Source Control | convenciones, LFS, layout de `Content/` | ✅ hecho | mover assets después genera los conflictos binarios más caros |
| 2 | Actor, Blueprint, Componentes, ciclo de vida | `BP_Space`, `BP_Gate`, los componentes de barras | [mapa de clases](04-mapa-de-clases.md) | si las barras nacen dentro del `Character`, sacarlas después toca todo |
| 2 | Depuración de Blueprints | visualización del grafo y del BFS | `UGraphSubsystem` + su test | un BFS sin debug visual se depura a ciegas. Y con test, se depura una sola vez |
| 3 | **Gameplay Framework** | **las 5 clases del esqueleto, con su subclase Blueprint cada una** | [paso 2](01-por-donde-se-empieza.md) | **es el tema que no se puede hacer tarde.** Todo lo demás se cuelga de acá |
| 3 | Static Mesh, Materiales | salas, figuras, caras de dado, feedback de estado | `Characters/`, `MaterialLibrary/` | nada grave |
| 3 | **Físicas y Colisiones** | los dados, y **el resolvedor que aísla la física del estado** | [la costura del dado](04-mapa-de-clases.md) | si el actor del dado declara el resultado, el save y los rerolls se complican después |

### Mecánicas y lógica

| Clase | Tema | Artefacto de arquitectura | Dónde cae | Si llega tarde |
|---|---|---|---|---|
| 4 | Inputs | `IMC_`/`IA_` + Enhanced Input (ya viene habilitado) | `Input/` | — |
| 4 | **Posesión** | alternar entre los 4 `Character` desde un `PlayerController` | [framework](04-mapa-de-clases.md) | encaje directo con el diseño: **el jugador controla los 4** |
| 4 | Raycast | `ISelectable` | interfaces | si la selección se resuelve casteando, la UI empieza a arrastrar arte |
| 5 | **Diseño de clases** | la jerarquía y **qué es componente y qué es herencia** | [mapa de clases](04-mapa-de-clases.md) | la clase base que sabe todo es irreversible en la práctica |
| 5 | **Comunicación entre Blueprints** | las 4 interfaces y los delegates | [comunicación](03-comunicacion-y-referencias.md) | **el tema con más deuda técnica potencial de todo el temario** |
| 5 | **Levels y SubLevels** | una sala por sublevel | `Maps/` | es la **condición para trabajar en paralelo**, no un tema más |
| 12 | Data Assets y Data Tables | `DA_MissionConfig` y las `DT_`, sobre `USTRUCT` de C++ | [datos](04-mapa-de-clases.md) | ver la excepción 2 |
| 12 | Gameplay Tags | vocabulario de tipos y efectos | transversal | reemplazar enums por tags toca todos los comparadores |
| 12 | Save Game | serialización de `GameState` + componentes | — | depende del mapa de autoridad, no de la clase |

### Animación, UI y efectos

| Clase | Tema | Artefacto de arquitectura | Dónde cae | Si llega tarde |
|---|---|---|---|---|
| 6 | Animation Blueprint, Blendspace, State Machine | `ABP_Character`, `ABP_Enemy` | `Characters/` | **obliga a que las figuras sean `Character` y no `Pawn`**: decidilo en clase 3 |
| 6 | Notifies | frame de impacto, pasos, tirada | `ABP_` | el daño aplicado sin notify se siente desincronizado |
| 7 | **UMG** | toda la jerarquía `WBP_` | [la UI](04-mapa-de-clases.md) | **el ítem más subestimado de la fase 1** |
| 7 | Sonidos | dados, umbral de `Ratchet`, avance del reloj | — | — |
| 11 | Lights | día arriba / oscuridad abajo; cambia con el reloj | `Maps/` | — |
| 11 | **Sequencer** | los 4 reveals de stage del `Adversary` | `DA_AdversaryStage` | encaje directo |
| 11 | **Chaos Destruction** | paredes que **agregan aristas al grafo** en runtime | `UGraphSubsystem` | si el grafo se construye inmutable, la destrucción no puede ser mecánica |
| 11 | **Landscape** | la sección de superficie del mapa | `Maps/` | — |

### IA, optimización y cierre

| Clase | Tema | Artefacto de arquitectura | Dónde cae | Si llega tarde |
|---|---|---|---|---|
| 9 | Navegación y Pathfinding | NavMesh en las salas | `Maps/` | — |
| 9 | **Percepción** | el skill `Stealth`: "1 enemigo no te sigue" | `AEnemyAIController` | encaje directo |
| 10 | Behaviour Tree | la decisión de activación del enemigo | `AEnemyAIController` + BT asset | **reemplaza** el scripting de fase 1; por eso ese scripting va en el `GameMode` y no adentro del enemigo |
| 10 | **EQS** | "el enemigo más cercano por camino más corto" y elección de destino | consulta al servicio de grafo | el EQS tiene que poder preguntarle al grafo: la distancia del juego **no es** la distancia del NavMesh |
| 13 | Profiler | medición con el mapa completo | — | medir en clase 13 lo que se armó mal en clase 5 solo dice cuánto hay que rehacer |
| 13 | Assets Streaming | sublevels de sala con referencias **blandas** | `Maps/`, `Data/` | con todo hard-referenciado no hay nada que streamear |
| 13 | Packaging | build jugable, y arreglar el `Game Default Map` | — | — |
| 14 | C++ | **adelantado al paso 0.5**: el módulo, los servicios, los tipos de datos y los tests | [límite C++/BP](06-limite-cpp-blueprint.md) | no llega tarde: es lo primero |

### La distancia del juego no es la distancia del NavMesh

Vale aislarlo porque es el error más fácil de cometer en las clases 9 y 10, y el más caro de
descubrir: el juego mide en **saltos de `Space` sobre el grafo, con aristas bloqueables**
([`mapa-y-espacios.md`](../gdd/01-fundamentos/mapa-y-espacios.md)). El NavMesh mide en
centímetros de camino caminable.

Los dos coinciden casi siempre y **difieren exactamente donde importa**: una pared cerrada, un
pasaje bloqueado, una escalera. Si el EQS pregunta "quién está más cerca" al NavMesh, la regla
del juego queda mal implementada de una forma que se ve poco y se siente rarísimo. El NavMesh
mueve el cuerpo; el grafo decide a dónde.

## La vista inversa: qué clases necesita cada sistema del GDD

Lo mismo leído desde el diseño, que es como se planifica el trabajo real.

| Sistema del GDD | Clases que hacen falta | Jugable en |
|---|---|---|
| [Mapa y espacios](../gdd/01-fundamentos/mapa-y-espacios.md) | 2 (actores), 5 (sublevels), 11 (Chaos) | P1, se completa en P2 |
| [Barras y recursos](../gdd/02-personaje/barras-y-recursos.md) | 2 (componentes), 7 (UMG) | P1 |
| [Trinquete](../gdd/02-personaje/trinquete.md) | 2, 7, 12 (las 8 cartas) | P1 parcial, cartas en P2 |
| [Skills](../gdd/02-personaje/skills.md) | 5 (efectos), 12 (`DT_Skills`) | P1 parcial |
| [Dados](../gdd/03-resolucion/dados.md) | **3 (físicas)**, 4 (raycast), 7 | P1 |
| [Turno](../gdd/03-resolucion/turno.md) | 3 (framework), 4 (posesión) | P1 |
| [Vocabulario de efectos](../gdd/03-resolucion/vocabulario-de-efectos.md) | 5 (interfaces), 12 (datos) | costura en P1, datos en P2 |
| [Enemigos](../gdd/04-oposicion/enemigos.md) | 2, 6 (anim), 9, 10 | scripteados en P1, IA en P2 |
| [Reloj y avance](../gdd/04-oposicion/reloj-y-avance.md) | 3, 7, 12 (las 16 cartas) | P1 |
| [Objetivo y adversario](../gdd/04-oposicion/objetivo-y-adversario.md) | 11 (Sequencer), 12 | P2 |
| [Victoria y derrota](../gdd/05-partida/victoria-y-derrota.md) | 3 (árbitro) | P1 |

**Lo que salta a la vista: la fase 1 se apoya casi entera en las clases 2, 3, 4, 5 y 7.** Las
cinco primeras clases y UMG. La IA y los datos —las partes que suenan más difíciles— son fase 2
y reemplazan cosas que ya funcionan, en lugar de habilitarlas.

## Huecos en nuestro propio registro

Cosas que faltan de **nuestros** papeles, no del temario:

1. **El temario no está en el repo.** Vive en el chat y en la matriz derivada. Si la matriz es
   el contrato, el original tiene que ser un archivo — es lo único de esta lista que puede
   invalidar todo lo demás.
2. **La clase 8 no figura en la matriz.** Puede ser la entrega del parcial, puede ser un tema
   sin registrar. Hay que confirmarlo.
3. **Las clases 15 a 19 no están desglosadas.** Del calendario se conocen las fechas (entregas
   del 2do parcial 12/11 y 19/11, recuperatorio 26/11, final 17/12), no los temas.

## Dependencias

- Consume: [`../course-alignment.md`](../course-alignment.md),
  [`04-mapa-de-clases.md`](04-mapa-de-clases.md)
- Alimenta: la planificación de las dos entregas
