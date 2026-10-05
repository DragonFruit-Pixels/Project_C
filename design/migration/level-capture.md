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

Dos instancias mas de `BP_Character`, con `IsEnemy = true`. Mismo riesgo que la tabla de
arriba: `Occupancy.CurrentSpace` y las tres variables de combate son **por instancia** y
viven en el `.umap`.

| Actor | Espacio | `IsEnemy` | `MaxHealth` | `AttackDamage` |
|---|---|---|---|---|
| `BP_Character_C_4` | `BP_Space_C_4` | `true` | 2 | 1 |
| `BP_Character_C_5` | `BP_Space_C_7` | `true` | 2 | 1 |

Y las cuatro figuras del jugador, con los valores que hay que reponer si se pierden:

| Actor | `IsEnemy` | `MaxHealth` | `AttackDamage` |
|---|---|---|---|
| `BP_Character_C_0` .. `_3` | `false` | 5 | 1 |

**Por que estan escritos aca y no solo en el CDO:** `MaxHealth`, `AttackDamage` e `IsEnemy`
son *Instance Editable*. Una propiedad nueva marcada asi **no hereda el default del CDO en
las instancias que ya estaban serializadas**: las cuatro figuras del jugador aparecieron con
`AttackDamage = 0` despues de agregar la variable, y hubo que setearlas una por una.
`Health` **no** es Instance Editable a proposito, justamente por eso: es estado de runtime y
`BP_Character:EventBeginPlay` lo inicializa con `Health = MaxHealth`.

## Componentes a recrear en el Blueprint

`Body` **no** esta aca: es un componente agregado en el Blueprint, no en C++, y sobrevive al
reparent. Se anota igual para poder verificar que no cambio.

| Componente | Clase nueva | Valores |
|---|---|---|
| `Ratchet` | `BPC_Ratchet` | `TrackLength=20`, `Thresholds=[4,8,12,15,18,19]`, `Position=0`, `ThresholdsCrossedCount=0` — todos default |
| `Occupancy` | `BPC_Occupancy` | `CurrentSpace` vacio en el CDO, por instancia segun la tabla de arriba |
| `SelectionBounds` | `SphereComponent` | `SphereRadius=60`, relativo `(0,0,0)`, attach al `CapsuleComponent`, perfil de colision `Figure` |
| `Body` (ya existe) | `StaticMeshComponent` | `/Engine/BasicShapes/Cylinder`, relativo `(0,0,0)`, escala `(0.68, 0.68, 1.76)` |

El perfil `Figure` esta definido en `Config/DefaultEngine.ini:101` — `QueryOnly`,
objeto `Figure`, bloquea `Selectable`, ignora `Space` y `Camera`. Hay que setearlo a mano en
el componente nuevo: el constructor C++ que lo hacia desaparece.

## Las cuatro instancias son identicas salvo el espacio

Ninguna tiene tags, ninguna override la config del ratchet, las cuatro usan el mismo mesh y la
misma escala. Lo unico irrecuperable por inspeccion es la columna de espacios.
