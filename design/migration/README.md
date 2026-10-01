# Migracion de la capa de actores: C++ -> Blueprint

Rama `feature/cpp-to-blueprint`. Plan completo en el plan de sesion; esto es lo que hace
falta saber para ejecutar o para retomar si la sesion se corta.

## Que es `dsl/`

El volcado de **todos** los grafos Blueprint del proyecto, tal como estaban antes de tocar
nada, capturado con `BlueprintTools.read_graph_dsl`. Es la red de seguridad real de esta
migracion: **reparentar un Blueprint descarta las variables y funciones que el padre nuevo
no tiene**, y estos archivos son lo que permite reconstruirlas.

Se vuelven a escribir con `write_graph_dsl`, que acepta la misma sintaxis que `read_graph_dsl`
devuelve.

Incluye `BP_CameraPawn` y `WBP_MissionHUD` aunque no se migran: el primero como referencia
del patron anti-spaghetti que ya usa el proyecto (una funcion por responsabilidad:
`HandlePan`, `ConsumeOrbit`, `ApplyToCamera`), el segundo porque **si** lo toca la migracion.

## Quien consume las clases que se van

Esto es lo que hay que mantener andando. El grep de `Source/` no alcanza: los consumidores
en Content no aparecen ahi y son los que mas facil se rompen en silencio.

| Clase C++ que se va | Consumidores en `Source/` | Consumidores en `Content/` |
|---|---|---|
| `AMissionGameState` | `ProjectCCharacter.cpp`, `MissionPlayerController.cpp`, `MissionGameMode.cpp`, `TurnOrderTest.cpp` | — |
| `AMissionGameMode` | `MissionPlayerController.cpp` (4 sitios) | `WBP_MissionHUD` (`GetActiveFigure`, `GetRound`, `GetFiguresLeftThisRound`, `GetPhase`, `GetActionsRemaining`) |
| `AMissionPlayerController` | — | `WBP_MissionHUD` castea a la clase C++ y llama `GetSelectedActor` |
| `URatchetComponent` | `ProjectCCharacter.h/.cpp` | — |
| `UOccupancyComponent` | `ProjectCCharacter.h/.cpp`, `MissionGameMode.cpp` | `BP_GameMode_Mission.TryMoveFigure` (por path de clase) |
| `AMissionPlayerState` | `MissionGameMode.cpp` | — |
| `AProjectCCharacter` | — | `BP_Character` (x2) |
| `UProjectCGameInstance` | — | `BP_GameInstance` |

**`WBP_MissionHUD` es el consumidor que hay que arreglar a mano.** Su `EventConstruct` hace
`CastToMissionPlayerController` contra la clase C++; cuando esa clase no exista, el cast
falla y el HUD queda mudo sin tirar error de compilacion. Hay que repuntarlo a
`BP_PlayerController_Mission`. Y las cinco funciones que llama en el GameMode tienen que
existir en el BP **con el mismo nombre**, o el HUD se rompe igual.

## Lo que se queda en C++ y por que

| Se queda | Motivo |
|---|---|
| `FGraphMath` (278 lineas) | BFS sobre `TArray<TArray<int32>>`. Blueprint no soporta containers anidados. |
| `UGraphSubsystem` | `UWorldSubsystem` no es Blueprintable. No hay forma de hacerle un hijo BP. |
| `ASpace` | Fuera de alcance. Implementa `ISelectable` en C++. |
| `ISelectable` | Ya es `UINTERFACE(BlueprintType)` con todo `BlueprintCallable`: un BP lo llama tal cual. Moverlo rompe `ASpace`. |
| `FRatchetRules`, `FSlotLayout` | Logica pura sin estado. Se exponen por `ProjectCRulesLibrary`. |
| `ProjectCCollision` | `constexpr ECollisionChannel`. Los canales viven en `DefaultEngine.ini`. |

## Hallazgos que cambian el trabajo

