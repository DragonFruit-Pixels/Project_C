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

3. **No hay assets duplicados: hay dos redirectors.** `Core/BP_Character` y
   `Placeables/BP_Space` son `ObjectRedirector` — el rastro que deja mover un asset en el
   editor sin correr el fixup. Apuntan a los assets reales, que son
   `Characters/Player/BP_Character` y `Map/Space/BP_Space`, y son esos dos los que usa
   `L_Mission_01`.

   Lo habia leido como duplicacion porque `read_graph_dsl` devolvia contenido identico por
   los dos caminos; era identico porque los dos resolvian al mismo objeto.
   **Son 6 Blueprints reales, no 8**, y no hay trabajo duplicado en la Fase 2.

4. **Los Class Defaults del GameMode ya estan puestos en el Blueprint.**
   `BP_GameMode_Mission` referencia `BP_GameState_Mission`, `BP_PlayerController_Mission` y
   `BP_CameraPawn`, o sea que ya sobrescribe lo que pone el constructor en C++. Y
   `GameStateClass`, `PlayerControllerClass` y `DefaultPawnClass` estan declaradas en
   `AGameModeBase`, no en `AMissionGameMode`: **sobreviven al reparent**. Lo unico que se
   pierde es lo declarado en la clase C++ propia — `ActionsPerTurn`, `SpacesPerMove` y
   `Phase`.

5. **Cuatro bindings a `OnOrderRefused`** en `BP_PlayerController_Mission`
   (`_Event`, `_Event_0`, `_Event_1`, `_Event_2`). Solo `_Event` tiene cuerpo; los otros tres
   son stubs vacios de clickear el `+` de mas. Se colapsan a uno.

6. **`BP_GameState_Mission` tiene `ReceiveTick` implementado** (vacio) y el GameState C++ no
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

## Trampas del DSL de Blueprint (encontradas escribiendo, no documentadas)

`read_graph_dsl` / `write_graph_dsl` hacen round-trip, pero el writer normaliza y en el camino
hay cosas que muerden. Todas estas salieron de fallas reales.

1. **Los eventos se llaman por su titulo, no por su UFUNCTION.** `list_events` devuelve
   `ReceiveBeginPlay`, pero el DSL quiere **`EventBeginPlay`**. Con `ReceiveBeginPlay` tira
   `AddEvent|ReceiveBeginPlay does not exist`. Igual con `EventEndPlay`, `EventTick`.

2. **Las llamadas a funciones propias y a dispatchers exponen un pin `self` como primer
   posicional.** `(CallFunction|MiFuncion a b)` conecta `a` a `self` y falla con
   *"Could not connect pin X to self"*. Usar **keyword args**:
   `(CallFunction|MiFuncion :self self :Param1 a :Param2 b)`. Lo mismo para
   `(Default|CallMiDispatcher :self self :Param x)`.

3. **`bind` dentro del cuerpo de un `for` se hoistea afuera del loop.** Esto es el peor,
   porque **compila y da el resultado equivocado en silencio**. Escribiendo
   `(for _th arr (bind _c (- _th _pos)) ...)` el `bind` sale del loop y queda colgado de un
   `ForEachLoop` suelto: `_c` no varia por iteracion. La expresion hay que **inlinearla** para
   que quede atada a la variable del loop, aunque eso duplique un nodo puro.

4. **Variables locales de funcion:** `add_variable` con el parametro `graph` las crea en ese
   grafo en vez de en la clase. Se acceden como `Variables|Default|GetX` / `SetX`. Sirve para
   acumuladores de loop sin ensuciar el estado de la clase.

5. **Los nodos multi-exec cortan el flujo.** `Utilities|IsValid` (pines `Is Valid` /
   `Is Not Valid`) termina el exec del cuerpo que lo contiene: lo que venga despues no corre.
   Dos chequeos seguidos al mismo nivel no se pueden. La salida limpia es una funcion chica por
   chequeo — que ademas es el patron anti-spaghetti que queremos.

6. **Tipos que acepta `add_variable`:** `bool, int, float, byte, name, string, text, Vector,
   Rotator, Transform, Vector2D, LinearColor`. `int32`, `integer` y `double` se rechazan.

7. **Los nombres de propiedad del CDO son camelCase**, no PascalCase: `trackLength`,
   `primaryComponentTick`. Y los dos tools no coinciden en la forma:
   `set_properties` toma `values` como **string JSON**, `get_properties` toma `properties`
   como **array de strings**.

8. **El path de un Blueprint para estos tools es `Paquete.Objeto`**
   (`/Game/.../BP_X.BP_X`), y el del CDO es `/Game/.../BP_X.Default__BP_X_C`. El path de
   paquete solo no sirve: *"is not a valid object path"*.

## Lo que el MCP no puede hacer (requiere el editor a mano)

Dos cosas, las dos chicas, las dos verificadas contra los 53 tools de `BlueprintTools`:

1. **Agregar una interfaz a un Blueprint.** Los tools solo *leen* interfaces
   (`list_events` / `list_functions` las mencionan). `ImplementedInterfaces` tampoco se
   alcanza por `ObjectTools`: el path `/Game/.../BP_X.BP_X` resuelve a la clase generada, no
   al `UBlueprint`. Afecta solo a **`BP_Character`**, que al reparentar a `Character` deja de
   implementar `ISelectable` — y de eso dependen el trace del cursor, los highlights y
   `CanActThisRound`.

