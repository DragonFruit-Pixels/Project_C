# 🗂️ Mapa de clases

← [Índice](README.md)

El esqueleto concreto de Project_C: qué Blueprint existe, de qué estado es **dueño**, qué lee, y
a qué clase del temario corresponde. Es la tabla del paso 1 de
[`01-por-donde-se-empieza.md`](01-por-donde-se-empieza.md), llena.

**Los nombres siguen las convenciones del proyecto** (`BP_`, `BPI_`, `WBP_`, `ABP_`, `DA_`,
`DT_`, componentes con sufijo `Component`). Ver
[`project-context.md`](../../docs/unreal/project-context.md).

> Todo el vocabulario de estado es el del GDD:
> [`vocabulario.md`](../gdd/01-fundamentos/vocabulario.md). No hay nombres temáticos acá tampoco.

---

## El framework

| Asset | Padre | **Escribe** (es dueño) | Lee | Clase |
|---|---|---|---|---|
| `BP_GameInstance_ProjectC` | `GameInstance` | semilla del RNG, partida en vuelo, progresión entre misiones | — | 3, 12 |
| `BP_GameMode_Mission` | `GameModeBase` | fase del turno, contador de acciones, resultado de tiradas, victoria y derrota | todo el `GameState` | 3 |
| `BP_GameState_Mission` | `GameStateBase` | `Doom Track`, ronda, personaje activo, mazos, grafo de espacios, figuras vivas | — | 3 |
| `BP_PlayerController_Mission` | `PlayerController` | selección de personaje, posesión, widgets, cámara | `GameState` por interfaz | 3, 4, 7 |
| `BP_Character_Base` | `Character` | su posición y su presentación | sus componentes | 2, 6 |
| `BP_Enemy_Base` | `Character` | su posición y su presentación | su `DA_EnemyType` | 2, 9, 10 |
| `AIC_Enemy` | `AIController` | la decisión de esta activación | percepción, EQS | 9, 10 |

**El reparto que importa**: el `GameMode` **decide** y el `GameState` **guarda**. Nadie castea
al `GameMode` para leer un dato, porque el `GameMode` no tiene datos que otros necesiten. Es la
regla 1 del [índice](README.md) aplicada al caso más frecuente.

## Los componentes: las capacidades del GDD

Uno por sistema del GDD, y cada uno es un archivo chico —lo que además los hace repartibles
entre personas sin colisionar.

| Componente | Dueño de | Sistema del GDD |
|---|---|---|
| `BP_WoundsComponent` | `Wounds` actuales y su capacidad | [`barras-y-recursos.md`](../gdd/02-personaje/barras-y-recursos.md) |
| `BP_ReserveComponent` | `Reserve` disponible | idem |
| `BP_RatchetComponent` | posición en el track, umbrales cruzados, `Ratchet Card` asignada | [`trinquete.md`](../gdd/02-personaje/trinquete.md) |
| `BP_SkillsComponent` | nivel de cada skill y los dados bonus ganados | [`skills.md`](../gdd/02-personaje/skills.md) |
| `BP_InventoryComponent` | `Trinket`, `Ally` y `Burden` en mano | [`contenido-de-una-mision.md`](../gdd/05-partida/contenido-de-una-mision.md) |
| `BP_OccupancyComponent` | en qué `Space` está esta figura | [`mapa-y-espacios.md`](../gdd/01-fundamentos/mapa-y-espacios.md) |

`BP_WoundsComponent` va tanto en el `Character` como en el `Enemy` y en el `Ally`. Ese reuso
entre clases que no comparten padre es la razón por la que esto son componentes y no herencia.

## El mapa: el grafo

El GDD define el mapa como un **grafo, no una grilla**
([`mapa-y-espacios.md`](../gdd/01-fundamentos/mapa-y-espacios.md)), y el grado de los nodos es
un valor que las reglas consultan
([`vocabulario-de-efectos.md`](../gdd/03-resolucion/vocabulario-de-efectos.md)). Eso fija la
implementación:

| Pieza | Qué es | Nota |
|---|---|---|
| `BP_Space` | un Actor por nodo, colocado en el nivel | se **registra** en el servicio de grafo en `BeginPlay` |
| `BP_Gate` | Actor de punto de spawn | 3 por mapa, uno por color |
| Servicio de grafo | funciones en `BP_GameState_Mission` | adyacencias, BFS con aristas bloqueables, grado de un nodo |
| Aristas | pares de `BP_Space` con un flag de bloqueo | una pared que se rompe **agrega** una arista en runtime |
| Sala | un **SubLevel** por sala | 1 a 3 `Space` por sala. Clase 5 |

**Por qué `Space` es un Actor y no solo datos:** tiene que existir en el mundo para que el
raycast del mouse lo seleccione (clase 4), para que el NavMesh y el EQS lo usen como destino
(clases 9-10) y para que las salas puedan editarse en sublevels distintos por personas
distintas. El grafo *lógico* igual vive en el servicio, no repartido entre los actores.

## Los objetos que no son actores

Blueprint también hereda de `Object`. Es el lugar correcto para lógica que no tiene cuerpo:

| Asset | Padre | Para qué |
|---|---|---|
| `BP_Deck` | `Object` | mezclar, robar, descartar, rebarajar. **Una clase, dos instancias**: `Pressure Deck` y `Reward Deck` |
| `BP_EffectExecutor` | `Object` | resuelve un efecto de carta paso a paso, con la semántica de "un paso que no se puede ejecutar se saltea" |
| `BPFL_GraphMath` | `BlueprintFunctionLibrary` | BFS y distancias, funciones puras y testeables |

