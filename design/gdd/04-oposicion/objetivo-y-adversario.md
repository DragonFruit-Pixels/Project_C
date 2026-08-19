# Objetivo y adversario

← [Índice](../README.md)

La partida tiene **dos mitades**, y la bisagra entre ellas es el objetivo.

---

## Mitad 1 — El objetivo

**El `Adversary` no puede ser atacado ni dañado.** Ni siquiera si ya está en el mapa.

Pero él **sí te puede atacar**. Esa asimetría es el punto: durante toda la primera mitad hay
una amenaza en el tablero que lastima y no se puede responder.

Lo único que se puede hacer es completar el **`Objective`**: la tarea que define la misión.
Cada escenario define la suya, junto con **2 acciones propias** que existen para ayudar a
completarla.

Al romper el objetivo, el adversario **se vuelve mortal** y se manifiesta al final del turno.

## La manifestación

El `Adversary` entra al mapa cuando pasa **cualquiera** de estas dos cosas:

- El reloj lo lleva al primer espacio marcado del `Doom Track`
- **O** rompés el objetivo

Lo que llegue primero.

Y ahí está la tensión del diseño: **el jugador no controla del todo cuándo empieza la pelea
final.** Si el reloj llega antes, el adversario aparece en el mapa mientras vos seguís sin
poder tocarlo. Te caza mientras trabajás.

Al manifestarse: la primera Stage card se corre al costado y se revela la segunda, con sus
efectos de revelación, que dicen dónde aparece la figura. **Los efectos de la Stage 1 quedan
en juego hasta el final de la partida.**

---

## Mitad 2 — La pelea

Una vez manifestado y con el objetivo roto, el `Adversary` **actúa como cualquier enemigo**:
ataca a los personajes que terminan su turno en su espacio.

Los dados que tira son **la suma de todos los dados de todas sus Stage cards reveladas**.

## Los 4 stages

El adversario tiene **4 stages**. Cada uno tiene:

- **Vida propia**
- Un **efecto de revelación**, que se dispara una sola vez al descubrirse
- Un **efecto permanente**, que queda hasta el final de la partida
- **Dados** que suma a sus ataques

Se matan **de a uno**. Cuando un stage acumula heridas iguales o mayores a su vida, se corre
al costado, se revela el siguiente y se resuelve su efecto de revelación.

**El daño sobrante no pasa al siguiente stage.** Si necesitabas 12 y hiciste 14, los 2 extra
se pierden.

## La regla que da vuelta la pelea

**Los efectos y los dados de los stages anteriores quedan todos en juego.**

O sea que **matarle un stage lo hace pegar más fuerte**. La pelea final no es una barra de
vida que baja: es un enemigo que se vuelve peor a medida que le ganás.

Eso invierte la curva de tensión de un boss fight normal. En la mayoría de los juegos, el
jefe se debilita mientras pelea. Acá cada victoria parcial es también un empeoramiento de tu
situación, y el jugador tiene que decidir si está en condiciones de aguantar lo que viene
antes de empujar el stage actual.

Combinado con el reloj —que sigue corriendo durante toda la pelea— la pregunta de cada turno
es si te alcanza el tiempo para curarte antes de abrir el siguiente stage.

## Cada revelación es un momento

Las 4 revelaciones son los picos dramáticos de la partida y deberían tratarse como tales:
cámara, cambio de iluminación, y el adversario reposicionándose en el mapa.

Es también el lugar natural para la destrucción del entorno: el adversario rompiendo una
pared al cambiar de stage **agrega una arista al grafo** y cambia el mapa para el resto de
la partida.
→ [`../01-fundamentos/mapa-y-espacios.md`](../01-fundamentos/mapa-y-espacios.md)

## Ganar

**Se gana matando el stage final.** No hay otra condición de victoria.
→ [`../05-partida/victoria-y-derrota.md`](../05-partida/victoria-y-derrota.md)

---

## Nota de alcance

Con **un solo adversario**, este documento describe la identidad completa del juego. No hay
contraste ni variedad de jefe: el que se elija define el tono, el tipo de presión y el
recurso especial de todo el proyecto.

Cuál se elige sigue abierto. → [`../06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)
