# 📊 Barras y recursos

← [Índice](../README.md)

Cada `Character` tiene **tres barras** y **tres skills**. Las skills están en
[`skills.md`](skills.md); acá van las barras.

## Las tres barras

| Barra | Se cura | Qué hace | Al llegar al final |
|---|---|---|---|
| `Wounds` | Sí, con `Recover` | Es el daño físico | Muerte |
| `Reserve` | Sí, con `Recover` | Se gasta para rerollear dados | No podés rerollear más |
| `Ratchet` | **Nunca** | **Es la progresión** | El personaje se pierde |

La asimetría es el diseño. **Dos barras renovables y una permanente**, y la permanente es
justo la que da poder.

## `Wounds`

Daño directo. Sube cuando un enemigo saca `Hit` al atacarte, cuando un `Hazard` te quema, y
cuando una carta lo dice.

Al llegar al final, el personaje muere. Las consecuencias de eso están en
[`05-partida/victoria-y-derrota.md`](../05-partida/victoria-y-derrota.md), y son más graves
de lo que parece: si pasa antes de que el adversario se manifieste, **se pierde la partida
entera**.

Se pueden desviar heridas a un `Ally` si el personaje tiene uno. Sigue contando como que el
personaje recibió el daño para todo efecto de carta; lo que cambia es dónde se aplica.

## `Reserve`

La moneda de reroll. **Máximo 4.**

Se gasta de a 1 para rerollear 1 dado, las veces que quieras mientras te quede. Funciona en
**cualquier** tirada, incluidas las de los enemigos cuando te atacan — porque el jugador
tira esos dados también.

Algunas cartas de recompensa cobran `Reserve` como precio de reclamarlas.

Estar al máximo no tiene más consecuencia que no poder subir más, aunque hay efectos que
convierten ese exceso en `Wounds`.

`Reserve` es el recurso táctico del juego: es lo que te deja pelear contra la varianza en el
momento, a cambio de quedarte sin red para el resto del turno.

## `Ratchet`

**Solo sube. Nunca baja. `Recover` no la toca.**

Es la barra central del juego y tiene documento propio:
→ [`trinquete.md`](trinquete.md)

Acá alcanza con lo mínimo: sube 1 por cada cara `Toll` que salga en cualquier tirada, y por
efectos de cartas. Al cruzar ciertos umbrales el personaje **gana poder permanente**. Al
llegar al final, se pierde.

## La acción `Recover`

**Solo se puede hacer en un espacio `Clear`**, es decir sin enemigos.

Curás **3 puntos en total**, repartidos como quieras entre `Wounds` y `Reserve`. Son 3
puntos totales, no 3 por barra. Se puede usar más de una vez en el mismo turno, gastando
una acción cada vez.

**`Ratchet` no se cura con `Recover`, ni con nada.**

Esa restricción es lo que le da sentido a todo el mapa: llegar a un espacio limpio y
quedarse ahí es la única forma de sanar, pero los enemigos te siguen cuando te movés, así
que "limpio" es un estado que hay que construir, no encontrar.

## Por qué esta estructura funciona

Las tres barras cubren tres horizontes de decisión distintos:

- **`Reserve`** es la decisión del momento: ¿gasto ahora para arreglar esta tirada?
- **`Wounds`** es la decisión del turno: ¿me conviene ir a limpiar un espacio y curarme, o
  sigo empujando?
- **`Ratchet`** es la decisión de la partida: ¿cuánto poder me conviene comprar, sabiendo
  que lo pago con el personaje?

Ninguna de las tres se puede optimizar sin empeorar las otras dos.
