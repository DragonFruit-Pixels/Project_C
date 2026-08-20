# 🧰 Managers y Subsystems

← [Índice](README.md)

**Sí, Unreal tiene una respuesta de primera clase para "servicios globales", y no se llama
Manager: se llama Subsystem.** Y tiene una limitación concreta que decide toda la arquitectura
de este proyecto, porque somos Blueprint-only.

---

## Qué es un Subsystem, y en qué se diferencia de un manager a mano

Un manager clásico es un singleton que alguien tiene que crear, guardar en algún lado,
inicializar en el orden correcto y destruir. Todo eso es código de plomería, y todo eso es
donde aparecen los bugs de arranque.

Un **Subsystem** es una clase que el engine instancia solo, una por ámbito, con ciclo de vida
garantizado (`Initialize` / `Deinitialize`) y con un nodo de acceso directo desde Blueprint. No
hay que crearlo, ni guardarlo, ni preocuparse por el orden.

Hay **cinco ámbitos**, y elegir el ámbito *es* la decisión de diseño:

| Subsystem | Vive mientras… | Para qué sirve | Ejemplo en este juego |
|---|---|---|---|
| `UEngineSubsystem` | corre el proceso | servicios que no saben nada del juego | telemetría, logging |
| `UEditorSubsystem` | está abierto el editor | herramientas de autor | validador del grafo de espacios |
| `UGameInstanceSubsystem` | vive el `GameInstance` — **sobrevive cambios de nivel** | servicios de partida larga | RNG con semilla, save, progresión entre misiones |
| `UWorldSubsystem` | vive el nivel | servicios que necesitan el mundo | registro del grafo de espacios, pool de enemigos |
| `ULocalPlayerSubsystem` | vive el jugador local | cosas por jugador | input, contexto de UI |

El más usado en producción es el de `GameInstance`: es el reemplazo natural del "GameManager"
que uno querría escribir.

## El límite que nos toca: **no se pueden crear desde Blueprint**

Verificado contra el código del engine instalado (UE 5.8,
`Engine/Source/Runtime/Engine/Public/Subsystems/`): las cinco clases base están declaradas así

```cpp
UCLASS(Abstract, Within = GameInstance, MinimalAPI)
class UGameInstanceSubsystem : public USubsystem
```

**Ninguna lleva `Blueprintable`.** Consecuencia directa: desde el editor de Blueprints no se
puede crear un Subsystem propio. Desde Blueprint solo se **consumen** los que ya existen —los
del engine y los que declare un módulo C++.

Y C++ es **clase 14, después del 2do parcial**. O sea que las dos entregas de la materia se
hacen sin poder escribir un Subsystem propio. Esto no es un detalle: es *la* restricción que
define dónde vive nuestra capa de servicios.

> Los que **sí** se consumen desde Blueprint ya, sin escribir C++, incluyen
> `EnhancedInputLocalPlayerSubsystem` (el `Add Mapping Context` de la clase 4). Enhanced Input
> viene con `"EnabledByDefault": true` en UE 5.8 —verificado en su `.uplugin`—, así que no hay
> nada que habilitar en el `.uproject`.

## Dónde vive entonces cada servicio

La regla es: **el servicio se hospeda en la clase del framework que tiene el mismo ciclo de
vida que tendría el Subsystem.** No es un workaround: es el mismo diseño, con otro contenedor.

| El servicio | Sería, en C++ | Hoy vive en | Migración en clase 14 |
|---|---|---|---|
| RNG con semilla | `GameInstanceSubsystem` | `BP_GameInstance` | mecánica |
| Save / carga | `GameInstanceSubsystem` | `BP_GameInstance` | mecánica |
| Registro del grafo de espacios | `WorldSubsystem` | `BP_GameState` | mecánica |
| Pool de figuras de enemigo | `WorldSubsystem` | `BP_GameState` | mecánica |
| Director de turno | ninguno — **es una regla** | `BP_GameMode` | no migra |
| Ejecutor de efectos de carta | `WorldSubsystem` | objeto propiedad del `GameMode` | mecánica |
| Ruteo de UI | `LocalPlayerSubsystem` | `BP_PlayerController` | mecánica |

