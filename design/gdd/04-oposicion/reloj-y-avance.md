# ⏳ El reloj (`Doom Track`)

← [Índice](../README.md)

## La estructura

El `Doom Track` tiene **8 espacios**. El `Adversary` arranca a la izquierda y avanza hacia
la derecha.

Si el marcador llega al **octavo espacio, se pierde la partida**. Sin excepciones y sin
apelación.

## Cómo avanza

Al final de cada turno se cuentan los **`Doom Symbol`** en la pila de descarte del
`Pressure Deck`.

**Si hay 3 o más**, tres cosas pasan, en este orden:

1. El `Adversary` **avanza 1 espacio** en el track
2. Se resuelven los efectos de ese avance
3. **Todo el descarte se remezcla** de vuelta en el mazo

Ese tercer paso es el que hace interesante todo el sistema.

## Por qué el ritmo es impredecible

Como el mazo **se remezcla en cada avance**, el contador vuelve a cero y las cartas con
símbolo se redistribuyen al azar.

Entonces nunca sabés cuándo viene el próximo avance. Puede caer en 3 turnos si salen tres
cartas con símbolo seguidas, o en 6 o más si no salen. **La presión se autorregula y no se
puede planificar con precisión.**

Eso es distinto de un contador fijo. Un reloj que avanza cada N turnos se optimiza: el
jugador cuenta y calcula. Este no se puede contar — solo se puede leer la pila y estimar.

Y por eso **la pila de descarte tiene que estar visible**. En la mesa es información pública
gratis: ves crecer los símbolos y sabés que estás cerca. En pantalla, si no se construye esa
UI, el jugador no tiene forma de sentir la presión y el mecanismo entero se pierde.

## El problema de escala

La carta de presión se roba **una por turno de personaje**. Con 4 personajes, eso es **4
cartas por ronda**.

El juego original, para partidas de un solo jugador, recomienda controlar **2** personajes.
Con 4, el reloj corre **al doble de velocidad** que en esa configuración.

**Este es el primer número que hay que re-balancear**, y no es un ajuste menor: cambia el
largo de la partida, cuántos umbrales de `Ratchet` alcanza a cruzar cada personaje, y por
lo tanto cuánta build llega a construirse.

Las opciones obvias son alargar el track, subir el umbral de símbolos, o robar menos
seguido. Cuál se elige no está decidido.
→ [`06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)

## Qué pasa en cada avance

El avance no es solo mover un marcador. Dispara efectos de dos fuentes a la vez:

- **Los del `Adversary`**, escritos en sus Stage cards reveladas
- **Los de la misión**, escritos en la carta de escenario

Se resuelven **primero los del adversario**.

Y da igual si lo que avanza es la figura del adversario o el marcador que la reemplaza
después de manifestarse: los efectos se resuelven igual.

## Las dos mitades del track

El track tiene un punto de quiebre marcado.

**Antes:** avanza la figura del `Adversary`. Todavía no está en el mapa.

**Al llegar al primer espacio marcado:** el adversario **se manifiesta**. La figura pasa al
mapa y en su lugar en el track se pone un marcador de progreso.

**Después:** avanza el marcador. El adversary ya está en el mapa peleando.

De ahí en adelante el reloj sigue corriendo igual, con la diferencia de que ahora tenés al
jefe encima además del reloj.
→ [`objetivo-y-adversario.md`](objetivo-y-adversario.md)

## El reloj como diseño

Vale entender qué logra este sistema, porque condiciona todas las decisiones del jugador:

**Castiga la pasividad sin usar un timer.** No hay reloj real, pero cada turno que jugás
acerca el final. No podés esperar a estar listo.

**Hace que la velocidad sea un recurso.** Ir rápido y sucio te deja peor preparado para la
pelea final; ir lento y prolijo te deja sin tiempo. No hay una respuesta correcta fija.

**Y le pone un techo a la estrategia del sacrificio.** Quemar un personaje para volverlo
fuerte lleva turnos, y cada turno gastado en eso es reloj que corre.
→ [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md)
