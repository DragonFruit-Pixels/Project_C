# 🎓 Alineación con el temario de la materia

Cruce entre el temario de la cursada (Unreal, 2026) y el diseño de Project_C, para poder
verificar cobertura contra una lista concreta en vez de a ojo.

**Objetivo declarado por el usuario: cumplir el temario en su totalidad.** Ningún tema
puede quedar sin un lugar propio en el juego.

> Este documento responde **"¿está cubierto?"**. El temario **original** está en
> [`temario.md`](temario.md) y le gana a este si se contradicen. El **orden de construcción**
> —qué artefacto produce cada clase, en qué Blueprint cae y qué se rompe si el tema llega tarde—
> está en [`architecture/05-temario-como-orden-de-construccion.md`](architecture/05-temario-como-orden-de-construccion.md),
> y el esqueleto técnico en [`architecture/README.md`](architecture/README.md).

## Fechas

| Hito | Fecha | Cubre hasta |
|---|---|---|
| **Última verificación de esta matriz** | **27/8/2026** | **clases 1-3 dictadas** |
| Clase 4 — Inputs, Posesión, Raycast | 27/8/2026 (hoy) | — |
| **Entrega 1er Parcial** | **24/9/2026** | **clases 1-7** |
| **Entrega 2do Parcial** | **12/11 y 19/11/2026** | **clases 1-14** |
| Recuperatorio | 26/11/2026 | — |
| Final | 17/12/2026 | — |

**28 días hasta el 1er parcial. 77 hasta el 2do.**

Los temas de las clases 1-4 salen del material real (`Clase 1 - Intro a Unreal.pdf`,
`Clase 2 - Blueprints.pdf`, `Clase 3 - Gameplay Framework.pdf` y el temario clase por clase),
no de un resumen. La columna **Estado** se verificó contra el repo y contra los Blueprints
leídos del editor por MCP, no a ojo.

## Condiciones de la cursada

- **No hay consigna escrita.** Por lo tanto **esta matriz de cobertura es el contrato**:
  es lo que se chequea tema por tema antes de cada entrega.
- **La entrega es grupal.** Ver la sección de trabajo en paralelo más abajo: en Unreal esto
  es un problema técnico antes que organizativo.
- **Se evalúa el juego Y la demostración de los temas.** Consecuencia directa: ningún tema
  puede quedar afuera aunque el juego no lo necesite. Chaos Destruction y Landscape dejan
  de ser opcionales.
- **El alcance no es una restricción** (decisión del usuario). El tamaño del equipo, sí.

## Restricciones duras que impone el temario

1. ~~**Blueprint-only**~~ → **arquitectura híbrida C++ + Blueprint**
   ([D-13](gdd/06-decisiones/registro.md), 2026-08-20). El temario pone C++ en la clase 14, pero
   eso ordena *cuándo se enseña*, no qué se puede usar. Los temas de Blueprint se siguen
   demostrando —subclases de contenido, Animation Blueprints, widgets, Behaviour Trees y actores
   colocados son Blueprint por diseño— y además queda demostrado el tema de la clase 14. Ver
   [`architecture/06-limite-cpp-blueprint.md`](architecture/06-limite-cpp-blueprint.md).
2. **La capa de datos es de 2do parcial.** Data Assets, Data Tables, Gameplay Tags y Save
   Game son clase 12 (22/10), o sea después del 1er parcial. El parcial 1 va con las reglas en
   código y los números en un `DA_MissionConfig` cargado a mano — la costura desde el día 1 es lo
   que convierte la clase 12 en una migración en vez de una reescritura.
3. **Los personajes se mueven en 3D con locomoción animada.** Blendspace y State Machine
   (clase 6) lo exigen. Descarta la representación de ficha que teletransporta de casilla
   a casilla.
4. **Hay que llegar a un build empaquetado.** Packaging es clase 13.
5. **La forma converge a por turnos con presentación 3D.** Animación, posesión, NavMesh,
   Behaviour Trees, percepción y EQS piden agentes moviéndose en un espacio 3D; nada del
   temario pide combate en tiempo real.

