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

## Enemigos, agregados el 2026-10-05

Los enemigos son instancias de **`BP_Enemy`**, hija de `BP_Character` con **cero nodos**:
toda la logica vive en el padre, la hija solo lleva numeros en su CDO.

| Actor | Clase | Espacio |
|---|---|---|
| `BP_Enemy_C_0` | `BP_Enemy` | `BP_Space_C_4` |
| `BP_Enemy_C_1` | `BP_Enemy` | `BP_Space_C_7` |

Lo unico por instancia es `Occupancy.CurrentSpace` (es colocacion, no puede ser de otra
forma). `IsEnemy`, `MaxHealth` y `AttackDamage` salen del CDO de `BP_Enemy`
(`true`, 2, 1) y **no** estan en el `.umap`.

## Variables de combate: donde vive cada una, y por que

| Variable | Instance Editable | Donde vive el valor |
|---|---|---|
| `IsEnemy` | **No** | CDO de la clase. `BP_Character` false, `BP_Enemy` true |
| `MaxHealth` | Si | Por instancia. Jugadores 5, enemigos 2 (del CDO de `BP_Enemy`) |
| `AttackDamage` | Si | Por instancia. 1 en todos |
| `Health` | **No** | Runtime. `BP_Character:EventBeginPlay` hace `Health = MaxHealth` |

**`IsEnemy` dejo de ser Instance Editable a proposito.** De que bando sos es un hecho de la
clase, no un tilde en el nivel: como checkbox por instancia vivia en el `.umap`, que es
exactamente donde este proyecto ya perdio datos dos veces (`Neighbours` y `CurrentSpace`),
y un tilde olvidado daba una figura del jugador peleando para el otro bando sin un solo error.

**`Health` tampoco es Instance Editable, y por otra razon:** una propiedad nueva marcada
Instance Editable **no hereda el default del CDO en instancias ya serializadas**. Al agregar
las variables, las cuatro figuras del jugador aparecieron con `Health = 0` y
`AttackDamage = 0` y hubo que setearlas una por una. `MaxHealth` y `AttackDamage` siguen por
instancia porque son perillas de balance reales (ver
[`07-balance/perillas-y-constantes.md`](../gdd/07-balance/perillas-y-constantes.md)); los
valores de los jugadores son `MaxHealth = 5`, `AttackDamage = 1` en las cuatro.
