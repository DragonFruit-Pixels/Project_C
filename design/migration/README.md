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

## Trampas del DSL, tercera tanda (del Character)

**21. El reparent se come una interfaz implementada a mano, si el padre viejo la implementaba
en C++.** Es la peor de todas porque invita a hacer el paso manual en el momento equivocado.
`BP_Character` tenia `Selectable` agregada explicitamente — verificado en el `.uasset`, que
llevaba un `BPInterfaceDescription` que `BP_Space` y `BP_CameraPawn` no tienen. Al reparentar
de `AProjectCCharacter` a `Character`, Unreal conforma `ImplementedInterfaces` y descarta la
entrada como duplicada del padre. Resultado: `CanBeSelected` y `GetSelectableName` desaparecen
de la lista de funciones y `SetHighlight` queda como evento huerfano `SetHighlight_1`.
**La interfaz se agrega despues del reparent, nunca antes.**

**22. `get_node_type_pins` crea nodos de verdad en el grafo.** No es una consulta: sondear
cuatro tipos deja cuatro `K2Node_CallFunction_N` colgados. Sondear en un grafo que se va a
reescribir entero, o barrer despues.

**23. Un prefijo de categoria puede resolver a la clase C++ en vez del Blueprint.**
`Mission|RegisterFigure` y `Ratchet|IsLost` apuntan a `AMissionGameState` y
`URatchetComponent`; `Class|BPGameStateMission|RegisterFigure` y `Class|BPCRatchet|IsLost`
apuntan a los Blueprints. `find_node_types` devuelve las dos variantes y no dice cual es cual:
hay que mirar el tipo del pin `self` con `get_node_type_pins`. Si se elige mal, el write falla
con `Could not connect pin X to self` — que es el caso bueno, porque falla fuerte.

Para auditar si un grafo ya escrito quedo apuntando al C++: `find_nodes` + `get_node_infos`
y mirar el `type_id` del pin `self`. Los 15 grafos del GameMode dieron
`BP Game State Mission Object Reference` en todas las llamadas, o sea bien.

**24. Una funcion Blueprint no es pure salvo que se la declare, asi que un `BlueprintPure` de
C++ portado a BP gana pines de exec y se poda en silencio.** `URatchetComponent::IsLost()` era
`BlueprintPure`; `BPC_Ratchet.IsLost` no. Usada inline en
`(return (not (Class|BPCRatchet|IsLost ...)))` se podo, se leyo como `false`, y
`CanBeSelected` devolvia `true` para **toda** figura, incluso una perdida. Compilaba limpio sin
`warnings_as_errors`. El fix es el `bind` explicito. No hay tool para marcar una funcion como
pure: o `bind`, o se hace a mano en el editor.

**25. `write_graph_dsl` no borra el evento huerfano que dejo el reparent.** El
`Custom|SetHighlight_1` sobrevivio a reescribir el EventGraph completo y hubo que borrarlo por
nodo. Y al borrarlo quedo huerfano el `ApplyHighlight` que colgaba de el: hace falta un segundo
barrido.

**26. `collisionProfileName` no se puede leer ni escribir directo, y el nombre del perfil no
resuelve el canal.** `set_properties` sobre `collisionProfileName` avisa
`properties could not be set`. Va por `bodyInstance.collisionProfileName`. Pero eso solo deja
el `objectType` en `ECC_WorldDynamic`, porque no dispara el `LoadProfileData` que el panel de
detalles si dispara: hay que escribir tambien `bodyInstance.objectType`. Referencia buena para
comparar: el `Bounds` de `BP_Space`, que lo puso el constructor C++, queda en
`Space / ECC_GameTraceChannel1 / QueryOnly`.