2. **Crear una variable de tipo enum.** `add_variable` acepta solo
   `bool/int/float/byte/name/string/text/Vector/Rotator/Transform/Vector2D/LinearColor`;
   rechaza tanto `EMissionTurnPhase` como `/Script/ProjectC.EMissionTurnPhase`.
   `add_struct_variable` tampoco: *"is not valid ScriptStruct"*. Afecta a **`Phase`** en
   `BP_GameMode_Mission`, y por lo tanto al cuerpo de `EnterPhase`, que quedo vacio.

## Mas trampas del DSL

9. **`write_graph_dsl` no limpia el grafo de forma confiable.** Si una escritura falla a
   mitad, los nodos que alcanzo a crear **quedan**. Una segunda escritura agrega otra cadena
   completa en paralelo colgada del mismo pin de entrada, y el resultado es una funcion que
   hace todo dos veces. En `BP_GameMode_Mission` quedaron 8 nodos huerfanos repartidos en tres
   grafos, incluidos getters de propiedades C++ que ya no existian; cada compile los reportaba
   sin decir en que grafo estaban. Se encuentran con `find_nodes` + `get_node_infos` filtrando
   por `type_id`, y se borran con `delete_node`.

10. **`read_graph_dsl` mal-etiqueta nodos que comparten nombre con algo del engine.** Mi
    `IsSet` se lee como `TypedElementFramework|Handle|IsSet`, mi `EndTurn` como
    `Online|TurnBased|EndTurn`, y `Class|BPCOccupancy|SetSpace` como
    `Class|MotionExtractorModifier|SetSpace`. El nodo real es el correcto — lo confirma que el
    Blueprint compile y que el error al borrar la funcion diga
    *"Could not find a function named IsSet in BP_GameState_Mission_C"*. **La etiqueta del
    read-back no es autoridad; el compile y la inspeccion de pines si.**

11. **El read-back colapsa nodos multi-salida y queda ambiguo.**
    `(bind (_origin _extent) (Collision|GetActorBounds ...))` + `(.z _extent)` se lee como
    `(.z (Collision|GetActorBounds Figure true))`, que no dice cual de las dos salidas se uso.
    Hay que mirar `connected_pins` por pin: en `GetFigurePlacement` quedo
    `Origin conn=0 / BoxExtent conn=1`, que es lo correcto.

12. **Reparentar convierte los eventos del padre en custom events huerfanos** con sufijo `_1`.
    `OnMissionReady` y `EndTurn` quedaron como `OnMissionReady_1` y `EndTurn_1`, con su cuerpo
    intacto y sin nadie que los dispare. Se borran con `delete_node`.

13. **Las propiedades declaradas en el padre de engine sobreviven el reparent.**
    `GameStateClass`, `PlayerControllerClass` y `DefaultPawnClass` estan en `AGameModeBase`, y
    los valores que el Blueprint les habia puesto quedaron intactos. Solo se pierde lo
    declarado en la clase C++ propia.

## Trampas del DSL, segunda tanda (del PlayerController)

14. **Los eventos de Enhanced Input no se pueden crear con el DSL.**
    `Input|EnhancedActionEvents|IA_Select` existe como `type_id`, pero no hay
    `AddEvent|EnhancedInputActionIA_Select`, asi que la forma `(event ...)` falla con
    *"does not exist"*. Se crean con `create_node` y se cablean con `connect_pins`. El pin que
    hay que conectar es **`Started`**, no `Triggered`: es el `ETriggerEvent` que usaba el C++.

15. **`write_graph_dsl` reemplaza el grafo entero, incluido lo que pusiste a mano.** Si en un
    grafo convive DSL con nodos creados por `create_node`, el orden es **DSL primero, nodos
    despues**. Reescribir el EventGraph del PlayerController despues de crear los input events
    los habria borrado a los tres.

16. **`bind` sobre un nodo puro multi-salida toma la PRIMERA salida, no la que querias.**
    `(bind _actor (Collision|BreakHitResult _hr))` ataba `bBlockingHit` y fallaba al conectarlo
    a un pin de objeto. `HitActor` es la salida 10 de 18, asi que hay que usar la forma
    posicional: `(bind (_b _io _t _d _loc _ip _n _in _pm _actor) (Collision|BreakHitResult _hr))`.
    Al menos este falla ruidosamente; el de `IsValid` no.

17. **`EventDispatchers|CreateEvent` necesita su `OutputDelegate` conectado ANTES de elegir la
    funcion.** Con el pin suelto, `set_create_event_function` responde
    *"Valid functions: []"*, porque sin el delegate no conoce la firma. El orden es: conectar
    `OutputDelegate` -> `Delegate` del nodo de bind, y recien despues
    `set_create_event_function`.

18. **El nodo de bind de un dispatcher Blueprint es `Default|BindEventto<Nombre>`.** El
    `Mission|BindEventtoOnActiveFigureChanged` que aparece al lado tiene el `self` tipado
    `Mission Game State` — es el del dispatcher **C++**, que todavia existe. El de mi Blueprint
    pide `BP Game State Mission`. Dos nodos con el mismo nombre visible y distinto target: hay
    que mirar el tipo del pin `self` para saber cual es cual.

19. **No se pueden escribir propiedades del CDO hasta compilar.** Despues de
    `add_object_variable`, `set_properties` falla con *"the following properties could not be
    set"* porque el CDO todavia no tiene el campo. Compilar primero, setear despues.

20. **Las referencias a assets guardadas sobre propiedades C++ se pierden en el reparent, en
    silencio.** `BP_PlayerController_Mission` tenia `IMC_Mission`, `IA_Select`, `IA_Cancel` y
    `IA_EndTurn` como overrides de `UPROPERTY(EditDefaultsOnly)`. Hay que **leerlas antes** de
    reparentar (`get_properties` sobre el CDO) y reponerlas despues. Nada avisa: el Blueprint
    compila igual y el juego simplemente no responde al input.
