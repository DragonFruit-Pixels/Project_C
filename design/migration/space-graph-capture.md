# Captura del grafo del tablero, antes de sacar `ASpace`

**Fecha:** 2026-10-02
**Nivel:** `/Game/Project_C/Maps/L_Mission_01`
**Motivo:** `ASpace::Neighbours` y `ASpace::BlockedTowards` son `EditInstanceOnly`
(`Source/ProjectC/Public/Map/Space.h:17-22`). Viven **por instancia dentro del `.umap`**, no en
el Blueprint, y el reparent de `BP_Space` a `/Script/Engine.Actor` los descarta sin avisar.
Este archivo es la única forma de recuperarlos. Mismo caso que
[`level-capture.md`](level-capture.md), que es la captura equivalente de
`UOccupancyComponent::CurrentSpace`.

Leído con `ObjectTools.get_properties` y `ActorTools.get_actor_transform` sobre las instancias
del nivel, con el editor abierto y el PIE detenido.

## Adyacencia

`BlockedTowards` está **vacío en los 9 espacios**. No hay ninguna arista bloqueada en este
nivel.

| Espacio | `Neighbours` | Grado |
|---|---|---|
| `BP_Space_C_0` | `1` | 1 |
| `BP_Space_C_1` | `0`, `2`, `4`, `6` | 4 |
| `BP_Space_C_2` | `1`, `3` | 2 |
| `BP_Space_C_3` | `2`, `4` | 2 |
| `BP_Space_C_4` | `1`, `3`, `5`, `7` | 4 |
| `BP_Space_C_5` | `4`, `8` | 2 |
| `BP_Space_C_6` | `1`, `7` | 2 |
| `BP_Space_C_7` | `4`, `6`, `8` | 3 |
| `BP_Space_C_8` | `5`, `7` | 2 |

**11 aristas, 9 nodos, simetría verificada arista por arista.** `SealGraph`
(`GraphSubsystem.cpp:24-80`) rechaza el grafo si una adyacencia es asimétrica, así que esto
tiene que seguir cumpliéndose después de re-wirear.

```
                2 ─── 3
                │     │
    0 ─── 1 ────┼──── 4 ─── 5
                │     │     │
                6 ─── 7 ─── 8
```

Aristas, en orden canónico (A < B): 0-1, 1-2, 1-4, 1-6, 2-3, 3-4, 4-5, 4-7, 5-8, 6-7, 7-8.

## Transforms

El `Bounds` (BoxComponent) es el root y lo crea el **constructor de C++**. Si el reparent
reemplaza el root component, la transform por instancia puede volver al default del CDO. Por
eso se capturan: rotación 0 y escala 1 en los nueve, Z = 120 en los nueve.

| Espacio | X | Y |
|---|---|---|
| `BP_Space_C_0` | 0 | 0 |
| `BP_Space_C_1` | 500 | 0 |
| `BP_Space_C_2` | 500 | -500 |
| `BP_Space_C_3` | 1000 | -500 |
| `BP_Space_C_4` | 1000 | 0 |
| `BP_Space_C_5` | 1500 | 0 |
| `BP_Space_C_6` | 500 | 500 |
| `BP_Space_C_7` | 1000 | 500 |
| `BP_Space_C_8` | 1500 | 500 |

Grilla de 500 unidades. `SlotSpacing` = 150 en todos (es `EditDefaultsOnly`, vive en el
Blueprint y **no** está en riesgo).

## Figuras

Re-verificado contra `level-capture.md`: **coincide, sin cambios**.

| Figura | `Occupancy.CurrentSpace` |
|---|---|
| `BP_Character_C_0` | `BP_Space_C_0` |
| `BP_Character_C_1` | `BP_Space_C_2` |
| `BP_Character_C_2` | `BP_Space_C_6` |
| `BP_Character_C_3` | `BP_Space_C_8` |

## Cómo restaurar

Después del reparent, por cada instancia:

```
ObjectTools.set_properties(
  instance = {"refPath": ".../PersistentLevel.BP_Space_C_<n>"},
  properties = {"Neighbours": [{"refPath": ".../BP_Space_C_<m>"}, ...]})
```

Y verificar leyendo de vuelta las 9 filas de la tabla de adyacencia antes de dar el paso por
terminado. La simetría es la comprobación barata: la suma de los grados tiene que dar 22.
