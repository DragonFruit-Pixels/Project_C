# 🔌 Comunicación y referencias

← [Índice](README.md)

Cómo se hablan los Blueprints entre sí, y por qué la elección no es de estilo: en Unreal
**cada forma de hablar tiene un costo de memoria y de merge distinto**. Es el tema de la clase
5, y es lo que decide si el proyecto sigue siendo editable en octubre.

---

## Los tres mecanismos, y cuándo va cada uno

La pregunta que decide no es "qué es más elegante". Es **cuánto sabe el que habla sobre el que
escucha**:

| Mecanismo | Cuando el que habla… | Dirección típica | Ejemplo del juego |
|---|---|---|---|
| **Llamada directa** | tiene la referencia y **es dueño** del otro | hacia abajo | el `Character` le pide a su componente `Ratchet` que sume `Toll` |
| **Blueprint Interface** | **no sabe ni le importa** de qué clase es el otro | hacia el costado | "recibí un `Hit`": lo implementan `Character`, `Servant`, `Creature`, `Ally` |
| **Event Dispatcher** | **no sabe quién escucha**, y pueden ser varios | hacia arriba | el `Ratchet` cruzó un umbral → escuchan el HUD, el `GameMode` y un VFX |

Regla práctica: **hacia abajo se llama, hacia arriba se avisa, hacia el costado se pregunta por
interfaz.** Si te encontrás casteando, casi siempre estás hablando hacia el costado con la
herramienta de hablar hacia abajo.

Tres detalles prácticos que se aprenden a golpes:

- **Interface: usá el nodo *Message*, no *Call*.** Sobre una referencia de tipo `Actor`, el nodo
  de interfaz es un mensaje: si el destino no implementa la interfaz, no pasa nada. No hay
  crash ni rama de error que escribir.
- **Las interfaces no tienen variables.** Solo funciones y eventos. Para exponer un dato va una
  función `Get...` que devuelve el valor; no se puede "poner una variable en la interfaz".
- **Para bindear un dispatcher hace falta una referencia tipada.** Por eso los dispatchers
  funcionan bien de hijo a padre: el padre ya tiene la referencia porque él lo creó. Si para
  bindear tenés que buscar al otro por el mundo, el mecanismo correcto era una interfaz.

**Castear no está prohibido.** Es legítimo cuando ya sos dueño del objeto y estás en la misma
familia de assets: el `GameMode` casteando su propio `GameState`, o un componente casteando a
su `Owner`. Lo que está prohibido es castear **para averiguar con quién estás hablando**.

## Por qué un `Cast` cuesta memoria

Cuando un Blueprint menciona otra clase —en un `Cast To`, en una variable tipada, en un
`Spawn Actor from Class`— crea una **referencia dura**. Unreal garantiza que si cargás el
primero, el segundo ya está cargado. Y el segundo arrastra sus propias referencias duras.

El resultado es una cadena: `BP_HUD` castea a `BP_Character`, que referencia su Skeletal Mesh,
sus animaciones, sus materiales, sus VFX… y todo eso entra en memoria al abrir el HUD. Nadie
lo diseñó así; se acumuló un `Cast` a la vez.

Dos herramientas del editor lo hacen visible, y conviene usarlas antes de que duela:

- **Reference Viewer** (click derecho sobre el asset): quién referencia a quién.
- **Size Map**: cuánta memoria arrastra un asset con toda su cadena.

Es clase 13 en el temario —optimización y streaming—, pero medir en clase 13 lo que se armó mal
en clase 5 solo sirve para saber cuánto hay que rehacer.

### Referencias blandas, para lo que se carga a demanda

Para lo que **no** tiene que estar en memoria siempre —el mesh de un enemigo que puede no
aparecer, el arte de una carta, un sublevel de sala— va una **referencia blanda** (`Soft Object
Reference` / `Soft Class Reference`) más `Async Load Asset`. La referencia blanda es una ruta,
no un puntero: mencionarla no carga nada.

Es el mecanismo que hace que el streaming de sublevels de la clase 13 funcione de verdad. Si
todas las salas están hard-referenciadas desde un mismo Blueprint, se cargan todas al abrir la
partida y el streaming no tiene nada que streamear.

## La dirección de las dependencias

Una regla, y alcanza para el 90% de los casos:

> **`Core/` no referencia a nadie. Todos referencian a `Core/`.**

Con el layout de `Content/Project_C/` que ya está definido en
[`project-context.md`](../../docs/unreal/project-context.md):

| Carpeta | Puede referenciar | Nunca referencia |
|---|---|---|
| `Core/` (framework, interfaces, servicios) | nada del proyecto | `Characters/`, `UI/`, `Placeables/` |
| `Characters/` | `Core/` | `UI/`, otros personajes |
| `UI/` | `Core/`, interfaces | `Characters/` concretos |
| `Data/` | interfaces y referencias **blandas** | assets duros de arte |

El caso que más se rompe: **el HUD queriendo leer las barras del personaje**. Si el widget
castea a `BP_Character_X`, la UI empieza a arrastrar arte. Lo correcto es que el widget lea por
interfaz, o mejor, que el personaje **avise** por dispatcher y el widget solo escuche.

## Por qué esto pesa doble en un trabajo grupal

Los `.uasset` son binarios: **no se mergean**. Ante un conflicto hay que elegir un archivo y
descartar el otro, y con él se descarta el trabajo de alguien.

Las referencias duras empeoran eso de una forma poco obvia: un Blueprint que referencia mucho
es un Blueprint que **mucha gente necesita abrir**. La misma disciplina que baja el uso de
memoria baja la probabilidad de colisión. Es el mismo argumento que sostiene "muchos Blueprints
chicos" en [`course-alignment.md`](../course-alignment.md).

## Dependencias

- Consume: [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md)
- Alimenta: [`04-mapa-de-clases.md`](04-mapa-de-clases.md)
- Toca del temario: clase 5 (comunicación entre Blueprints), clase 7 (UMG), clase 13
  (streaming y optimización)
