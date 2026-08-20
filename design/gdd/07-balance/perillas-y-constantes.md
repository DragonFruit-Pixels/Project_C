# 🎛️ Perillas y constantes

← [Índice](../README.md)

**Este archivo es el dueño único de todos los números del juego.** Si un documento de sistema
dice un default o un rango distinto al de acá, **gana este** y hay que corregir el otro.

Existe porque el problema apareció de verdad: los mismos valores se habían escrito en varios
archivos con rangos que no coincidían. La regla del README —"los números viven en un solo
lugar"— necesitaba un lugar.

---

## Constantes del manual

Salen de la transcripción y **no son perillas**: cambiarlas es cambiar el juego, no
balancearlo. El default es el valor original.

| Constante | Valor | Fuente |
|---|---|---|
| Acciones por turno | 3 | pág. 13 |
| Espacios por `Move` | 3 | pág. 13 |
| Dados estándar por tirada | 3 | pág. 11 |
| Puntos de `Recover` (repartidos entre `Wounds` y `Reserve`) | 3 | pág. 14 |
| Máximo de `Reserve` | 4 | pág. 8 |
| `Doom Symbol` necesarios para avanzar | 3 | pág. 18 |
| Espacios del `Doom Track` | 8 | pág. 20 |
| Umbrales que dan dado bonus permanente | 4 | pág. 8 |
| Stages del `Adversary` | 4 | pág. 19 |
| Skills comunes × niveles | 6 × 4 | pág. 22, 24-25 |
| Cartas de presión | 16 (8 + 8) | pág. 10 |
| Cartas de recompensa | 15 | pág. 6 |
| Cartas de trinquete | 8 | pág. 10 |
| `Gate` por mapa | 3 | pág. 9 |
| Pool máximo de `Servant` | 10 | pág. 20 |
| Espacios por sala | 1 a 3 | pág. 9 |

---

## Perillas de balanceo

Un solo default y un solo rango seguro por perilla. La columna **Dueño** dice qué documento
explica el efecto en detalle.

| Perilla | Default | Rango seguro | Qué afecta | Dueño |
|---|---|---|---|---|
| Acciones por turno | 3 | **2–4** | A 2 el juego se vuelve asfixiante y `Swiftness` domina; a 4 se puede atacar y escapar en el mismo turno | [turno](../03-resolucion/turno.md) |
| Espacios por `Move` | 3 | **2–4** | A 4 el kiting se vuelve viable y los enemigos pegajosos dejan de ser una amenaza | [mapa](../01-fundamentos/mapa-y-espacios.md) |
| Puntos de `Recover` | 3 | **2–4** | Cuánto castiga entrar a un espacio con enemigos | [barras](../02-personaje/barras-y-recursos.md) |
| Máximo de `Reserve` | 4 | **3–6** | Cuánta varianza puede corregir el jugador por turno | [barras](../02-personaje/barras-y-recursos.md) |
| `Doom Symbol` para avanzar (`T`) | 3 | **3–6** | La velocidad del reloj. **Es la perilla de compensación por jugar con 4 personajes**, y el valor propuesto para eso es 6 | [reloj](../04-oposicion/reloj-y-avance.md) |
| Espacios del `Doom Track` | 8 | **8–15** | El largo total de la partida. Alternativa a subir `T` | [reloj](../04-oposicion/reloj-y-avance.md) |
| Cartas con `Doom Symbol` (`q` de 16) | sin dato | **8–14 de 16** | La varianza del ritmo del reloj: menos cartas con símbolo = avances más impredecibles | [reloj](../04-oposicion/reloj-y-avance.md) |
| Umbrales que dan dado bonus | 4 | **2–5** | Cuánto poder compra el trinquete | [trinquete](../02-personaje/trinquete.md) |
| Largo del track de `Wounds` | sin dato | **5–8** | Cuántos golpes aguanta un personaje | [barras](../02-personaje/barras-y-recursos.md) |
| Largo del track de `Ratchet` | sin dato | **8–12** | Cuántos umbrales entran, y por lo tanto cuánta build se construye | [trinquete](../02-personaje/trinquete.md) |
| Umbrales totales del track | sin dato | **4–6** | Las decisiones de build por personaje | [trinquete](../02-personaje/trinquete.md) |
| Pool de `Servant` | 10 | **6–14** | El techo de presión simultánea en el mapa | [enemigos](../04-oposicion/enemigos.md) |
| Espacios totales del mapa | sin dato | **12–32** | Cuánto cuesta cruzar el mapa, y por lo tanto el valor de `Swiftness` | [mapa](../01-fundamentos/mapa-y-espacios.md) |