## Matriz de cobertura

`P1` = tiene que estar para el 1er parcial. `P2` = para el 2do.

### Fundamentos del Gameplay

Estado: ✅ hecho · ◐ parcial · ❌ sin empezar.

| Clase | Tema | Estado | Dónde vive en el juego / qué falta |
|---|---|---|---|
| 1 | Estructura de proyecto | ✅ | `Source/` `Content/` `Config/` + `design/` |
| 1 | **Source Control** | ✅ | git + **LFS** para `.uasset`/`.umap`, `.gitignore`, rama publicada |
| 1 | Templates, Editor | ✅ | proyecto y `L_Mission_01` desde template |
| 2 | Qué es un Actor | ✅ en C++ | `ASpace`, `AProjectCCharacter` |
| 2 | Qué es un Blueprint | ◐ | los 5 Blueprints existen y heredan bien, pero **están vacíos** |
| 2 | Componentes | ✅ en C++ | `URatchetComponent` (ActorComponent), `UBoxComponent` (SceneComponent) |
| 2 | **Blueprint Workflow** | ❌ | variables, funciones, arrays, loops, casting, macros: **cero nodos** |
| 2 | **Depuración de Blueprints** | ❌ | breakpoints y Blueprint Debugger sin usar. Iría sobre el cálculo de distancias del grafo |
| 2 | Ciclo de vida del Actor | ◐ | `BeginPlay` en `ASpace`, `StartPlay` en GameMode. Falta **Spawn/Destroy**: enemigos en las 3 puertas |
| 3 | **Gameplay Framework** | ✅✅ | las 6 clases del PDF: `MissionGameMode`, `MissionGameState`, `MissionPlayerController`, `MissionPlayerState`, `ProjectCGameInstance`, `AProjectCCharacter` |
| 3 | Subsystems | ✅ extra | `UGraphSubsystem` (`UWorldSubsystem`). El PDF los nombra en la slide de GameInstance |
| 3 | **Static Mesh** | ❌ | nada. `ASpace` tiene Box de colisión pero ningún mesh; el nivel está vacío |
| 3 | Colisiones — canales, presets, modos | ✅ | `Space`/`Figure` (object) + `Selectable` (trace), perfiles `QueryOnly` |
| 3 | **Físicas — simulación real** | ❌ | **los dados.** Sin `SimulatePhysics`, sin eventos `Hit`/`Overlap`, sin Physics Material ni Constraint |
| 3 | Materiales | ❌ | caras de los dados, estado de las salas, feedback de trinquete |

### Mecánicas y Lógica del Juego

| Clase | Tema | Estado | Dónde vive en el juego / qué falta |
|---|---|---|---|
| 4 | **Inputs** | ✅ | `IMC_Mission` + 5 `IA_` en `Content/Project_C/Input/`. El `PlayerController` los recibe como `EditDefaultsOnly` y agrega el contexto por `UEnhancedInputLocalPlayerSubsystem` |
| 4 | **Posesión** | ✅ | `BP_CameraPawn` (SpringArm + Camera) como `DefaultPawnClass`, **enteramente en Blueprint**: componentes, variables, `BeginPlay`, `Tick` y los tres eventos de Enhanced Input. No hay clase de C++ debajo ([D-30](gdd/06-decisiones/registro.md)). **Corregido respecto de la versión anterior de este doc:** el jugador **no** posee personajes ([D-18](gdd/06-decisiones/registro.md), [`architecture/04-mapa-de-clases.md`](architecture/04-mapa-de-clases.md)). Posee una cámara, y cada figura la mueve su propio `AAIController`. El tema queda igual de demostrado, y de hecho por partida doble |
| 4 | Raycast | ✅ | `ISelectable` + `GetHitResultUnderCursorByChannel` sobre el canal `Selectable`. Lo implementan `ASpace` y `AProjectCCharacter`; el highlight lo dibuja `BP_Space` |
| 5 | Diseño de clases | ◐ | jerarquía `Figure` -> `Character` / `Enemy` -> `Adversary`. Hoy existen `AProjectCCharacter` y `ASpace`; falta el resto |
| 5 | Comunicación entre Blueprints | ◐ | `OnPhaseChanged` ya es un `BlueprintAssignable` en el GameMode. Faltan interfaces y el resto de los dispatchers |
| 5 | **Levels y SubLevels** | ❌ | **Corregido:** "cada sala es un sublevel" chocaba con [D-21](gdd/06-decisiones/registro.md) (un nivel por misión, grafo sellado al arrancar). Sirve igual **si los sublevels son Always Loaded y no streaming**: así terminan todos los `BeginPlay` antes de que `StartPlay` selle. Con streaming en runtime, un `ASpace` se registra tarde y `UGraphSubsystem` lo loguea como error |
| 12 | **Data Assets y Data Tables** | Cartas de presión, cartas de botín, skills, tipos de enemigo, stages del jefe | P2 |
| 12 | **Gameplay Tags** | Tipos de enemigo, fuentes de daño, efectos de carta, condiciones. Es el mecanismo que hace el core agnóstico al tema | P2 |
| 12 | Save Game | Guardar partida en curso y progresión entre partidas | P2 |