**27. `get_connected_subgraph` es componente conexo, no alcanzabilidad por exec.** No sirve
para encontrar cadenas duplicadas: las copias comparten los nodos de entrada (el
`Variables|Self-Reference`, los pines de parametro) y por eso cuentan como un solo componente.
`TryMoveFigure` da 39 de 39 "alcanzables" con unos 15 nodos vivos. La deteccion real es contar
ocurrencias por `type_id` y compararlas contra el DSL leido.

## Deuda conocida que queda

- ~~`BP_GameMode_Mission:TryMoveFigure` tiene la cadena duplicada~~ **resuelto**: los
  duplicados no eran un write fallido, eran el artefacto del override de `BlueprintNativeEvent`
  (ver trampa 28). Recrear el grafo los elimino: 39 nodos -> 18, cada tipo una sola vez.
- **Los helpers `IsSet` por clase** deberian plegarse dentro de `ProjectCRulesLibrary` en el
  proximo build de C++.
- **`BPC_Ratchet.IsLost` y las otras queries deberian ser pure**, para que usarlas inline no
  sea una trampa. Es un cambio a mano en el editor.

## Trampa 28, la que casi rompe la Fase 3 en silencio

**Un override de `BlueprintNativeEvent` sobrevive al reparent como grafo, pero se queda con la
firma declarada en C++.** Esto es lo que mas cerca estuvo de pasar desapercibido, porque todas
las senales apuntaban a que no habia problema:

- `BP_GameMode_Mission` ya estaba reparentado a `GameModeBase` desde el commit 369a1fb.
- `list_functions` mostraba `TryMoveFigure` como `bIsImplemented: true`, junto a las otras 15.
- `read_graph_dsl` lo leia como `(fn TryMoveFigure (Figure To) ...)`, identico en forma a
  cualquier funcion Blueprint.
- Compilaba limpio con `warnings_as_errors`, antes y despues del reparent.

Razone desde eso que el reparent lo habia convertido en funcion Blueprint normal. No: los pines
`Figure`, `To` y el `Return Value` los seguia declarando el `UFUNCTION` de `AMissionGameMode`.
Al borrar la clase en la Fase 3 el nodo de entrada perdio los tres y aparecio

    In use pin  Figure  no longer exists on node  TryMoveFigure
    Return nodes don't match each other
    Function 'TryMoveFigure' called from  TryMoveFigure  should not be called from a Blueprint

el ultimo desde `BP_PlayerController_Mission`, que lo llamaba por el stub.

Lo unico que lo destapo fue **recompilar los Blueprints con el editor reabierto despues de
borrar el C++**. El build de C++ y los 7 tests pasaron igual: no tocan Blueprints. Un barrido de
compilacion despues de la Fase 3 no es opcional.

### Como se arregla

1. `remove_function_graph`.
2. **`compile_blueprint` en el medio**, si no el nombre sigue reservado y `add_function_graph`
   devuelve `TryMoveFigure_0`.
3. `add_function_graph`, y la firma a mano con `add_object_function_param` x2 (`Figure` ->
   `/Script/Engine.Actor`, `To` -> `/Script/ProjectC.Space`) y `add_function_param`
   (`ReturnValue`, `bool`, `input_param: False`).
4. `write_graph_dsl` del backup, con los `(return false)` restaurados: el grafo roto los leia
   como `(return)` porque habia perdido el pin.
5. En el consumidor, cambiar la llamada implicita por la explicita:
   `Class|BPGameModeMission|TryMoveFigure`. Se verifica mirando que el pin `self` del nodo diga
   `BP Game Mode Mission Object Reference`.

`EndTurn` era tambien `BlueprintNativeEvent` y **no** se rompio, porque no tiene parametros ni
retorno: no habia firma que perder. Y `HandleEndTurn` en el PlayerController ya lo llamaba por
`Class|BPGameModeMission|EndTurn`, la forma explicita, que es justamente la que aguanta.

### Falsa alarma, para no volver a perseguirla

Los tres eventos de Enhanced Input leen **sin cuerpo** en el DSL:

    (event EnhancedInputActionIA_Select (ActionValue ElapsedSeconds TriggeredSeconds InputAction))