1. **El seam ya funcionaba como fue disenado.** `TryMoveFigure` y `EndTurn` son
   `BlueprintNativeEvent` y `BP_GameMode_Mission` ya los implementa en nodos. Los
   `_Implementation` en C++ no son logica: son stubs que logean *"BP_GameMode_Mission is what
   implements it"* si falta el override. No son codigo muerto, son asserts.

2. **Los enums no se convierten.** Un `UENUM` vive en el **modulo**, no en el header. Moviendo
   `EMissionTurnPhase` y `EInteractionMode` a `ProjectCTypes.h` (que sobrevive), los assets
   siguen resolviendo `/Script/ProjectC.EMissionTurnPhase` y no hay que rewirear ni un nodo.
   `ESelectionHighlight` ya vive en `Selectable.h`, que tambien sobrevive.

3. **Hay cuatro assets duplicados byte a byte**, no dos:
   `Core/BP_Character` = `Characters/Player/BP_Character`, y
   `Map/Space/BP_Space` = `Placeables/BP_Space`. Fuera de alcance de esta migracion, pero
   conviene resolverlo: hoy se migran dos veces las mismas dos clases.

4. **Cuatro bindings a `OnOrderRefused`** en `BP_PlayerController_Mission`
   (`_Event`, `_Event_0`, `_Event_1`, `_Event_2`). Solo `_Event` tiene cuerpo; los otros tres
   son stubs vacios de clickear el `+` de mas. Se colapsan a uno.

5. **`BP_GameState_Mission` tiene `ReceiveTick` implementado** (vacio) y el GameState C++ no
   tickea. Resto de una prueba; no se porta.

## Baseline de tests

`RunTestsByFilter "StartsWith:ProjectC.Rules"` -> **8/8 en verde, 58 ms** (2026-10-01, antes
de empezar).

| Test | Sobrevive |
|---|---|
| `ProjectC.Rules.Graph.Distances` | si |
| `ProjectC.Rules.Graph.PushAndTies` | si |
| `ProjectC.Rules.Graph.UnreachableAndDegree` | si |
| `ProjectC.Rules.Ratchet.AdvanceAndLoss` | si |
| `ProjectC.Rules.Ratchet.Thresholds` | si |
| `ProjectC.Rules.Slots.Layout` | si |
| `ProjectC.Rules.Move.Legality` | si — solo incluye `Map/GraphMath.h`, no toca el GameMode |
| `ProjectC.Rules.Turn.Order` | **no** — instancia `AMissionGameState` |

Al terminar tienen que quedar **7/7**. Si quedan menos, algo se rompio en el C++ que se queda.

La unica baja es `TurnOrderTest.cpp` (86 lineas). `MoveLegalityTest.cpp` parecia condenado por el
nombre pero prueba `FGraphMath::Reachable` con una constante local, no el GameMode: sobrevive.

## La trampa del unity build

`GraphMathTest.cpp` y `MoveLegalityTest.cpp` definian el **mismo** `ExampleGraph()` y el mismo
`enum { S1..NodeCount }`, byte por byte, cada uno en un namespace **anonimo**. Un anonimo da
linkage interno, asi que por archivo no colisiona — pero UBT concatena `.cpp` en blobs de unity,
y ahi los dos anonimos caen en la misma unidad de traduccion y el nombre queda ambiguo.

No estallaba porque UBT usa **adaptive unity**: calcula el working set con `git status` y saca
del blob los archivos que estas editando. Mientras esos dos estuvieran limpios y en buckets
distintos, nadie lo veia. Agregar `ProjectCTypes.h` y `ProjectCRulesLibrary` invalido el makefile
("source file added"), reordeno los buckets, y los junto por primera vez.

El fixture quedo una sola vez en `Private/Tests/TestGraphs.h`, en `namespace ProjectCTest` (con
nombre) y `inline`. Los dos tests lo incluyen y hacen `using namespace ProjectCTest;` **dentro de
cada `RunTest`**, no a nivel de archivo: a nivel de archivo el using se filtraria al resto del
blob de unity, que es la misma clase de trampa.

**Consecuencia para validar:** un build donde esos archivos esten sucios en git **no prueba el
fix**, porque compilan solos. Hay que commitear y recompilar para que vuelvan al blob.
