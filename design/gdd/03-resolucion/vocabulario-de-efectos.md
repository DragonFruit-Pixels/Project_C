# 🧩 Vocabulario de efectos

← [Índice](../README.md)

Las cartas del juego —presión, recompensa y trinquete— **no son efectos arbitrarios**. Son
combinaciones de un vocabulario chico y cerrado. Este documento lo cataloga, porque es lo que
después se convierte en Data Tables y Gameplay Tags, y es lo que hace que la misión 2 sea
contenido en vez de código.

**Está derivado de evidencia, no inventado.** Las piezas de abajo salen de cartas reales del
juego original, recuperadas de la wiki y de fuentes de la comunidad. Ver [Procedencia](#procedencia).

---

## El hallazgo que cambia el diseño

Las cartas de trinquete **no son puro castigo**. Cada una es un **condicional de dos ramas**:

```
si <se cumple una condición de estado>  ->  costo
si no                                   ->  recompensa
```

Cuatro ejemplos reales, con la mecánica traducida a nuestro vocabulario:

| Carta original | Qué lee | Rama de costo | Rama de recompensa |
|---|---|---|---|
| *Claustrophobia* | **Cuántos espacios adyacentes** tiene tu espacio | ≤ 2 adyacentes → +2 `Reserve` | si no → cura 1 `Reserve` **por espacio adyacente** |
| *Codependency* | La posición de **otro personaje** designado al inicio | está vivo y no comparte tu espacio → +2 `Reserve`, y después uno de los dos se mueve al espacio del otro **sin que los enemigos sigan** | comparte tu espacio → nada |
| *Hypochondria* | **Cuántos objetos y `Ally`** tenés, contra un contador acumulado **en la propia carta** | menos objetos que tokens → `Reserve` igual a la diferencia | si no → cura **todo** el `Reserve` |
| *Recurring Trauma* | El **descarte de presión**, con una carta guardada al lado | hay carta guardada → se resuelve como si la acabaras de robar | no hay → cura **todo** el `Reserve` |

**Por qué importa.** Yo tenía descrita la carta de trinquete como "un efecto propio del
personaje". Es mucho más que eso: es un **incentivo posicional y de estado que se dispara 6
veces por partida**, y que el jugador puede jugar a favor.

Consecuencias directas:

- El jugador no solo administra *cuándo* cruza un umbral: administra **en qué estado está** cuando
  lo cruza. Cruzarlo parado en el lugar correcto puede ser un beneficio neto.
- Eso le da al trinquete una capa de decisión que no venía de los dados, y **responde en parte a
  la crítica de que "el trinquete te pasa, no lo elegís"** ([A-05](../06-decisiones/abiertas.md)).
- Y hace que las 8 cartas sean **contenido de diseño de primer orden**, no relleno temático.

## El otro hallazgo: la topología del mapa es input de juego

*Claustrophobia* lee **cuántos espacios son adyacentes al tuyo**. O sea que el **grado de los
nodos del grafo** es un valor que las reglas consultan, no solo una propiedad del layout.

Consecuencia para el mapa: **no puede ser una retícula uniforme.** Si todos los espacios tienen
el mismo grado, ese tipo de carta no distingue nada y el efecto se vuelve constante. El mapa
necesita variedad deliberada de grado: pasillos de grado 1-2, cruces de grado 4-5.

Eso convierte una perilla que había puesto como estimación —el grado promedio del grafo— en un
parámetro con consecuencia mecánica real.
→ [`../07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md)

---

## El vocabulario, catalogado

### Selectores — qué apunta el efecto

| Selector | Visto en |
|---|---|
| El personaje activo | todas |
| Otro personaje, designado al inicio de la partida | *Codependency* |
| Los espacios adyacentes al tuyo, y su cantidad | *Claustrophobia* |
| Tu espacio, y qué hay en él | referéndum, `Recover` |
| Tu inventario, y su cantidad | *Hypochondria* |
| El enemigo más cercano de un tipo, por camino más corto | cartas de presión |
| Todos los enemigos de un tipo | cartas de presión |
| Una `Gate` por color | cartas de presión |
| El descarte de presión | *Recurring Trauma* |
| Tokens acumulados **en la propia carta** | *Hypochondria* |

### Condiciones — cuándo se dispara cada rama

Comparación de cantidad (`≤ 2 adyacentes`, `objetos < tokens`), presencia o ausencia
(`hay carta guardada`), estado de otra figura (`está vivo`), y posición relativa
(`no comparte tu espacio`).

### Acciones — qué hace

| Acción | Nota |
|---|---|
| Sumar o curar `Reserve` | incluido **"curá todo el `Reserve`"**, que es una recompensa grande |
| Sumar o curar `Wounds` | |
| Sumar `Ratchet` | |
| Mover una figura N espacios hacia un objetivo | por camino más corto, para al llegar |
| Mover un personaje a otro espacio | *Codependency* usa la excepción de **"colocar"**: los enemigos **no** siguen |
| Invocar un enemigo en una `Gate` | se saltea si no hay figura disponible |
| Colocar un token sobre una carta | estado persistente **entre activaciones** |
| Resolver otra carta como si se acabara de robar | *Recurring Trauma* |
| Forzar una tirada | con umbral de `Hit` a superar, o efecto por cada `Hit` |
| Subir un nivel de skill | |
| Reclamar o descartar una carta | |
| Avanzar el `Doom Track` | |

### Semántica de ejecución

Dos reglas que valen para **todo** el vocabulario, y que se implementan una sola vez:

1. **Un paso que no se puede ejecutar se saltea, no falla.** La carta sigue resolviéndose.
2. **Estado persistente en la carta.** Una carta puede acumular tokens entre activaciones, así
   que el efecto crece con la partida. *Hypochondria* se vuelve peor cada vez que se dispara.

---

## Qué falta y por qué no se cataloga todavía

El catálogo de arriba sale de **4 cartas de trinquete** de las 8 que necesita una misión, y de la
estructura de 3 partes de las cartas de presión. Falta:

- Las cartas de presión reales: solo se conoce su **estructura**, no un set de efectos concretos
- Las cartas de recompensa: objetos, `Ally` y `Burden`, con dos lados cada una
- Los efectos de revelación y permanentes de los 4 stages del adversario

**No hay que buscarlos.** Ese es el contenido que este proyecto diseña, y el vocabulario de arriba
ya alcanza para empezar. La regla sigue siendo la de
[D-09](../06-decisiones/registro.md): se implementan las primeras 5 o 6 cartas a mano, se mira qué
se repite, y el vocabulario definitivo se extrae de esa evidencia — no de esta lista, que es un
punto de partida.

## Procedencia

- [Insanities — Cthulhu Death May Die Wiki (Fandom)](https://cthulhu-death-may-die.fandom.com/wiki/Insanaties) — el texto de las cartas de trinquete
- [Insanity Card: Recurring Trauma — BoardGameGeek](https://boardgamegeek.com/thread/2321338/insanity-card-recurring-trauma) — la interacción con el descarte de presión
- La estructura de 3 partes de las cartas de presión y la semántica de "se saltea" salen del
  manual transcripto, `docs/rulebook/rulebook.md` pág. 15

Los textos de carta son de **fuentes de terceros**. Se usan como evidencia de la *forma* del
vocabulario, no como contenido a copiar: las cartas de este juego se diseñan de cero.
