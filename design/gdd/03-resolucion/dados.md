# 🎲 Dados

← [Índice](../README.md)

## La tirada base

Toda tirada es **3 dados estándar + los dados bonus que apliquen**.

El jugador **no puede elegir tirar menos** dados de los que le corresponden, salvo que el
efecto sea explícitamente opcional. No se puede jugar a lo seguro tirando menos.

## Las cuatro caras

| Cara | Efecto |
|---|---|
| `Hit` | Éxito. Atacando, es 1 herida al objetivo |
| `Wild` | Nada, salvo que una skill o carta la lea |
| `Toll` | **+1 `Ratchet`**. Siempre, en toda tirada |
| `Blank` | Nada |

Algunas caras traen **dos resultados a la vez** — por ejemplo `Hit` y `Toll` juntos. Se
aplican los dos.

## La regla que define el juego

**`Toll` cobra en TODAS las tiradas.** Atacando, defendiéndote, o tirando por cualquier
motivo.

Esto es lo que hace que actuar sea intrínsecamente caro. No existe una acción gratis: cada
vez que agarrás los dados estás aceptando la posibilidad de acercarte al final del track. Y
como el track también es tu progresión, la pregunta nunca es solo "¿me sale?".

Consecuencia directa: **no hacer nada es una jugada legítima**. Un turno donde te movés a un
espacio limpio y descansás es un turno donde no tiraste dados, y eso vale.

## Dados bonus

Vienen de tres lados: skills, cartas, y los umbrales de
[`Ratchet`](../02-personaje/trinquete.md) que dan uno permanente.

**Los dados bonus no tienen cara `Toll`.** Son puro beneficio.

Eso hace que el poder que ganás con el trinquete sea doblemente bueno: no solo tirás más
dados, tirás dados que no te pueden cobrar. Un personaje muy avanzado en el track tiene una
proporción de dados seguros mucho mejor que uno limpio.

**No hay límite** de dados en una tirada.

**Excepción importante:** los dados bonus permanentes del trinquete **no aplican a las
tiradas contra vos mismo**. Ver abajo.

## Tiradas contra vos mismo

Algunas tiradas son **en tu contra**: cada `Hit` te perjudica en vez de ayudarte. Los dos
casos son el ataque de un enemigo y la resolución de `Hazard`.

En esas tiradas los dados bonus permanentes **no se suman**, porque agregarlos sería
agregarte más chances de que te peguen. Los efectos y skills sí se aplican, salvo que digan
lo contrario.

Los dados de los enemigos **los tira el jugador**. No hay un lado de la mesa que tire por
la oposición: todo pasa por tus manos, incluido lo que te lastima.

## Rerolls

Gastás **1 `Reserve` para rerollear 1 dado**, ignorando su resultado original. Se puede
repetir todas las veces que quieras mientras te quede `Reserve`.

Funciona en **cualquier** tirada, incluidas las de los enemigos atacándote.

Algunas skills dan **rerolls gratis**, que no cuestan `Reserve`.

Los rerolls son la única herramienta del jugador contra la varianza, y son limitados. La
tensión real del sistema es que `Reserve` se gasta en el momento pero se recupera solo
descansando, y descansar exige un espacio limpio.

## Orden de resolución

Cuando terminaste de rerollear, se cuentan los resultados y se aplican **en este orden**:

**1. `Hit`.** Atacando, son heridas al objetivo. Si un efecto te pide "hacé una tirada",
seguí lo que diga la carta: a veces hay que igualar o superar un número, a veces cada `Hit`
te deja hacer algo.

**2. Efectos disparados.** Todo lo que dependa del resultado: "si sobrevive al ataque…",
"cuando el enemigo muere…".

**3. `Toll`.** Recién ahora sube el `Ratchet`, 1 por cada `Toll`. Si cruzás un umbral, el
marcador para ahí, se ignora el resto, y se resuelve el umbral completo.

El orden importa: **matás primero y pagás después**. Un personaje puede matar a un enemigo
y cruzar un umbral con la misma tirada, y el enemigo muere igual.

## Conversión de caras

Algunas skills convierten una cara en otra — `Insight` cuenta `Wild` como `Hit`.

Regla dura: **una cara convertida deja de contar como la original**, salvo que el efecto
diga explícitamente lo contrario. Y **no se puede convertir dos veces**: dos habilidades no
pueden encadenarse sobre la misma cara.

## Implementación

Los dados se tiran con **física real**: colisión, rebote y lectura de la cara que quedó
arriba. No es una animación sobre un resultado ya calculado.

Eso vale la pena por dos razones. Es la mecánica más presente del juego —se tira en casi
todas las acciones— y merece peso. Y como los dados son físicos, el jugador puede
seleccionarlos individualmente con el mouse para elegir cuál rerollear.
