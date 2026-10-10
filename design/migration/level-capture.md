# Captura de `L_Mission_01` antes de reparentar `BP_Character`

Tomada el 2026-10-02, con `BP_Character` todavia colgado de `/Script/ProjectC.ProjectCCharacter`.

`UOccupancyComponent::CurrentSpace` es `EditInstanceOnly`: vive en el `.umap`, por instancia,
y lo asigno el usuario a mano. El reparent destruye los tres componentes declarados en C++
(`Ratchet`, `Occupancy`, `SelectionBounds`) y con ellos estas asignaciones, sin emitir un solo
error. Este archivo es la unica forma de restaurarlas.

## `Occupancy.CurrentSpace` por figura

| Actor | Espacio |
|---|---|
| `BP_Character_C_0` | `BP_Space_C_0` |
| `BP_Character_C_1` | `BP_Space_C_2` |
| `BP_Character_C_2` | `BP_Space_C_6` |
| `BP_Character_C_3` | `BP_Space_C_8` |

Paths completos: `/Game/Project_C/Maps/L_Mission_01.L_Mission_01:PersistentLevel.<nombre>`.

## Las figuras del nivel, despues de partir la jerarquia (2026-10-05)

Rutas: `Characters/BP_Character` (la base), `Characters/Player/BP_Player`,
`Characters/Enemies/BP_Enemy`. La base **no** vive en `Player/`: es de los dos bandos.

```
BP_Character                 base compartida, SIN instancias en el nivel
  |                          Body, Occupancy, SelectionBounds, Ratchet
  |                          Health, MaxHealth, AttackDamage
  |                          ApplyDamage, Die, GetSelectableName, ApplyHighlight
  |                          IsHostile -> false,  CanBeSelected -> Health > 0
  |                          BeginPlay -> OnSpawned,  EndPlay -> OnDespawned
  |
  +-- BP_Player              OnSpawned  -> RegisterFigure en el Party
  |                          OnDespawned-> UnregisterFigure
  |                          CanBeSelected -> Health > 0 y no Ratchet.IsLost
  |                          CDO: MaxHealth 5, AttackDamage 1
  |
  +-- BP_Enemy               OnSpawned  -> tinte rojo
                             IsHostile  -> true
                             CanBeSelected -> false
                             CDO: MaxHealth 2, AttackDamage 1
```

| Actor | Clase | Espacio |
|---|---|---|
| `BP_Player_C_0` | `BP_Player` | `BP_Space_C_0` |
| `BP_Player_C_1` | `BP_Player` | `BP_Space_C_2` |
| `BP_Player_C_2` | `BP_Player` | `BP_Space_C_6` |
| `BP_Player_C_3` | `BP_Player` | `BP_Space_C_8` |
| `BP_Enemy_C_0` | `BP_Enemy` | `BP_Space_C_4` |
| `BP_Enemy_C_1` | `BP_Enemy` | `BP_Space_C_7` |

Transforms: Z = 108 en las cuatro del jugador, Z = 220 en los enemigos, rotacion 0, escala 1.
X/Y segun el espacio, grilla de 500 (ver [`space-graph-capture.md`](space-graph-capture.md)).

**Lo unico que queda por instancia en el `.umap` es `Occupancy.CurrentSpace`**, y es
inevitable: es colocacion en el tablero. Todo lo demas sale del CDO de la clase.

### `IsEnemy` ya no existe

Era un bool *Instance Editable*, o sea un tilde por instancia viviendo en el `.umap` — el
mismo lugar donde este proyecto ya perdio `Neighbours` y `CurrentSpace`. Un tilde olvidado
daba una figura del jugador peleando para el otro bando sin un solo error. Lo reemplaza
`IsHostile()`, una funcion de la clase: `false` en la base, `true` en `BP_Enemy`. No se
puede equivocar por instancia porque no hay nada que tildar.

### Por que hay hooks `OnSpawned` / `OnDespawned` y no un `BeginPlay` por hija

