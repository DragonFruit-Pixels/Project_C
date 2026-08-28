# Temario de la materia — Desarrollo Avanzado de Videojuegos

Transcripción del temario oficial (`Clase a Clase - Unreal Engine.pdf`). **Este es el original.**
[`course-alignment.md`](course-alignment.md) es la matriz derivada que cruza estos temas con el
juego; si los dos se contradicen, gana este.

Docente: Lic. Mauro Leonel Mendoza.

| Clase | Fecha | Unidad | Temas |
|---|---|---|---|
| 1 | 6/8/2026 | Fundamentos del Gameplay | Presentación · Introducción a la materia · Introducción a Unreal · Instalación de Unreal · Crear mi primer proyecto · Estructura de Proyecto · Unreal Editor · Source Control · Templates |
| 2 | 13/8/2026 | Fundamentos del Gameplay | ¿Qué es un Actor? · ¿Qué es un Blueprint? · Componentes · Blueprint Workflow · Depuración y Análisis de Blueprints · Ciclo de Vida de un Actor |
| 3 | 20/8/2026 | Fundamentos del Gameplay | Gameplay Framework · Static Mesh · Físicas y Colisiones · Materials |
| 4 | 27/8/2026 | Mecánicas y Lógica del Juego | Inputs · Posesión · Raycast |
| 5 | 3/9/2026 | Mecánicas y Lógica del Juego | Diseño de clases · Comunicación entre Blueprints · Levels y SubLevels |
| 6 | 10/9/2026 | Animación y Efectos Visuales | Animation Blueprint · Blendspace · State Machine · Notifies |
| 7 | 17/9/2026 | Animación y Efectos Visuales | UMG · Sonidos |
| **8** | **24/9/2026** | — | **Entrega 1er Parcial** |
| 9 | 1/10/2026 | Inteligencia Artificial | Introducción a IA · Navegación y Pathfinding · Percepción |
| 10 | 8/10/2026 | Inteligencia Artificial | Behaviour Tree · EQS |
| 11 | 15/10/2026 | Animación y Efectos Visuales | Landscape · Chaos Destruction · Lights · Sequencer |
| 12 | 22/10/2026 | Mecánicas y Lógica del Juego | Data Assets y Data Tables · Gameplay Tags · Save Game |
| 13 | 29/10/2026 | Optimización y Publicación | Profiler · Assets Streaming · Optimización · Packaging |
| 14 | 5/11/2026 | — | Instalación de Visual Studio · Instalación de Rider · **C++** |
| **15** | **12/11/2026** | — | **Entrega 2do Parcial** |
| **16** | **19/11/2026** | — | **Entrega 2do Parcial** |
| 17 | 26/11/2026 | — | Recuperatorio TT |
| 18 | 3/12/2026 | — | Libre |
| 19 | 17/12/2026 | — | Final |

## Detalle del material dictado

Lo que sigue sale de las presentaciones, no del temario resumido. Sirve para saber **qué se
evalúa exactamente** dentro de cada tema.

### Clase 2 — Blueprints

- **Actor**: cualquier cosa que se puede colocar en el mundo.
- **Blueprint Editor**: Components · My Blueprint · Viewport · Event Graph · Details · Palette ·
  Class Settings.
- **Componentes**: `USceneComponent` (tiene transform, se anida) vs `UActorComponent` (sin
  transform, lógica modular, con `InitializeComponent` / `BeginPlay` / `TickComponent` / `EndPlay`).
- **Workflow**: variables y sus tipos · contenedores · operadores aritméticos, de comparación y
  lógicos · nodos condicionales, de bucle, secuenciales, de puertas lógicas y de control de tiempo ·
  funciones · eventos · **funciones vs eventos** · graphs · macros · casting · enums · structs.
- **Depuración**: Buscar en Blueprints · Breakpoints (F9 poner/quitar, F5 continuar, F10 siguiente
  nodo, F11 entrar, Shift+F11 salir) · Blueprint Debugger con Call Stack y Data Flow.
- **Ciclo de vida**: `SpawnActor()` para crear · `Destroy()` marca como *pending to kill*, el actor
  sigue presente hasta el próximo paso del Garbage Collector.

Hay además 8 ejercicios prácticos (`Blueprints Ejercicios.pdf`), todos de Blueprint puro:
variables y `Print String` · una función `Sumar` · condicionales con AND/OR/NOT · array de enteros
con `ForLoop`/`ForEachLoop` · array de strings con función de búsqueda · un `Structure` `S_Enemigo`
con función de cálculo · un Actor Component contador con `Delay` · un Actor Component alarma con
`Gate`.

### Clase 3 — Gameplay Framework, Físicas y Colisiones

Jerarquía completa: `UObject` -> `AActor` -> `APawn` -> `ACharacter`; `AController` ->
`APlayerController` / `AAIController`; `AGameMode`, `AGameState`, `APlayerState`, `UGameInstance`.

La slide de `UGameInstance` menciona explícitamente los **GameInstanceSubsystems** para lógica y
servicios globales — es el respaldo del temario a que existan subsystems en el proyecto.

Físicas:

- **Chaos Physics** y sus módulos: Rigid Body · Destruction · Cloth · Vehicles · Niagara Physics ·
  Fluid/Water (experimental).
- **Tipos de colisión**: Box · Sphere · Convex · Capsule.
- **Modos**: No Collision · Query Only · Physics Only · Collision Enabled · Probe Only ·
  Query and Probe.
- **Canales de objeto** que trae el motor: WorldStatic · WorldDynamic · Pawn · Visibility · Camera ·
  PhysicsBody · Vehicle · Destructible.
- **Respuestas**: Ignore · Overlap · Block.
- **Physics Constraint** y **Physics Material**.
- **Objects, Traces & Presets**.

## Condiciones

- **No hay consigna escrita** para las entregas.
- La entrega es **grupal**.
- Se evalúa **el juego y la demostración de los temas**: un tema puede quedar sin cubrir aunque el
  juego funcione, y eso cuenta como faltante.

## Material de origen

Los PDFs no están versionados (son material de cátedra). Al 27/8/2026 estaban en `~/Downloads`:
`Clase a Clase - Unreal Engine.pdf`, `Clase 1 - Intro a Unreal.pdf`, `C1_Apuntes_intro_UE.pdf`,
`Clase 2 - Blueprints.pdf`, `Blueprints Ejercicios.pdf`, `Clase 3 - Gameplay Framework.pdf`.