### Animación y Efectos Visuales

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 6 | Animation Blueprint | Personajes y enemigos | P1 |
| 6 | Blendspace | Locomoción direccional al moverse entre espacios | P1 |
| 6 | State Machine | Idle / caminar / atacar / herido / muerto | P1 |
| 6 | Notifies | Frame de impacto del ataque, pasos, tirada de dados | P1 |
| 7 | **UMG** | HUD completo: 3 barras por personaje, track del jefe, pila de descarte visible, cartas, dados, skills | P1 |
| 7 | Sonidos | Dados, impactos, umbral de trinquete, avance del jefe | P1 |
| 11 | Lights | Iluminación del mapa; cambio de estado al avanzar el jefe | P2 |
| 11 | **Sequencer** | **Los 4 reveals de stage del jefe.** Encaje directo | P2 |
| 11 | **Chaos Destruction** | **Paredes destructibles que modifican el grafo de espacios.** Ver abajo | P2 |
| 11 | **Landscape** | **La sección de superficie del mapa**, antes de descender. Ver abajo | P2 |

### Inteligencia Artificial

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 9 | Navegación y Pathfinding | Movimiento de enemigos entre espacios sobre NavMesh | P2 |
| 9 | **Percepción** | **El skill Stealth**: "1 enemigo no te sigue". Encaje directo | P2 |
| 10 | Behaviour Tree | Decisión del enemigo en su activación: seguir, atacar, reposicionar | P2 |
| 10 | **EQS** | **"El enemigo más cercano por camino más corto"** y elección de espacio destino | P2 |

### Optimización y Publicación

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 13 | Profiler | Medición sobre el mapa completo con enemigos | P2 |
| 13 | Assets Streaming | Carga y descarga de sublevels de sala | P2 |
| 13 | Optimización | Presupuestos de performance | P2 |
| 13 | Packaging | Build jugable entregable | P2 |
| 14 | C++ | **Adelantado**: el módulo, los servicios como Subsystems, los tipos de datos y los tests de reglas | P1 |

## Los huecos, al 27/8 después de la clase 4

Ordenados por urgencia, no por tamaño. La clase 4 cerró uno entero y mordió otro.