En Blueprint, si una hija pone su propio `Event BeginPlay`, **el del padre no corre** salvo
que haya un nodo `Parent: BeginPlay`, y ese nodo **no existe en el MCP**. Sobreescribir
`BeginPlay` en las hijas se habria comido en silencio el init de vida, la colision y el
material. El padre conserva el unico `BeginPlay` y al final llama `OnSpawned`, que cada hija
sobreescribe. Las dos son "event-shape" (sin retorno ni parametros), asi que en la hija se
sobreescriben como **evento**, no como grafo de funcion: `add_function_graph` lo rechaza y
hay que usar `add_event`.

### Deuda conocida: el `Ratchet` sigue en la base

Deberia estar en `BP_Player` — el GDD no le da ratchet a los enemigos — pero **el MCP no
puede agregar un componente a un Blueprint sin Construction Script**, y ni `BP_Player` ni
`BP_Enemy` tienen uno, porque se crearon duplicando `BP_GameInstance` (que no es un Actor) y
reparentando. Ese camino se eligio porque `BlueprintTools.create` abre un modal y cuelga el
editor.

Arreglo a mano, 2 clicks: en `BP_Player`, Add Component -> `BPC_Ratchet`, nombrarlo
`Ratchet`; en `BP_Character`, borrar el componente `Ratchet`. El `CanBeSelected` de
`BP_Player` ya lo referencia y resuelve solo.

Costo de dejarlo: cada enemigo arrastra 4 enteros que nadie lee. Cero costo de runtime.

### Variables de combate: donde vive cada una, y por que

| Variable | Instance Editable | Donde vive el valor |
|---|---|---|
| `MaxHealth` | Si | CDO de la clase; editable por instancia para balance |
| `AttackDamage` | Si | idem |
| `Health` | **No** | Runtime. `BP_Character:EventBeginPlay` hace `Health = MaxHealth` |

**`Health` no es Instance Editable a proposito:** una propiedad nueva marcada Instance
Editable **no hereda el default del CDO en instancias ya serializadas**. Al agregar las
variables, las cuatro figuras del jugador aparecieron con `Health = 0` y `AttackDamage = 0`
y hubo que setearlas una por una. `MaxHealth` y `AttackDamage` siguen por instancia porque
son perillas de balance reales (ver
[`07-balance/perillas-y-constantes.md`](../gdd/07-balance/perillas-y-constantes.md)).

### La caja del espacio se come a las figuras: por que el trace del cursor va en dos pasos

Las Z no son decorativas, deciden que se puede clickear. Los numeros, de este archivo y de
[`space-graph-capture.md`](space-graph-capture.md):

| Volumen | Z centro | Media altura | Rango en Z |
|---|---|---|---|
| `BP_Space.Bounds` (BoxComponent, root) | 120 | 100 | **20 -> 220** |
| `SelectionBounds` de una figura jugador (esfera r=60) | 108 | 60 | **48 -> 168** |
| `SelectionBounds` de un enemigo (esfera r=60) | 220 | 60 | **160 -> 280** |

La caja del espacio **envuelve por completo** la esfera de una figura del jugador. La camara
mira desde arriba y `LineTraceForObjects` devuelve el hit mas cercano al origen del rayo, asi
que con los dos object types en un mismo `Make Array` el tile gana siempre y **la figura del
jugador es inclickeable**. El enemigo asoma 60 unidades por encima de la tapa (280 > 220) y
ese si gana: la asimetria no es un bug del codigo, es la geometria.

Conclusion que cuesta re-descubrir: **un solo trace con `[Space, Figure]` no puede funcionar
en este tablero.** `TraceSelectableUnderCursor` hace dos traces:

1. Solo `ObjectTypeQuery8` (Figure). Si pega, devuelve esa figura.
2. Si no pego **y hay algo seleccionado**, solo `ObjectTypeQuery7` (Space). Si pega, el tile.
3. Si no hay seleccion y no pego una figura, devuelve null: sin un personaje elegido, los
   tiles se ignoran.

Asi sale el flujo que se pidio: sin seleccion clickeas un player y lo elegis (un enemigo
tambien devuelve, pero su `CanBeSelected` es false, o sea se ignora solo); con un player
elegido, el tile libre te mueve y el enemigo te deja atacar.

**No** se resolvio aprovechando que el enemigo asoma, aunque sale mas barato en nodos: eso
ataria el ataque a que el mesh del enemigo quede mas alto que la tapa del tile, y los modelos
de Meshy van a cambiar esas alturas. La prioridad va explicita en el grafo.