**Por qué la migración es mecánica y no una reescritura:** si todos los llamadores piden el
servicio a través de una **Blueprint Interface** en lugar de castear al host, lo único que
cambia al mover el servicio a un Subsystem es el nodo que lo obtiene. La lógica de adentro se
copia tal cual. Es trabajo de una tarde, no de una semana — pero solo si la regla 2 del
[índice](README.md) se respetó desde el principio.

**El director de turno no migra a ningún Subsystem**, y vale marcarlo: no es un servicio, es el
árbitro. Los servicios responden preguntas; el `GameMode` decide. Meter reglas en un Subsystem
es el error simétrico al de meter estado en el `GameMode`.

## Subsystem/servicio vs Component: la decisión

Es la confusión más común, y se resuelve con una pregunta: **¿de quién es esto?**

| | Servicio (Subsystem o host del framework) | Actor Component |
|---|---|---|
| Cuántos hay | **uno** por ámbito | **uno por Actor** |
| Tiene transform | no | sí, si es `SceneComponent` |
| Responde | "¿cuál es el camino más corto entre A y B?" | "¿cuántas `Wounds` tengo?" |
| En este juego | grafo, mazos, pool, RNG | `Wounds`, `Reserve`, `Ratchet`, `Skills` |

Regla corta: **si la pregunta empieza con "cuál" o "cuántos en total", es un servicio. Si
empieza con "mi", es un componente.**

## El patrón que reemplaza a `Get All Actors Of Class`

`Get All Actors Of Class` recorre todos los actores del mundo cada vez que se llama, y además
esconde una dependencia global: nada en el grafo dice de quién depende ese nodo.

El patrón correcto es al revés — **los actores se anuncian**:

1. En `BeginPlay`, cada `BP_Space` se registra en el servicio de grafo del `GameState`.
2. El servicio guarda la lista y el índice de adyacencias.
3. Todo el resto le pregunta al servicio, nunca al mundo.

Cuesta diez nodos y elimina de un saque la búsqueda lineal, el orden de inicialización frágil
y las dependencias invisibles. Para el cálculo de distancias del GDD
([`mapa-y-espacios.md`](../gdd/01-fundamentos/mapa-y-espacios.md)) es directamente un
requisito: un BFS que empieza por escanear el mundo no es un BFS, es dos.

## Anti-patrones, con la razón y no con el dogma

| Anti-patrón | Por qué duele **acá** |
|---|---|
| **Un `BP_GameManager` que hace todo** | Es un `.uasset` binario que todo el equipo necesita el mismo día: el primer merge borra el trabajo de alguien. Y hard-referencia todo, así que abrirlo carga medio proyecto |
| **`Cast To` para hablar** | Crea referencia dura → el asset destino y su cadena entran en memoria. Ver [`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md) |
| **`Get All Actors Of Class`** | Búsqueda lineal + dependencia invisible. Ver arriba |
| **Lógica en el Level Blueprint** | No se reusa, no se hereda, y es el archivo más disputado del repo |
| **`Event Tick` en un juego por turnos** | No hay nada que tickear: si algo tickea es porque una fase no está declarada. Es el costo que después aparece en el Profiler de la clase 13 |
| **El `GameMode` guardando el estado que otros leen** | Nadie debería castear al `GameMode` para leer el `Doom Track`. Ese dato es del `GameState` |
| **Un Subsystem con reglas adentro** | Los servicios responden, el árbitro decide. Si el servicio decide, hay dos árbitros |

## Dependencias

- Consume: [`01-por-donde-se-empieza.md`](01-por-donde-se-empieza.md) (mapa de autoridad)
- Alimenta: [`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md),
  [`04-mapa-de-clases.md`](04-mapa-de-clases.md)
- Toca del temario: clase 3 (framework), clase 5 (diseño de clases y comunicación), clase 12
  (datos), clase 13 (optimización), clase 14 (C++ — la migración)
