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
| Espacios del track de `Ratchet` | **20** | componente, ver abajo |
| Umbrales del track de `Ratchet` | **6**, en las casillas 4, 8, 12, 15, 18 y 19 | componente, ver abajo |
| Capacidad de `Wounds` | **4** — la 5.a es letal | componente, ver abajo |
| Capacidad de `Reserve` | **4** | pág. 8 + componente |

### Dónde esta versión se aparta del manual

La tabla de arriba es **transcripción**: dice lo que el juego original hace y no se toca. Esto
es lo que este proyecto implementa distinto, y es una decisión de alcance, no de balance.

| Constante | Manual | Esta versión | Por qué |
|---|---|---|---|
| Skills por personaje | 3 (1 de firma + 2 comunes) | **2** | Recorte de alcance para la cursada → [A-08](../06-decisiones/abiertas.md) |
| Niveles por skill | 4 | **3** | Se quita el nivel 4. Baja el contenido a construir de 24 celdas a 18 |
| Subidas de skill disponibles por partida | 9, contra 6 umbrales | **4**, contra 6 umbrales | **Consecuencia, no decisión.** Dos umbrales por partida pierden su subida, y la repartición legal pasa a ser única: `(3,3)`. La elección de build desaparece — ver F4 en [`skills.md`](../02-personaje/skills.md) |

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
| Capacidad de `Wounds` | **4** | **3–6** | Cuántos golpes aguanta un personaje. Es la perilla más directa sobre la fragilidad de la mitad 1 | [barras](../02-personaje/barras-y-recursos.md) |
| Espacios del track de `Ratchet` | **20** | **16–24** | Cuánto `Toll` aguanta un personaje antes de perderse. Moverlo sin re-espaciar los umbrales cambia la progresión de rebote | [trinquete](../02-personaje/trinquete.md) |
| Umbrales del track | **6** (4, 8, 12, 15, 18, 19) | **5–7**, y el **espaciado importa más que la cantidad** | Las decisiones de build por personaje. El espaciado original acelera: 4-4-4-3-3-1 | [trinquete](../02-personaje/trinquete.md) |
| Pool de `Servant` | 10 | **6–14** | El techo de presión simultánea en el mapa | [enemigos](../04-oposicion/enemigos.md) |
| Espacios totales del mapa | sin dato | **12–32** | Cuánto cuesta cruzar el mapa, y por lo tanto el valor de `Swiftness` | [mapa](../01-fundamentos/mapa-y-espacios.md) |

> **`T` y los espacios del `Doom Track` no se mueven juntas.** Las dos alargan la partida por
> el mismo lado; tocar las dos a la vez duplica el efecto sin que se note de dónde vino.

---

## La distribución de caras del dado

**Verificada.** El manual **no la publica** —ni el de Season 3 ni el core original, comprobado
extrayendo el texto de los dos: listan los cuatro resultados y avisan que algunas caras traen
dos, pero nunca dan el reparto, porque está impreso en los dados y no en el reglamento.

Se recuperó de fuentes de la comunidad, con dos coincidencias independientes, y **cuadra con las
dos pistas que sí están en el manual**: que hay una cara con dos resultados, y el ejemplo de la
pág. 14 donde una tirada saca *elder sign, blank, success + tentacle, tentacle*.

### Dado estándar

| Cara | Cantidad |
|---|---|
| `Hit` | 2 |
| `Hit` + `Toll` | 1 |
| `Toll` | 1 |
| `Wild` | 1 |
| `Blank` | 1 |

    p(Hit)   = 3/6 = 0,50
    p(Toll)  = 2/6 = 0,33
    p(Wild)  = 1/6 = 0,17
    p(Blank) = 1/6 = 0,17

### Dado bonus

| Cara | Cantidad |
|---|---|
| `Hit` | 2 |
| `Wild` + `Hit` | 1 |
| `Wild` | 1 |
| `Blank` | 2 |

    p(Hit)   = 3/6 = 0,50
    p(Wild)  = 2/6 = 0,33
    p(Blank) = 2/6 = 0,33
    p(Toll)  = 0      <- esto sí lo confirma el manual, pág. 11

**El dato que esto revela:** los dos dados tienen **la misma probabilidad de `Hit`**. El dado
bonus no pega más fuerte — pega **igual, sin cobrar `Toll`**, y con el doble de chances de
`Wild`. O sea que el premio del trinquete no es potencia: es **quitarle el costo a la tirada**, y
además le sube el valor a la skill `Insight`, que es la que lee `Wild`.