### `TraceDistance`: hay dos variables con ese nombre y una es del motor

`APlayerController` trae heredada `HitResultTraceDistance` (categoria **MouseInterface**,
`Float single-precision`). El Blueprint tiene su propia `TraceDistance` (categoria
**Selection**, `Float double-precision`). En el DSL los especificadores son casi iguales:

- `Variables|MouseInterface|GetTraceDistance` -> la del **motor**, no es la que queremos
- `Variables|Selection|GetTraceDistance` -> la del **Blueprint**, es esta

Escribir la primera compila limpio y el trace anda, con la distancia equivocada. La forma de
distinguirlas sin ambiguedad es `get_node_infos`: mirar el **nombre del pin** de salida
(`TraceDistance` vs `HitResultTraceDistance`) y la precision. `read_graph_dsl` las imprime
igual a las dos, asi que la lectura del DSL **no** sirve para verificar esto.

### El color de reposo es una funcion overrideable, no una variable

Sintoma: los enemigos perdian el rojo apenas pasabas el mouse por encima. Causa: habia
**tres** respuestas distintas a "de que color es esta figura en reposo".

| Momento | Que pintaba | Resultado |
|---|---|---|
| `BeginPlay` | `SetMaterial(Body, M_Space)` y nada mas | el `Colour` default del material |
| `BP_Enemy.OnSpawned` | rojo, una sola vez al spawnear | enemigo rojo |
| `ApplyHighlight(None)` | el literal `(0.8, 0.8, 0.85)` | gris claro, ni lo uno ni lo otro |

`RefreshHighlights` arranca mandando a `None` todo lo que estaba resaltado, asi que el primer
hover sobre un enemigo le pisaba el rojo **para siempre**. El estado `None` estaba
implementado como "pinta este literal" en vez de "volve a tu color".

Ahora `BP_Character.GetBaseColour()` devuelve el color y `BP_Enemy` la **overridea**. Los
estados `None` y `Legal` de `ApplyHighlight` la llaman en vez de tener el literal, y
`BeginPlay` llama `ApplyHighlight(None)` despues del `SetMaterial`, asi que el color de reposo
vale desde el primer frame y es el mismo que se restaura despues de cada highlight.
`BP_Enemy.OnSpawned` **se borro**: ya no hace falta. Es el mismo patron que `IsHostile()`.

**Por que una funcion y no una variable.** Lo intente primero con una variable `BaseColour`
(Vector) y el valor en el CDO de cada hija. Compilo limpio, y en PIE las cuatro figuras
salieron **negras**: el CDO tenia `(0.8, 0.8, 0.85)` y las instancias del nivel `(0, 0, 0)`.

> Agregar una variable nueva a un Blueprint que **ya tiene instancias serializadas en el
> `.umap`** deja esas instancias en el valor cero, y esto pasa **aunque la variable no sea
> Instance Editable**. Es la misma trampa que con `Health` y `MaxHealth`.

Una funcion vive en la clase y no se serializa por instancia, asi que es inmune. Regla para
este proyecto: **una constante que varia por clase va como funcion overrideable, nunca como
variable con el valor en el CDO**, mientras el nivel tenga instancias ya guardadas.

`BP_Space.ApplyHighlight` se dejo con sus literales: no tiene subclases y nunca tuvo el bug.
Los estados `Hovered` y `Selected` siguen duplicados entre `BP_Space` y `BP_Character` (dos
literales iguales en cada uno). Unificarlos de verdad pide una clase base comun o una function
library, que es mas estructura de la que justifican dos colores.

### `read_graph_dsl` no sabe representar flujo convergente

Si dos ramas de un `Branch` caen en el **mismo** nodo, el lector imprime la segunda como
`(else _)`, o sea vacia. Paso con `TryActivateFigure`, que se lee asi:

```lisp
(elif (<= (GetRound _state) 0)
  (BeginRound _state)
  (SetActiveFigure _state Figure)
  (return _changed)
  (else _))                      ; <- parece un else vacio
```