1. **Los 8 ejercicios de Blueprint del profe siguen sin hacerse** (clase 2). Son Blueprint puro
   —variables, arrays, `ForEachLoop`, structs, Actor Components, `Gate` + `Delay`— y todo eso
   está resuelto en C++, que es más avanzado pero **no es donde lo van a buscar**. Es el hueco
   más urgente porque el 1er parcial cubre clases 1-7.

   Lo que **sí** se cerró: los Blueprints ya no son cascarones. `BP_Space` tiene un grafo real
   (override de `SetHighlight`, función `ApplyHighlight` con un switch de 4 ramas que maneja
   parámetros de material), `BP_PlayerController_Mission` tiene sus 6 referencias de Input, y
   `BP_CameraPawn` dejó de ser un cascarón para pasar a ser **la cámara entera** —componentes,
   15 variables, `BeginPlay`, `Tick` y los tres eventos de Enhanced Input— con el C++ que hacía
   ese trabajo ya borrado ([D-30](gdd/06-decisiones/registro.md)). `BP_Character` trae mallas y
   valores. `BP_GameState_Mission` y
   `BP_GameInstance` siguen vacíos, y está bien: todavía no tienen dato que sostener.

   No contradice [D-13](gdd/06-decisiones/registro.md): la arquitectura híbrida sigue en pie. Lo
   que falta es llevar a Blueprint cosas que ya funcionan, no inventar lógica nueva.

2. ~~**No hay nada visible**~~ — **cerrado el 27/8.** `L_Mission_01` tiene 9 `BP_Space` con malla
   y material (`M_Space`, con parámetros `Colour` y `Glow`), una figura colocada, y el grafo
   sella con 9 espacios y 11 aristas. El mapa **no es una retícula**: los grados van de 2 a 4, y
   hay una "escalera" S1↔S8 que es lejana en geometría y adyacente en el grafo — que es
   justamente lo que `Claustrophobia` necesita poder leer.

3. **Faltan las físicas de verdad** (clase 3). Son **los dados**, que son mecánica central y no
   decorado. Cuanto más tarde, más caro: [D-17](gdd/06-decisiones/registro.md) ya fijó que el dado
   físico *responde* pero no *decide*, así que la costura está pensada, pero no construida.
   **Pasa a ser el hueco técnico más grande.**

## Los dos temas que no tenían lugar natural

Como se evalúa la demostración de los temas, estos dos necesitan un destino diseñado y no
un agregado al final. Los dos tienen una versión que es mecánica en vez de decorativa.

### Chaos Destruction -> paredes destructibles que modifican el grafo

El mapa es un grafo de espacios, y el manual ya trae el concepto de pasajes bloqueados que
afectan el cálculo del camino más corto (página 20). Si una pared se puede romper, aparece
una arista nueva y cambian de golpe todas las distancias, las rutas de huida y quién es "el
enemigo más cercano".

Eso hace que la destrucción sea **una mecánica**, no un efecto. Quién puede romper qué
queda por decidir — candidatos: el jefe al cambiar de stage, un monstruo pesado, o una de
las 2 acciones propias de la misión.

### Landscape -> la sección de superficie del mapa

El mapa tiene una parte exterior donde arranca la partida y una subterránea donde está el
objetivo. Landscape arriba, meshes abajo. No es forzado: los episodios de DMD incluyen
mapas al aire libre, y **las dos temáticas candidatas son sobre descender**.

Beneficio lateral: le da trabajo real a Lights (clase 11) — luz de día arriba, oscuridad
abajo, y la iluminación del mapa entero cambiando a medida que el jefe avanza en su track.

## Trabajo en paralelo: los assets de Unreal son binarios

El problema técnico del trabajo grupal en Unreal, y no tiene nada que ver con el diseño.
`.uasset` y `.umap` son binarios: si dos personas tocan el mismo Blueprint en ramas
distintas, git no puede mergear — **se pierde el trabajo de una de las dos**. No hay
resolución de conflicto posible, hay que elegir un archivo y descartar el otro.

Tres defensas, en orden de importancia:

1. **Una sala por sublevel.** Si el mapa es un solo `.umap`, una sola persona puede tocar
   el nivel a la vez. Con sublevels, cada uno trabaja en el suyo. Los SubLevels (clase 5)
   dejan de ser un tema del temario y pasan a ser la condición para trabajar en paralelo.
2. **Muchos Blueprints chicos en vez de pocos grandes.** No es solo buena práctica: baja la
   probabilidad de que dos personas necesiten el mismo archivo el mismo día.