No estan desconectados. El lector del DSL no sabe seguir las conexiones de esos nodos, igual que
no sabe crearlos (trampa 14). Se verifica con `find_nodes` + `get_node_infos` mirando el pin
`Started`: los tres lo tienen conectado.

## Trampa 29, la que rompio el juego sin romper ninguna compilacion

**Lo que hacia el constructor de C++ sobre un componente no sobrevive como default de la
plantilla, para los actores que ya estan puestos en un nivel.**

`AProjectCCharacter()` hacia `SelectionBounds->SetCollisionProfileName(Figure)` y
`SetSphereRadius(60)`. El constructor corre para cada instancia, asi que las cuatro figuras de
`L_Mission_01` lo tenian. Al pasar a Blueprint eso quedo como valores de la plantilla del SCS,
y las figuras que ya existian en el nivel **no los tomaron**: se quedaron con
`OverlapAllDynamic`, `ECC_WorldDynamic` y radio 32.

`GetHitResultUnderCursorForObjects` traza por **object type**, no por canal de trace. Con el
objeto en `WorldDynamic` en vez de `ECC_GameTraceChannel2`, el trace no pegaba en nada: sin
hover, sin seleccion, sin partida. Los 11 Blueprints compilaban limpio, `pruned` daba 0 y los
7 tests estaban en verde.

### Por que mi verificacion no lo vio

Lei el CDO, lo vi correcto (`Figure` / `ECC_GameTraceChannel2` / `Selectable: ECR_Block`) y di
por hecho que las instancias lo heredaban. El CDO era la respuesta correcta a la pregunta
equivocada. **Para cualquier cosa que el constructor C++ hacia sobre un componente, hay que
leer la instancia del nivel, no el CDO.**

### Y por que el ConstructionScript no alcanzo

Moverlo al ConstructionScript parecia el reemplazo natural del constructor, y a medias lo es:
`SetSphereRadius` ahi si se aplico (lo verifique poniendo 77 y leyendo 77 en PIE). Pero
`SetCollisionObjectType` y `SetCollisionProfileName` no: **los overrides por instancia que el
nivel tiene guardados se re-aplican despues de correr el ConstructionScript** y pisan lo que
este haya seteado.

La solucion es **BeginPlay**, donde ya no queda nada que lo pise:

```lisp
(event EventBeginPlay
  (bind _bounds (Variables|Default|GetSelectionBounds))
  (Collision|SetCollisionEnabled :self _bounds :NewType "QueryOnly")
  (Collision|SetCollisionObjectType :self _bounds :Channel "ECC_GameTraceChannel2")
  (Collision|SetCollisionResponsetoChannel :self _bounds :Channel "ECC_GameTraceChannel3" :NewResponse "ECR_Block")
  ...)
```

Ojo tambien con la lectura: `set_properties` sobre `collisionProfileName` avisa
`could not be set`, y leer `bodyInstance.collisionProfileName` despues de un
`SetCollisionProfileName` sigue devolviendo el valor viejo. `objectType` si refleja el cambio.
Lo unico confiable para saber si una escritura entro es poner un valor inconfundible y leerlo
de la instancia viva en PIE.

## Un bug de la camara que no era de la migracion

`BP_CameraPawn:ConsumePan` rompia un `Vector2D` **vacio** en vez de `PendingPan`: el pin
`InVec` del `BreakVector2D` estaba desconectado y en todo el grafo no existia un nodo
`GetPendingPan`. `HandlePan` acumulaba bien el input de WASD, pero el consumidor leia siempre
cero, asi que la camara no se movia y Q/E (que van por `ConsumeOrbit`) si funcionaban.

Un cable, y venia de antes de la migracion. Vale como recordatorio de que el DSL lo mostraba
como `(Math|Vector2D|BreakVector2D 0)` — ese `0` no es un literal del autor, es un pin de
entrada sin conectar.