y parece decir que `SetActiveFigure` solo corre cuando `Round <= 0`, o sea que ninguna figura
se activaria nunca despues de la primera ronda. **Es mentira.** `get_node_infos` sobre el
`Branch` muestra que `then` va a `BeginRound` y `else` va directo a `SetActiveFigure`, y que
`BeginRound` tambien sigue a `SetActiveFigure`: el nodo es comun a las dos ramas y la funcion
esta bien.

**Nunca reescribir una funcion por lo que dice el DSL si aparece un `(else _)`.** Verificar
con `get_node_infos` sobre el `Branch` primero. Casi reescribo una funcion correcta por esto.

### Los nodos puros se reevaluan donde se usan, no donde parecen atados

El bug mas caro de la sesion. Sintoma: mover una figura **arrastraba otras figuras con ella**
a la misma casilla.

`BPC_Occupancy.SetSpace` tenia este orden de ejecucion real (leido con `get_node_infos`, no
con el DSL):

```
1. SetCurrentSpace(NewSpace)          VariableSet
2. LeaveSpace(Target <- GetCurrentSpace)   CallFunction, Target cableado al GETTER
```

`GetCurrentSpace` es un nodo **puro**. Blueprint no cachea un nodo puro donde "parece" atado:
lo reevalua **en el momento en que el consumidor lee el pin**. Para cuando `LeaveSpace` lee su
`Target`, `CurrentSpace` ya vale el espacio **nuevo**.

```
LeaveSpace(espacio NUEVO) -> RemoveOccupant(nuevo, figura)
                          -> la figura todavia no esta ahi -> no hace nada
EnterSpace(espacio NUEVO) -> AddOccupant -> se agrega
  => el espacio VIEJO se queda con la figura, para siempre
```

Las figuras se acumulan en cada casilla que pisaron. Y `AddOccupant` llama
`RefreshOccupantPlacement()` (`Source/ProjectC/Private/Map/Space.cpp:99`), que hace
`SetActorLocation` sobre **todos** los actores del array. Entrar a una casilla teletransporta
ahi a todos sus fantasmas.

Evidencia en PIE, despues de tres turnos jugados:

| | |
|---|---|
| `BP_Space_C_1.Occupants` | `[Player_0, Player_1, Player_2]` |
| `Player_0.Occupancy.CurrentSpace` | `BP_Space_C_4` |
| `Player_1.Occupancy.CurrentSpace` | `BP_Space_C_0` |
| Posicion real de `Player_0` | `(425, -75)`, o sea un slot de `BP_Space_C_1` |

Ninguno de los tres estaba logicamente en Space_1, y aun asi Space_1 los reposicionaba.

**El arreglo**: una variable `PreviousSpace` en `BPC_Occupancy`. Se escribe **antes** de pisar
`CurrentSpace`, y `LeaveSpace`, el dispatcher `OnSpaceChanged` y el return leen esa variable.
Una escritura de variable es impura, asi que el valor queda congelado y no se reevalua.

```
SetPreviousSpace(GetCurrentSpace)   <- captura
SetCurrentSpace(NewSpace)
LeaveSpace(GetPreviousSpace)        <- el viejo, de verdad
EnterSpace(NewSpace)
OnSpaceChanged(GetPreviousSpace, NewSpace)
return GetPreviousSpace
```

**Regla**: si un valor se lee *despues* de que algo pudo haberlo cambiado, no alcanza con
leerlo "antes" en el DSL. O se guarda en una variable, o se reordena para que la lectura pase
antes de la escritura. Esto no lo detecta el compilador y no sale en los logs.

Y una trampa mas del lector: el DSL imprime esto como

```lisp
(bind _currentspace (Variables|Occupancy|GetCurrentSpace))
(if ... (Variables|Occupancy|SetCurrentSpace NewSpace)
        (CallFunction|LeaveSpace _self _currentspace _returnvalue))
```

o sea **como si el valor estuviera capturado en un `bind` previo**. No lo esta: ese `bind` es
una comodidad del lector para no repetir la expresion. Sumado al `(else _)` de los flujos
convergentes, van dos formas distintas en que `read_graph_dsl` miente sobre la semantica. Para
cualquier cosa que dependa de orden de ejecucion, `get_node_infos` es la unica fuente.
