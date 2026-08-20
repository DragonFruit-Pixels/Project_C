# 🏁 Victoria y derrota

← [Índice](../README.md)

## Ganar

**Se gana matando el stage final del `Adversary`.** Es la única condición de victoria.

Si varios finales se disparan a la vez, **la victoria tiene precedencia**. Si el último
personaje y el adversario mueren en el mismo momento, ganaste.

## Perder

Se pierde con **cualquiera** de estas tres:

| # | Condición |
|---|---|
| 1 | Un personaje se pierde **antes** de que el `Adversary` se manifieste |
| 2 | **Todos** los personajes se pierden, después de que se manifestó |
| 3 | El marcador llega al **octavo** espacio del `Doom Track` |

"Perderse" es lo mismo por las dos vías: llegar al final de `Wounds` o al final de
`Ratchet`. Las dos barras matan.

## Las dos fases de riesgo

Las condiciones 1 y 2 no son la misma regla escrita dos veces. Son **dos juegos distintos
pegados**, separados por la manifestación del adversario.

### Antes: no puedo perder a nadie

Un solo personaje que caiga **termina la partida**. No hay margen, no hay pérdidas
aceptables, no hay sacrificio.

Durante toda esta fase el jugador tiene que jugar conservador con los cuatro, mientras el
reloj corre y el adversario —que ya puede estar en el mapa— pega sin poder ser respondido.

### Después: bajas aceptables

Manifestado el adversario, la partida sigue mientras **quede alguien vivo**. Perder un
personaje duele muchísimo, pero no termina nada.

Ahí se abre la puerta al [sacrificio deliberado](../02-personaje/trinquete.md): empujar a un
personaje al fondo de su `Ratchet` para convertirlo en el más fuerte del equipo.

## Por qué esto funciona tan bien

El arco de dificultad **no hubo que inventarlo**: sale de dos reglas que ya estaban.

El juego arranca frágil y termina brutal. Al principio sos cuatro personas débiles que no
pueden permitirse un error; al final sos un equipo con builds armadas peleando contra algo
que se fortalece mientras muere.

Y el jugador **controla parcialmente cuándo cruza esa línea**, porque romper el objetivo
adelanta la manifestación. Eso convierte a la mitad 1 en una pregunta estratégica real:

- Romper el objetivo **temprano** te habilita el sacrificio antes y te da más turnos de
  pelea, pero llegás con menos build construida.
- Romperlo **tarde** te deja más tiempo para prepararte, pero el reloj puede llevar al
  adversario al mapa igual, y ahí lo comés sin poder responder mientras seguís trabajando.

Ninguna de las dos es correcta siempre. Eso es exactamente lo que debería hacer una decisión
estratégica.

## Muerte de un personaje

Cuando un personaje se pierde, y la partida continúa:

- Se **descartan** todas sus cartas, tokens y componentes.
- La misión puede definir qué pasa con tokens especiales que estuviera cargando.
- Si murió **durante su propio turno**, se saltean todas las fases restantes **menos** el
  chequeo del descarte de presión. Ese se hace igual: si hay 3 `Doom Symbol`, el adversario
  avanza. Si hace falta saber dónde estaba el personaje para resolver algo, se usa el
  espacio donde murió.
- De ahí en más, **su turno se saltea por completo**.

Ese último punto tiene una consecuencia de ritmo que conviene tener en cuenta: **menos
personajes vivos significa menos cartas de presión por ronda**, o sea que el reloj se
desacelera. Perder gente te hace más débil pero te da más tiempo.

Es un balanceo automático que el diseño original tiene y que conviene no romper al
re-balancear el reloj para 4 personajes.
→ [`../04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)