> **`T` y los espacios del `Doom Track` no se mueven juntas.** Las dos alargan la partida por
> el mismo lado; tocar las dos a la vez duplica el efecto sin que se note de dónde vino.

---

## La distribución de caras del dado

**Esto es lo más importante de este archivo, y es un problema abierto.**

El manual **no dice cuántas caras del dado muestran cada resultado**. Lista los cuatro
resultados posibles y avisa que algunas caras traen dos a la vez, pero nunca da el reparto.
Esos valores están impresos en los dados, no en el reglamento.

**Consecuencia:** ninguna probabilidad, ningún daño esperado y ningún cálculo de ritmo del
juego se puede fundamentar hasta tener este dato. Todas las fórmulas del GDD que usan
probabilidad están construidas sobre la distribución provisional de abajo.

### Distribución provisional declarada

| Cara | Cantidad | Resultado |
|---|---|---|
| `Hit` | 2 | 1 éxito |
| `Hit` + `Toll` | 1 | 1 éxito y +1 `Ratchet` |
| `Toll` | 1 | +1 `Ratchet` |
| `Wild` | 1 | inerte sin skill |
| `Blank` | 1 | nada |

De ahí:

```
p(Hit)  = 3/6 = 0,50
p(Toll) = 2/6 ≈ 0,33
p(Wild) = 1/6 ≈ 0,17
```

**Todos los documentos del GDD usan estos valores.** Si un ejemplo numérico usa otra
distribución, es un error y hay que corregirlo — no una variante legítima.

Del dado bonus el manual sí confirma una cosa: **no tiene cara `Toll`**. El reparto entre
`Hit`, `Wild` y `Blank` también es provisional.

---

## Valores que el manual no contiene

Todos estos aparecen en las fórmulas del GDD como propuesta o como parámetro sin valor. La
razón es la misma en casi todos los casos: **el manual es un reglamento, no una especificación
de componentes.** Los largos de track están impresos en los tableros de personaje, las caras
en los dados, y las vidas y dados de ataque en las fichas de enemigo.

| Qué falta | Dónde estaría en el original | Impacto |
|---|---|---|
| Distribución de caras del dado | Impreso en los dados | **Alto.** Bloquea toda probabilidad |
| Largo del track de `Ratchet` y cantidad de umbrales | Tablero de personaje | **Alto.** Define la progresión |
| Largo del track de `Wounds` | Tablero de personaje | Medio |
| Vida y dados de ataque por tipo de enemigo | Fichas de enemigo | Medio. Solo se ven ejemplos suel­tos: `Servant` con 2 de vida, una `Creature` con 3, una ficha de 1 estándar + 2 bonus |
| Cuántas de las 16 cartas de presión traen `Doom Symbol` | Las cartas | Medio. Define la varianza del reloj |
| Vida y dados por stage del `Adversary` | Stage cards | Medio |
| Capacidad de heridas de cada `Ally` y precio de cada recompensa | Las cartas | Bajo, es contenido propio |

Para los de impacto alto hay dos caminos: leerlos del arte de los componentes, o **decidirlos
nosotros y tratarlos como diseño propio**. La segunda es legítima —el juego es una adaptación,
no un port— pero hay que hacerla a propósito y anotarla en el registro de decisiones.
