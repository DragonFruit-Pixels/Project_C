# El trinquete (`Ratchet`)

← [Índice](../README.md)

> **Este es el documento más importante del GDD.** Si algo del juego se rediseña, esta
> mecánica es la que no se toca: es lo que lo hace distinto de cualquier otro táctico por
> turnos.

## La regla

`Ratchet` **solo sube**. No hay forma de bajarla. Descansar no la toca, curarse no la toca,
ningún objeto la toca.

Se llama trinquete porque es exactamente eso: un mecanismo que solo gira para un lado.

## Cómo sube

- **1 por cada cara `Toll`** que salga en cualquier tirada. Cualquiera: atacando, siendo
  atacado, o tirando por un efecto. Ver [`03-resolucion/dados.md`](../03-resolucion/dados.md).
- Por efectos de cartas de presión y de recompensa que lo digan.
- Por cada `Toll` al resolver `Hazard` a fin de turno.

## Los umbrales

El track tiene **umbrales** marcados. Cada vez que el marcador cruza uno, pasan cuatro
cosas, en este orden:

**1. Se ignora el exceso.** Si ibas a perder 3 puntos y el umbral estaba a 1, perdés 1 y
los otros 2 se descartan. El marcador **para justo en el umbral**.

**2. Se dispara la `Ratchet Card` del personaje.** Cada personaje recibe una al empezar la
partida, tomada de un mazo de 8. Es un efecto propio que se ejecuta cada vez que cruza un
umbral, así que un personaje con muchos umbrales cruzados la dispara muchas veces.

**3. Sube 1 nivel de un skill, a elección del jugador.** Esta es la progresión.

**4. En 4 de los umbrales, gana +1 dado bonus permanente.** Se suma a **todas** sus tiradas
por el resto de la partida, con una excepción: no aplica a las tiradas contra sí mismo.

Al llegar al final del track, el personaje **se pierde y sale del juego**.

## Por qué esto es el juego

**El jugador quiere recibir daño.** En esta barra, y solo en esta.

Es una inversión que casi ningún juego hace. En la mayoría, el daño es puro costo y el
jugador aprende a evitarlo. Acá el daño en `Ratchet` es el **único** camino a subir skills
y ganar dados. Un personaje que juega perfecto y no cruza ningún umbral es un personaje
débil que va a llegar al final de la partida sin herramientas.

Y al mismo tiempo, cada punto lo acerca a perderse. La misma barra es la fuente de poder y
el reloj de muerte.

Eso convierte cada tirada en una pregunta doble: no es solo "¿me sale bien?", es también
"¿cuánto quiero que me cueste?".

## La estrategia que este juego desbloquea

Los 4 personajes son tuyos. Cada uno tiene su propio `Ratchet` independiente.

Entonces podés **quemar a uno a propósito**: llevarlo deliberadamente hacia el fondo del
track para acumularle niveles de skill y dados bonus, y usarlo como el que entra a los
espacios llenos de enemigos mientras los otros tres se mantienen limpios y seguros.

**En el juego de mesa esa estrategia existe pero nadie la juega**, porque cada jugador
controla su propio personaje y ninguno se ofrece de voluntario para ser el sacrificado. Es
un problema social, no de diseño.

Vos no tenés ese problema. **La adaptación a single player no pierde esa estrategia: la
habilita.**

## El candado que la hace interesante

Si el sacrificio fuera gratis, sería la jugada obvia y el juego se rompería. No lo es, por
una regla que ya está en el diseño:

**Si un personaje se pierde antes de que el adversario se manifieste, se pierde la partida
entera.** Después de que se manifiesta, los demás siguen jugando.

Entonces el sacrificio tiene un **candado temporal**: no podés empujar a nadie al fondo del
track hasta que el adversario esté en el tablero. Y como el momento en que aparece depende
del reloj y de vos, el juego te obliga a calcular cuándo se abre esa ventana.

Eso genera dos fases de riesgo completamente distintas dentro de la misma partida, y no hubo
que inventarlas: salen de las reglas que ya estaban.
→ [`05-partida/victoria-y-derrota.md`](../05-partida/victoria-y-derrota.md)

## El motor de rejugabilidad

Con [una sola misión](../00-vision/alcance.md), lo que cambia entre partidas es **qué skills
subiste y en qué orden**. Cada umbral es una decisión de build, y las decisiones tempranas
condicionan a las tardías porque algunos niveles de skill reemplazan al anterior en vez de
sumarse.

Con 4 personajes y 4-6 umbrales cada uno, una partida son entre 16 y 24 decisiones de build.
Eso es lo que hace que la segunda partida no sea igual a la primera.

## Riesgo de diseño conocido

**El trinquete no se elige: te pasa.** Sube por resultados de dados, no por una decisión.
En la mesa eso es drama y funciona; en pantalla puede leerse como arbitrario.

El upgrade más grande que el medio digital habilita es **convertirlo en palanca**: hoy
gastás `Reserve` para rerollear, y se podría permitir gastar `Ratchet` también. Ahí lo que
te pasaba pasa a ser algo que decidís pagar.

No está decidido. → [`06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)
