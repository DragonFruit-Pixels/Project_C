# 📐 Alcance

← [Índice](../README.md)

## La decisión

**1 misión y 1 adversario.**

Eso no es un recorte: es **la unidad atómica del juego original**. El juego de mesa se arma
combinando una caja de escenario con una caja de jefe, una de cada una. Con una misión y un
adversario tenés una partida completa, ganable y perdible, no una porción de algo.

## Qué entra

| Sistema | Estado |
|---|---|
| Los cinco sistemas que cargan el peso | Completos, ninguno se recorta |
| Mapa | 1 layout, con superficie y subterráneo |
| Personajes | 4, controlados todos por el jugador |
| Adversario | 1, con sus 4 stages |
| Objetivo | 1, más las 2 acciones propias de la misión |
| Skills | El pool de 6, con **3 niveles** cada uno = 18 celdas. Cada personaje lleva **2** (el manual da 3 skills de 4 niveles) → [A-08](../06-decisiones/abiertas.md) |

El inventario contado de contenido está en
[`05-partida/contenido-de-una-mision.md`](../05-partida/contenido-de-una-mision.md).

## Qué cambia respecto del original

**Se pierde la rejugabilidad por combinación.** El juego de mesa rejuega porque son 6
escenarios × 2 jefes = 12 configuraciones distintas. Acá hay una sola.

Lo que queda para que valga volver a jugar:

- El orden de los mazos, que cambia en cada partida
- Qué personajes elegís
- Qué carta de trinquete le toca a cada uno
- **Qué skills subís y en qué orden** ← es la que pesa
- Las reglas opcionales de dificultad

De esa lista, la única que produce partidas realmente distintas es la de los skills. Por eso
[el trinquete](../02-personaje/trinquete.md) carga con doble función: es la tensión del
juego y es el motor de rejugabilidad.

## La consecuencia de forma

Con una sola misión, el juego no es una campaña. Es **un escenario que se repite, en el que
mejorás y después subís la dificultad**. Esa es la silueta de un roguelike de escenario
único, más cerca de una isla de *Into the Breach* que de una campaña.

No hace falta declararlo como género todavía, pero conviene tenerlo presente al diseñar: si
la misión solo se juega una vez, la mitad del valor del diseño se desperdicia.

## El adversario carga la identidad

Con uno solo, **no hay contraste**. En el juego original los dos jefes son deliberadamente
distintos de textura: uno es un depredador físico, el otro es una anomalía. Elegir uno solo
significa que ese define el tono, el tipo de presión y el recurso especial del juego entero.

Cuál se elige sigue abierto → [`06-decisiones/abiertas.md`](../06-decisiones/abiertas.md).

## Alcance vs. la materia

Este proyecto también tiene que cumplir el temario de una cursada, con entregas en fechas
fijas. **El alcance del juego excede el de la materia**: la cursada termina en diciembre y
el juego es multi-año.

La materia es un hito adentro del juego, no el juego. El plan de dos fases atado a las
fechas de los parciales está en [`course-alignment.md`](../../course-alignment.md), junto
con la matriz que verifica que ningún tema del temario quede sin lugar.
