# ⏸️ Resolución y pausa

← [Índice](README.md)

Cartas, dados y acciones parecen tres sistemas distintos. **Son el mismo**: resoluciones de
varios pasos que pueden detenerse a mitad esperando una decisión del jugador. Este documento
define el mecanismo único que las tres usan, y es el que decide si el save, el log y los tests
son gratis o son imposibles.

---

## La evidencia: el juego se pausa en todos lados

No es una hipótesis. Son cuatro reglas del GDD, una por sistema:

| Regla | Documento | Dónde se detiene |
|---|---|---|
| "Si la carta invoca varios sin especificar orden, **elige el jugador**" | [`enemigos.md`](../gdd/04-oposicion/enemigos.md) | a mitad de una carta de presión |
| "Gastás 1 `Reserve` para rerollear 1 dado… todas las veces que quieras" | [`dados.md`](../gdd/03-resolucion/dados.md) | a mitad de una tirada, **N veces** |
| "`Recover`: curás 3 puntos repartidos entre `Wounds` y `Reserve` **como quieras**" | [`turno.md`](../gdd/03-resolucion/turno.md) | a mitad de una acción |
| "Sin `Brawling`, el objetivo se elige **antes** de tirar" | [`turno.md`](../gdd/03-resolucion/turno.md) | antes de una tirada |

Y las cartas de trinquete agregan el caso feo: son **condicionales de dos ramas** que leen
estado —el descarte, tu inventario, la posición de otro personaje— y cada rama puede a su vez
pedir una decisión. Ver [`vocabulario-de-efectos.md`](../gdd/03-resolucion/vocabulario-de-efectos.md).

## Por qué la solución obvia no existe

La forma natural de escribir esto es una función que resuelve la carta de arriba hacia abajo. Y
esa función, en el paso 3, necesita preguntarle algo al jugador y **seguir con el paso 4 cuando
conteste**. Una función no puede hacer eso.

Las tres salidas que aparecen solas, y por qué las tres son peores que el problema:

- **Lógica adentro de los callbacks de la UI.** El widget del reroll termina sabiendo qué venía
  después de la tirada. Es la regla 2 del [índice](README.md) rota de la peor manera: la UI
  pasa a ser dueña de la secuencia.
- **Una máquina de estados ad hoc por sistema.** Una para cartas, otra para dados, otra para
  acciones. Tres mecanismos de pausa que se comportan distinto, y el bug siempre está en la
  interacción entre dos.
- **Booleanos `bEsperandoAlJugador` sueltos.** Funciona con una pausa. Con una pausa adentro de
  otra —un reroll de una tirada que pidió una carta— no hay booleano que alcance.

**Nada de esto se arregla con `Delay` ni con nodos latentes.** Un nodo latente espera *tiempo*;
acá hay que esperar *una respuesta*, que puede no llegar nunca porque el jugador se fue a hacer
un café.

## La forma: una pila de pasos

Un solo mecanismo, en C++, que las tres cosas usan:

```
UResolutionSubsystem  (UWorldSubsystem)
  └── pila de FResolutionStep
        └── un paso puede: mutar estado, empujar más pasos, o pedir una decisión
```

**`UEffectSubsystem` ya era esto**, pero solo para cartas —"resuelve un efecto paso a paso",
según [`04-mapa-de-clases.md`](04-mapa-de-clases.md). Lo que cambia es que deja de ejecutar por
su cuenta y pasa a ser un **productor de pasos** que empuja a la pila común. Lo mismo el
resolvedor de dados y el de acciones. Un solo ejecutor, tres productores.

### Pila, no cola — y por qué importa

Una carta de presión se resuelve **de arriba hacia abajo**: pasos A, B, C. Pero si A es "spawnea
2 enemigos" y eso genera A1 y A2, esos dos tienen que correr **antes** que B, no al final.

Eso es recorrido en profundidad, y sale gratis con una pila si **cada paso empuja sus hijos en
orden inverso**. Con una cola, los hijos de A quedan detrás de C y la carta se resuelve en un
orden que no es el que dice el texto.

Es la clase de detalle que no se nota hasta que una carta hace algo raro tres semanas después.

### La pausa

Un paso no llama a la UI. **Publica que hay una decisión pendiente y la máquina se estaciona**:

```
FPendingChoice
  ├── qué tipo de decisión es      (elegir objetivo, repartir puntos, ordenar spawns, rerollear)
  ├── las opciones legales
  └── quién tiene que contestar
```

El `AMissionPlayerController` —que es el dueño de la UI— escucha, muestra lo que corresponda y
devuelve la respuesta. La máquina despierta y sigue.

**La máquina nunca conoce un widget.** Eso es la regla 2 aplicada al caso más tentador de
romperla, y es lo que permite contestar una decisión desde un test sin abrir la UI.

## Qué se gana gratis

Esto es lo que hace que valga la pena escribirlo antes y no después:

- **Guardar a mitad de tirada deja de ser "un caso feo"** —así está marcado hoy en
  [A-09](../gdd/06-decisiones/abiertas.md)—. Si la pila es serializable, el save funciona en
  cualquier momento, incluso con una decisión pendiente.
- **El log de partida es la pila ejecutada.** Un juego de tablero necesita "¿qué pasó?" para ser
  legible, y ese mismo log es la mejor herramienta de debug que vas a tener.
- **Los tests dejan de necesitar la UI.** Un test empuja pasos, contesta las decisiones con
  valores fijos y verifica el estado final. Con `URandomSubsystem` de semilla, es determinista.
- **El replay** sale de la misma estructura, si alguna vez hace falta.

## La trampa: undo + RNG con semilla

Un juego por turnos invita al undo, y acá tiene un filo. Si la pila guarda **la semilla**,
deshacer una tirada y rehacerla da un resultado distinto: eso es *save scumming* y desarma la
tensión entera del `Toll`, que es la regla que define el juego.

Si la pila guarda **el resultado**, deshacer y rehacer da lo mismo, y el undo solo sirve para
corregir un click.

**Recomendación: guardar el resultado.** Pero no hace falta decidir el undo ahora —hace falta
decidir *esto*, que es qué se serializa, porque cambia el formato del save. Si se guarda el
resultado, el undo queda disponible el día que se quiera; si se guarda la semilla, no.

Queda como decisión abierta a registrar en [`abiertas.md`](../gdd/06-decisiones/abiertas.md).

## Quién es dueño de qué

| Pieza | Dónde vive | Qué hace |
|---|---|---|
| `UResolutionSubsystem` | `UWorldSubsystem`, C++ | ejecuta la pila y publica las decisiones pendientes |
| `FResolutionStep` | `USTRUCT` | un paso serializable |
| `AMissionGameMode` | ya existe | **decide qué empujar**: robar carta, abrir la fase, cerrar el turno |
| `UEffectSubsystem` | `UWorldSubsystem` | traduce una carta a pasos |
| `AMissionPlayerController` | ya existe | contesta las decisiones pendientes |
| `AMissionGameState` | ya existe | el estado que los pasos mutan |

**La máquina no decide, ejecuta.** Es la misma división que ya vale entre `GameMode` y
`GameState`, un nivel más abajo: el `GameMode` sigue siendo el árbitro y la pila es su
herramienta. Meter reglas adentro de la máquina es el error simétrico al de meter estado en el
`GameMode` — ver [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md).

## Lo que **no** va acá

- **Animaciones y esperas de presentación.** La pila resuelve en lógica y no espera a nadie más
  que al jugador. El porqué y el cómo, en
  [`08-presentacion-y-reglas.md`](08-presentacion-y-reglas.md).
- **Las reglas.** Un paso aplica un efecto; qué efecto corresponde lo decidió el `GameMode` o la
  carta antes de empujarlo.

## Dependencias

- Consume: [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md),
  [`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md),
  [`../gdd/03-resolucion/turno.md`](../gdd/03-resolucion/turno.md),
  [`../gdd/03-resolucion/dados.md`](../gdd/03-resolucion/dados.md),
  [`../gdd/03-resolucion/vocabulario-de-efectos.md`](../gdd/03-resolucion/vocabulario-de-efectos.md)
- Alimenta: [`04-mapa-de-clases.md`](04-mapa-de-clases.md),
  [`08-presentacion-y-reglas.md`](08-presentacion-y-reglas.md)
- Toca del temario: clase 3 (framework y máquina de fases), clase 5 (comunicación),
  clase 12 (datos), y el harness de tests
