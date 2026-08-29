# 🎬 Presentación y reglas

← [Índice](README.md)

Una regla del juego nunca espera a una animación. **Las reglas resuelven instantáneamente y la
presentación reproduce después, a su propio ritmo.** Es una sola frase y decide media
arquitectura: sin ella, el tiempo de una animación se vuelve parte de las reglas, y eso no se
saca sin reescribir los sistemas que ya funcionan.

Es la contracara de [`07-resolucion-y-pausa.md`](07-resolucion-y-pausa.md): ese documento dice
cómo se resuelve, este dice cómo se ve.

---

## Qué pasa si se mezclan

El síntoma es inocente: "cuando el enemigo te pega, esperá a que termine el golpe para aplicar
la herida". Suena a orden natural. Lo que compra:

- **No se puede testear.** Un test de la regla de ataque tarda lo que tarda la animación, y falla
  distinto según la máquina. El harness deja de servir.
- **No se puede acelerar ni saltear.** Un juego por turnos con 4 personajes y una fase de
  enemigos por ronda **necesita** un botón de velocidad. Si el tiempo vive en las reglas, ese
  botón no existe.
- **El save queda indefinido a mitad de animación.** ¿Se guarda la herida aplicada o no? No hay
  respuesta buena, porque el estado del juego depende de un frame.
- **El orden se vuelve frágil.** "Todos los enemigos que estaban ahí te siguen"
  ([`enemigos.md`](../gdd/04-oposicion/enemigos.md)) son N movimientos. Si cada uno espera al
  anterior, la regla —que es instantánea y simultánea— pasa a tener una duración que nadie
  diseñó.

## La forma: la pila ejecutada es el guion

Con la máquina de [`07`](07-resolucion-y-pausa.md), esto sale casi solo. La resolución produce
una lista de pasos ya ejecutados; **esa lista es el guion de la presentación**:

```
reglas   →  [Hit, Hit, Toll, muere Servant #3, sube Doom]   →  presentación
(instantáneo)          la misma lista que el log                (tarda lo que quiera)
```

El log de partida y el feed de la presentación **son la misma cosa**. No son dos sistemas: es
uno, consumido por dos lectores.

Y la regla de "te siguen" se vuelve trivial de presentar bien: la lógica movió los 5 enemigos de
una, y la presentación los escalona 80 ms cada uno porque queda mejor. Cambiar ese escalonado no
toca ninguna regla.

## La distinción que importa: esperar al jugador **sí**, esperar a una animación **no**

Son dos esperas que parecen iguales y no lo son.

| | Esperar al jugador | Esperar a una animación |
|---|---|---|
| Qué se espera | una **respuesta** que cambia el resultado | que pase el tiempo |
| Puede no llegar nunca | sí | no |
| Afecta el estado del juego | sí, es una decisión | no, ya está decidido |
| Dónde vive | la pila se estaciona ([`07`](07-resolucion-y-pausa.md)) | la capa de presentación, sola |

La máquina de resolución **solo** conoce la primera. La segunda no la conoce nadie más que quien
dibuja.

## Cómo esto resuelve A-09 sin discutirla

[A-09](../gdd/06-decisiones/abiertas.md) pregunta quién es la autoridad del dado: la física o la
lógica. Con la regla general puesta, la pregunta cambia de forma.

Una tirada física no es una animación: **es una fuente que contesta un valor.** O sea, es el
mismo caso que "esperar al jugador" —la máquina se estaciona esperando una respuesta— con otro
respondedor:

```
la pila necesita un valor de dado
  ├── si hay dados físicos:  los tira, detecta "asentado", contesta la cara
  ├── si no se asienta en N segundos: contesta URandomSubsystem
  └── si el modo rápido está activo: contesta URandomSubsystem directo
```

Eso preserva las tres cosas a la vez: la física entra al proyecto como demostración real (clase
3 del temario), el resultado nunca se cuelga, y **la física jamás es dueña del estado del
juego** — solo contesta una pregunta. Es exactamente el híbrido que ya recomienda
[`04-mapa-de-clases.md`](04-mapa-de-clases.md), pero deja de ser una decisión sobre dados y pasa
a ser una consecuencia de la regla general.

Un `UDiceResolver` en C++ sigue siendo el único que **declara** el resultado. Quién se lo sopló
—la física o el RNG— es indistinto para todo lo que viene después.

> Ojo con el reroll: cada reroll es otra pregunta a la misma fuente. Con física, es otra tirada
> real; con RNG, es un valor nuevo. La máquina no cambia.

## Las tres trampas concretas

- **`Delay` adentro de un grafo de reglas.** Si aparece un `Delay` en el `GameMode` o en un
  efecto de carta, la regla acaba de adquirir una duración. Es la señal más clara de que algo se
  mezcló.
- **`Tick` preguntando "¿ya terminó?".** [`01-por-donde-se-empieza.md`](01-por-donde-se-empieza.md)
  ya lo marca: un juego por turnos casi no necesita `Event Tick`, y los `Tick` que aparecen son
  casi siempre esta espera disfrazada. La presentación puede usar `Tick` todo lo que quiera; las
  reglas no.
- **Widgets leyendo el estado en vez de consumir el feed.** Si `WBP_DoomTrack` lee
  `DoomTrackPosition` cada frame, el reloj salta al valor final antes de que la animación cuente
  la historia. Los widgets escuchan pasos, no polean estado.

## Quién es dueño de qué

| Capa | Quién | Puede |
|---|---|---|
| Reglas | `AMissionGameMode`, `UResolutionSubsystem`, `UEffectSubsystem` | mutar `AMissionGameState`. **Nunca** referencia un widget ni un mesh |
| Estado | `AMissionGameState` | guardar |
| Presentación | `AMissionPlayerController` y sus widgets, componentes visuales de los actores | leer el feed, animar, sonar, esperar |

Es la regla 2 del [índice](README.md) con una dirección explícita: **la presentación conoce a
las reglas, las reglas no conocen a la presentación.** Si un `#include` o un `Cast` va en el
sentido contrario, está mal.

## Lo que esto habilita

- **Botón de velocidad y skip**, gratis: es un multiplicador en la capa que reproduce.
- **Tests a tiempo cero.** Con el consumidor de presentación apagado, una partida entera corre en
  milisegundos. Es lo que hace posible testear balance por simulación.
- **Cambiar el arte sin tocar reglas.** Rehacer cómo se ve una tirada no puede romper el juego,
  porque la tirada ya estaba resuelta.

## Dependencias

- Consume: [`07-resolucion-y-pausa.md`](07-resolucion-y-pausa.md),
  [`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md),
  [`../gdd/03-resolucion/dados.md`](../gdd/03-resolucion/dados.md)
- Alimenta: [`04-mapa-de-clases.md`](04-mapa-de-clases.md) (cierra A-09),
  [`05-temario-como-orden-de-construccion.md`](05-temario-como-orden-de-construccion.md)
- Toca del temario: clase 3 (físicas y colisiones), clase 7 (UMG), clase 13 (profiling)