`BP_EffectExecutor` es la contracara directa del
[vocabulario de efectos](../gdd/03-resolucion/vocabulario-de-efectos.md): selectores,
condiciones y acciones. Mientras las cartas sean pocas, los efectos se escriben a mano; el
ejecutor existe desde el principio para que la clase 12 tenga dónde enchufar las Data Tables
sin tocar a los llamadores.

## Los datos

Existen desde el día 1 **con valores puestos a mano**, y se llenan de verdad en la clase 12.

| Asset | Contiene | Fuente |
|---|---|---|
| `DA_MissionConfig` | **todos los números**: acciones por turno, espacios por `Move`, umbral de `Doom`, capacidades | [`perillas-y-constantes.md`](../gdd/07-balance/perillas-y-constantes.md) |
| `DA_EnemyType` | vida, dados de ataque, comportamiento, mesh (referencia **blanda**) | [`enemigos.md`](../gdd/04-oposicion/enemigos.md) |
| `DT_PressureCards` | las 16 cartas de presión | [`reloj-y-avance.md`](../gdd/04-oposicion/reloj-y-avance.md) |
| `DT_RewardCards` | las 15 de recompensa | [`contenido-de-una-mision.md`](../gdd/05-partida/contenido-de-una-mision.md) |
| `DT_RatchetCards` | las 8 de trinquete, con sus dos ramas | [`trinquete.md`](../gdd/02-personaje/trinquete.md) |
| `DT_Skills` | 6 skills × 4 niveles | [`skills.md`](../gdd/02-personaje/skills.md) |
| `DA_AdversaryStage` | los 4 stages y sus efectos | [`objetivo-y-adversario.md`](../gdd/04-oposicion/objetivo-y-adversario.md) |

**`DA_MissionConfig` es la costura más importante del proyecto.** Es lo que convierte la clase
12 en una migración de 20 minutos en lugar de una cacería de números por todos los grafos.

## Las interfaces

| Interfaz | La implementan | Para |
|---|---|---|
| `BPI_Damageable` | `Character`, `Servant`, `Creature`, `Adversary`, `Ally` | recibir `Hit` sin saber qué es el destino |
| `BPI_Occupant` | todo lo que ocupa un `Space` | preguntar y mover ocupación |
| `BPI_EffectTarget` | figuras, `Space`, mazos | ser apuntado por un efecto de carta |
| `BPI_Selectable` | `Space`, figuras, dados | ser elegido por el mouse (clase 4) |

Cuatro interfaces cubren casi todo el tráfico lateral del juego. Es deliberado: cada interfaz
extra es una decisión más al implementar una carta nueva.

## La UI

Todos los widgets son propiedad del `PlayerController`, y **leen por interfaz o escuchan
dispatchers** — nunca castean a un personaje concreto (ver
[`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md)).

| Widget | Muestra |
|---|---|
| `WBP_HUD_Mission` | contenedor |
| `WBP_CharacterPanel` | ×4: `Wounds`, `Reserve`, `Ratchet` y skills de un personaje |
| `WBP_DoomTrack` | el reloj |
| `WBP_DiceTray` | dados, rerolls y gasto de `Reserve` |
| `WBP_CardView` | una carta cualquiera, alimentada por Data Table |
| `WBP_DiscardPile` | **la pila de descarte de presión, visible** |

> El descarte visible no es un lujo: en la mesa es información pública gratis y es lo que hace
> legible la presión del reloj. Si no se construye, se pierde la mecánica entera. Está marcado
> como riesgo de fase 1 en [`course-alignment.md`](../course-alignment.md).

## La costura sin resolver: ¿quién es la autoridad del dado?

El temario pide **físicas y colisiones** (clase 3) y el candidato obvio son los dados. Pero eso
abre una pregunta que es de arquitectura y de diseño a la vez, y **no está decidida**:

| Opción | A favor | En contra |
|---|---|---|
| **La física decide** — se tiran mallas con colisión y se lee la cara que quedó arriba | demostración de físicas indiscutible; el jugador ve que no hay truco | el dado se puede caer de la mesa o quedar de canto → hace falta timeout y re-tirada; guardar la partida a mitad de tirada es un caso feo; los rerolls por skill se vuelven una coreografía |
| **La lógica decide, la física presenta** | resultado determinista, save trivial, rerolls limpios | si el jugador nota que la animación está guionada, la tirada pierde credibilidad |
| **Híbrido**: la física decide, con un resolvedor que detecta "asentado" y cae a lógica si no se asienta en N segundos | conserva la demostración y no se puede colgar | dos caminos que mantener |

**Recomendación: híbrido**, y que el resolvedor —no el actor del dado— sea el único que declara
el resultado. La física entra al proyecto como *fuente* de un valor, no como dueña del estado
del juego; ese aislamiento es lo que hace que las otras dos opciones sigan disponibles después.

Queda anotado como decisión abierta **A-09** en
[`abiertas.md`](../gdd/06-decisiones/abiertas.md).

## Lo que este mapa **no** decide todavía

- La cámara: fija por sala, orbital libre, o seguimiento del personaje activo. Es UX y arrastra
  el layout de las salas.
- Cómo se representa una figura "colocada" sin que los enemigos la sigan (la excepción de
  *Codependency*): es una regla del GDD sin implementación pensada.
- Si el `Adversary` es un `BP_Enemy_Base` con más vida o una clase propia. Depende de cuántas de
  sus reglas sean excepciones.

## Dependencias

- Consume: [`01-por-donde-se-empieza.md`](01-por-donde-se-empieza.md),
  [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md),
  [`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md), y todos los
  documentos de sistema del [GDD](../gdd/README.md)
- Alimenta: [`05-temario-como-orden-de-construccion.md`](05-temario-como-orden-de-construccion.md)