3. **Git LFS locking.** Marcar `*.uasset` y `*.umap` como `lockable` en `.gitattributes`
   deja los archivos en solo-lectura hasta que alguien los bloquea explícitamente. Es
   fricción, pero avisa antes en vez de después. Cae en Source Control, que es clase 1.

Además hay que definir **ownership por área** — quién es dueño de qué Blueprints — para
que la defensa 2 funcione de verdad.

## Plan de dos fases

### Fase 1 — hasta el 24/9 (28 días)

El loop completo jugable, con reglas en Blueprint y enemigos scripteados. **Los enemigos de
DMD no necesitan IA para las reglas base**: te siguen al salir de un espacio y te atacan si
comparten espacio. Eso es scripting legítimo, no una simplificación tramposa, y deja a las
clases 9-10 el trabajo real de reemplazarlo.

Alcance sugerido, más chico que la misión completa:

- Grafo de espacios con 4-6 salas como sublevels **Always Loaded** (ver [D-21](gdd/06-decisiones/registro.md))
- 2 personajes movidos por su `AAIController` y una cámara poseída por el jugador (no los 4 todavía)
- 1 tipo de enemigo, spawn en 1 puerta
- Dados físicos con las 4 caras y la tirada completa
- Las 3 barras y los umbrales de trinquete con subida de skill
- La secuencia de turno completa: 3 acciones, carta de presión, `Reckoning`, fin de turno en sus 5 pasos
- HUD en UMG y sonidos
- Condición de victoria y de derrota

### Fase 2 — hasta el 12/11 (49 días más)

- Reemplazar el scripting de enemigos por NavMesh + Behaviour Tree + EQS + Percepción
- Extraer las reglas a Data Tables y Data Assets, con Gameplay Tags como vocabulario
- Los 4 personajes, el jefe con sus 4 stages y Sequencer en los reveals
- Save Game
- Landscape, Lights, Chaos Destruction
- Profiler, streaming de sublevels y packaging

## Riesgos

**El más grande: 28 días, y la fase 1 pide UMG pesado.** El HUD de este juego no es un HUD:
son 3 barras por personaje, el track del jefe, la pila de descarte visible, las cartas, los
dados y los skills. Es el ítem que más se subestima y cae justo antes del 1er parcial.

**Un merge perdido cuesta más que una semana de diseño.** Si el equipo empieza a trabajar
antes de tener los sublevels y el ownership definidos, la primera colisión sobre un
Blueprint grande borra trabajo real. Es el riesgo más barato de evitar y el más caro de
descubrir tarde.

**La pila de descarte tiene que ser visible.** En la mesa es información pública gratis; en
pantalla, si no se construye, se pierde el mecanismo de presión entero. Es UMG de fase 1.

**El alcance del juego excede el de la materia.** 1 misión + 1 boss + 4 personajes + 24
efectos de skill + 31 cartas es el juego; la materia termina el 17/12. La materia es un
hito adentro del juego, no el juego.

## Pendiente de confirmar

Huecos de **este registro**, no del temario:

- ~~**El temario no está en el repo.**~~ Resuelto el 27/8: transcrito en
  [`temario.md`](temario.md) desde los PDFs de cátedra, con el detalle de lo dictado en las
  clases 2 y 3. Ese archivo es el original y le gana a esta matriz si se contradicen.
- ~~**La clase 8 no figura en la matriz.**~~ Resuelto: clase 8 (24/9) **es** la entrega del
  1er parcial, no un tema.
- ~~**Las clases 15 a 19 no están desglosadas.**~~ Resuelto: 15 y 16 son la entrega del 2do
  parcial, 17 el recuperatorio, 18 libre y 19 el final. Ninguna trae tema nuevo.
- **Ownership por área** — quién es dueño de qué Blueprints. No es una pregunta sobre el
  equipo: es la lista de áreas, y se puede escribir ya. Los sublevels y los componentes
  chicos son la parte técnica; esto es la parte acordada. **Sigue abierto.**
