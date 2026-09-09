# 🎯 Concepto

← [Índice](../README.md)

## Qué es

Táctico por turnos, single player. Controlás **4 personajes** que bajan a un mapa a romper
un objetivo antes de que un **adversario** termine de manifestarse. Cuando el objetivo se
rompe, el adversario se vuelve vulnerable y empieza la pelea final.

El juego tiene un reloj que corre solo y no se puede detener. Todo lo que hacés lo acelera.

## La decisión central

**Tus personajes se vuelven más fuertes solo dañándose en una barra que nunca se cura.**

El poder y la muerte salen del mismo lugar. No hay una ruta segura que dé progresión: el
único camino para subir skills y ganar dados permanentes es gastar la barra que, al
llenarse, te saca el personaje del juego.

Eso invierte el instinto de casi cualquier otro juego. El jugador **quiere** recibir daño,
en una barra específica, y en la cantidad justa. El detalle está en
[`02-personaje/trinquete.md`](../02-personaje/trinquete.md), que es el documento más
importante de este GDD.

## Los cinco sistemas que cargan el peso

Si algo se recorta, no puede ser ninguno de estos. Son la estructura del juego.

**1. El trinquete.** Tres barras, dos se curan y una nunca. La que no se cura es la que da
poder. → [`02-personaje/trinquete.md`](../02-personaje/trinquete.md)

**2. El reloj de ritmo variable.** El adversario avanza cuando se acumulan símbolos en el
descarte, y ahí el mazo se remezcla. Nunca sabés si el próximo avance viene en 3 turnos o
en 6. → [`04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)

**3. El `Reckoning` de fin de turno.** Todo turno termina en premio o en paliza, según si
limpiaste tu espacio. Una sola regla que es un loop completo.
→ [`03-resolucion/turno.md`](../03-resolucion/turno.md)

**4. Enemigos pegajosos.** Al salir de un espacio, todos los enemigos que había te siguen.
No hay desengage, y eso convierte el movimiento en la decisión más cara del juego.
→ [`04-oposicion/enemigos.md`](../04-oposicion/enemigos.md)

**5. El adversario en dos fases que se fortalece muriendo.** No se lo puede tocar hasta
romper el objetivo, y sus stages acumulan dados: matarle uno lo hace pegar más fuerte.
→ [`04-oposicion/objetivo-y-adversario.md`](../04-oposicion/objetivo-y-adversario.md)

## Qué tipo de experiencia busca

**Desafío y maestría.** No es un juego de narrativa ni de expresión. Lo que el jugador
aprende partida a partida es a leer probabilidad, a administrar una pérdida que no se
puede revertir, y a decidir cuándo conviene empujar y cuándo retirarse.

Con una sola misión, lo que cambia entre partidas es **qué skills subiste y en qué orden**.
El trinquete no es solo la mecánica central: es también el motor de rejugabilidad.

## Qué lo hace distinto

Hay muchos tácticos por turnos y muchos juegos de dados. Lo raro acá es la combinación:

- **El recurso de progresión es también el reloj de muerte del personaje**, y no se puede
  devolver. La mayoría de los juegos con progresión te dejan volver atrás o farmear; acá
  cada punto que ganás es definitivo en las dos direcciones.
- **Los dados cobran siempre.** Una de las cuatro caras te cuesta trinquete aunque estés
  atacando y ganando. Actuar es intrínsecamente caro, y eso hace que "no hacer nada" sea
  una jugada real.
- **Controlás a los cuatro.** Eso habilita una estrategia que el juego de mesa suprime:
  quemar deliberadamente a un personaje para convertirlo en el más fuerte, y usarlo como el
  que entra a la carnicería. Ningún jugador de mesa se ofrece de voluntario para eso.

## Fuera de alcance

Nada de esto está en el juego, y no es un olvido:

- Multijugador y cooperativo. Es single player.
- Combate en tiempo real. Es por turnos.
- Narrativa ramificada, diálogo, elecciones morales.
- Progresión entre partidas que dé poder permanente. La dificultad se sube con reglas
  opcionales, no bajando el desafío con grind.