Eso reordena una prioridad de balanceo: `Insight` escala con la cantidad de dados bonus que
tengas, así que un personaje quemado con `Insight` alto es una combinación bastante más fuerte de
lo que las dos piezas sugieren por separado. Hay que vigilarla.

### El espaciado de los umbrales

Los 6 umbrales están en las casillas **4, 8, 12, 15, 18 y 19** de 20. Los saltos son
**4, 4, 4, 3, 3, 1**.

**El espaciado acelera.** La primera mitad del track cobra 4 puntos por umbral y la última cobra
1. Consecuencias:

- El 5.o y el 6.o umbral llegan **casi juntos**, y el 6.o está a **una sola casilla** de la muerte.
- El último tramo entrega dos niveles de skill casi de golpe y después mata.
- La estrategia de quemar un personaje tiene un **acelerón final**: llegado al 5.o umbral, el 6.o
  sale con el primer `Toll` que aparezca. Pero el paso siguiente es perderlo.

Nada de esto estaba en el GDD antes de verificar el componente, y cambia cómo se lee el
sacrificio deliberado: no es una pendiente pareja, es una rampa que se empina al final.

## Valores que el manual no contiene

El manual es un **reglamento, no una especificación de componentes**: los largos de track están
impresos en los tableros de personaje, las caras en los dados, y las vidas y dados de ataque en
las fichas de enemigo. Comprobado extrayendo el texto del reglamento de Season 3 y del core
original: ninguno de los dos los trae.

**Resueltos** con fuentes de la comunidad, dos coincidencias independientes cada uno:

| Qué faltaba | Valor | Estado |
|---|---|---|
| Distribución de caras de los dos dados | ver arriba | verificado y coherente con el manual |
| Espacios del track de `Ratchet` | 20 | verificado |
| Umbrales y sus posiciones | 6, en 4/8/12/15/18/19 | verificado |
| Capacidad de `Wounds` | 4, la 5.a es letal | verificado |
| Capacidad de `Reserve` | 4 | verificado, coincide con el manual |

**Todavía abiertos:**

| Qué falta | Dónde estaría | Impacto |
|---|---|---|
| Cuáles 4 de los 6 umbrales dan dado bonus | Tablero de personaje | Medio. Cambia cuándo llega el poder, no cuánto |
| Vida y dados de ataque por tipo de enemigo | Fichas de enemigo | Medio. Del manual solo se ven ejemplos sueltos: un `Servant` con 2 de vida, una `Creature` con 3, y una ficha de 1 estándar + 2 bonus |
| Cuántas de las 16 cartas de presión traen `Doom Symbol` | Las cartas | Medio. Define la varianza del reloj |
| Vida y dados por stage del `Adversary` | Stage cards | Bajo, es contenido propio de todos modos |
| Capacidad de heridas de cada `Ally` y precio de cada recompensa | Las cartas | Bajo, es contenido propio |

Los que quedan son de impacto medio o bajo, y los tres primeros son contenido que este proyecto
diseña igual. **Ya no hay nada que bloquee un cálculo.**

## Procedencia

Los valores de componente no salen del reglamento. Se verificaron contra:

- [Into the De(ath)pths, Part II — There Will Be Games](https://www.therewillbe.games/articles-analysis/9443-into-the-de-ath-pths-a-deep-dive-into-cthulhu-death-may-die-part-ii-basics-of-investigation) — caras de los dos dados, capacidad de `Wounds` y `Reserve`
- [Dice Probabilities & Arcane Mastery — BoardGameGeek](https://boardgamegeek.com/thread/3099040/dice-probabilities-and-arcane-mastery) — probabilidades de los dos dados
- [Dized Rules — Sanity](https://rules.dized.com/game/kp6yXekcT6agDFGangIB1Q/XXNB96tBSqWBcLGQXdDfYA/sanity) — mecánica del track y los 4 dados bonus
- [Review — Co-op Board Games](https://coopboardgames.com/cooperative-board-game-reviews/cthulhu-death-may-die-review/) — track de 20 espacios y posiciones de los umbrales
- [Reglamento del core original (PDF)](https://cdn.1j1ju.com/medias/e1/c4/90-cthulhu-death-may-die-rulebook.pdf) — usado para **confirmar la ausencia**: 20 páginas extraídas, ningún largo de track ni cara de dado

**Son fuentes de terceros, no oficiales.** Se marcan como verificadas porque hay dos
coincidencias independientes por dato y porque cuadran con las pistas del manual. Si algún día se
puede leer el componente directo, conviene revalidar.
