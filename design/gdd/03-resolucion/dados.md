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

## 📐 Fórmulas

**Aviso previo, obligatorio.** El manual **no publica la distribución de caras del dado**.
Dice que cada dado tiene 4 resultados posibles y que algunas caras traen dos resultados a la
vez, pero nunca dice cuántas de las 6 caras físicas corresponden a cada resultado. Todo lo
que sigue está escrito **paramétrico**, y los ejemplos usan una distribución **provisional,
declarada acá**, que es una perilla de balanceo y no un dato del manual.

Distribución provisional — **NO sale del manual**:

| Dado | `Hit` | `Hit`+`Toll` | `Toll` | `Wild` | `Blank` | p(`Hit`) | p(`Toll`) | p(`Wild`) |
|---|---|---|---|---|---|---|---|---|
| estándar | 2 | 1 | 1 | 1 | 1 | 3/6 = 0.500 | 2/6 ≈ 0.333 | 1/6 ≈ 0.167 |
| bonus | 3 | — | — | 1 | 2 | 3/6 = 0.500 | **0** (esto sí sale del manual) | 1/6 ≈ 0.167 |

La suma de las probabilidades del dado estándar pasa de 1 y no es un error: no son eventos
mutuamente excluyentes, porque existe la cara doble `Hit`+`Toll`. Corolario práctico: `Hit` y
`Toll` **están correlacionados dentro del mismo dado**, así que no se pueden simular como dos
tiradas independientes.

### 1. Tamaño de la tirada

```
N = 3 + B_skill + B_carta + (B_ratchet si la tirada NO es contra vos mismo)
```

| Variable | Qué es | Rango esperado |
|---|---|---|
| `3` | dados estándar, fijos en toda tirada del personaje (manual, pág. 11) | constante |
| `B_skill` | dados bonus de skills activas (`Brawling` nivel 1 da 1, `Marksman` nivel 2 da 2) | 0–3 |
| `B_carta` | dados bonus de cartas y efectos puntuales | 0–2 |
| `B_ratchet` | dados bonus permanentes de umbrales; **4 umbrales los dan** (manual, pág. 8) | 0–4 |

Las tiradas de enemigo **no** se calculan con esta fórmula: salen de la ficha del enemigo,
como `s` dados estándar + `b` dados bonus. El único dato de ficha que aparece en el manual es
un ejemplo suelto: 1 estándar + 2 bonus (pág. 17). **Las fichas concretas de este proyecto no
están definidas todavía y el manual no las da.**

**Ejemplo:** personaje con 2 umbrales-con-dado cruzados y `Brawling` nivel 1, atacando en su
espacio: `N = 3 + 1 + 0 + 2 = 6` dados (3 estándar + 3 bonus). El mismo personaje siendo
atacado por un enemigo de 1+2 tira **3 dados** (los de la ficha), y sus 2 bonus permanentes
**no** se suman.

### 2. `Hit` esperados

```
E[Hit] = n_std × p_H_std + n_bon × p_H_bon
```

- `n_std`, `n_bon` — cantidad de dados estándar y bonus de la tirada. Rango 3–4 y 0–6.
- `p_H_std`, `p_H_bon` — probabilidad de `Hit` por dado. Con la tabla provisional, 0.5 los dos.
- Rango práctico del resultado: **1.5** (tirada pelada) a **~4.0** (personaje cargado).

**Ejemplo:** ataque con 3 estándar + 2 bonus → `E[Hit] = 3(0.5) + 2(0.5) = 2.5` heridas
esperadas. Contra un enemigo de 3 de vida, eso es "casi lo mata de una".

### 3. `Toll` esperados — el número que gobierna el juego

```
E[Toll] = n_std × p_T_std          ← los dados bonus no participan, nunca
```

Rango: **1.0** siempre, en toda tirada de personaje, sin importar cuántos bonus tenga.
Solo cambia si se toca la cantidad de dados estándar (perilla) o la distribución de caras.

**Ejemplo:** un turno de tres acciones de ataque son `3 × 1.0 = 3.0` puntos de `Ratchet`
esperados, aunque el personaje tire 7 dados por acción. Ese desacople es todo el diseño del
trinquete en una línea.

### 4. Eficiencia de la tirada

```
Eficiencia = E[Hit] / E[Toll]        "cuántas heridas compro por cada punto de Ratchet"
```

**Ejemplo:** personaje limpio → `1.5 / 1.0 = 1.5`. Personaje con los 4 dados bonus
permanentes → `3.5 / 1.0 = 3.5`. El poder que se compra con el trinquete no es un +33% de
daño: es **2.33× de eficiencia**. Esta es la métrica que hay que mirar al balancear la curva
de poder, no el daño bruto.

### 5. Distribución del `Toll` en la tirada base

```
P(k Toll) = C(3,k) × p_T^k × (1 − p_T)^(3−k)      con p_T = 1/3
```

| k | Probabilidad | % |
|---|---|---|
| 0 | 8/27 | 29.6% |
| 1 | 12/27 | 44.4% |
| 2 | 6/27 | 22.2% |
| 3 | 1/27 | 3.7% |

Verificación: `0(0.296) + 1(0.444) + 2(0.222) + 3(0.037) = 1.00`, que coincide con la
fórmula 3. Lectura de diseño: **7 de cada 10 tiradas cobran algo** y 1 de cada 4 cobra 2 o
más. No existe la tirada gratis.

### 6. Tiradas esperadas por umbral

```
N_tiradas ≈ d / E[Toll]
```

- `d` — puntos de `Ratchet` que faltan hasta el próximo umbral. **El manual no dice cuántos
  umbrales tiene el track ni a qué distancia están entre sí**; solo dice que 4 de ellos dan
  dado bonus (pág. 8).
- Rango de `d`: sin dato del manual. Provisionalmente 2–4.

**Ejemplo:** con `d = 3` y tiradas base, `3 / 1.0 = 3 tiradas`, o sea **un turno de puro
ataque por umbral**. Ojo con el desperdicio: como el exceso se descarta, si el umbral está a
1 y la tirada saca 3 `Toll`, se pagan 3 y avanza 1. Las tiradas con muchos `Toll` de golpe
son *menos* eficientes como progresión, no más.

### 7. Conversión de caras (`Insight`)

```
Hit_efectivos = H + c × min(W, w_max)
```

| Variable | Qué es | Valores |
|---|---|---|
| `H` | `Hit` realmente tirados | 0…N |
| `W` | `Wild` tirados | 0…N |
| `c` | multiplicador de la conversión (manual, pág. 24) | 1 en niveles 1–3, 2 en nivel 4 |
| `w_max` | cuántos `Wild` se pueden convertir | 1 en nivel 1, ilimitado en nivel 2+ |

En esperanza, con `w_max` ilimitado: `E[Hit_ef] = N × (p_H + c × p_W)`.

**Ejemplo:** 3 dados estándar. Sin `Insight`, `E = 1.5`. Con `Insight` nivel 2,
`3 × (0.5 + 0.167) = 2.0`. Con nivel 4, `3 × (0.5 + 0.333) = 2.5`. O sea que `Insight` nivel 4
vale en esperanza **lo mismo que 2 dados bonus**, y a diferencia de los bonus también rinde en
las tiradas contra vos mismo cuando el efecto lo permite.

### 8. Valor marginal de un reroll

Rerollear el dado `i` mueve la esperanza así:

```
ΔE[Hit]  = p_H − h_i
ΔE[Toll] = p_T − t_i
```

`h_i` y `t_i` valen 1 si la cara actual de ese dado aporta ese resultado y 0 si no. Costo
fijo: 1 `Reserve` (manual, pág. 11).

| Cara que rerolleás (dado estándar) | ΔE[Hit] | ΔE[Toll] | Veredicto |
|---|---|---|---|
| `Toll` solo | +0.50 | −0.67 | el mejor reroll del juego: mejora las dos barras |
| `Blank` | +0.50 | +0.33 | compra daño aceptando riesgo |
| `Wild` sin `Insight` | +0.50 | +0.33 | idéntico a `Blank` |
| `Wild` con `Insight` nivel 2+ | −0.17 | +0.33 | **no rerollear nunca** |
| `Hit` solo | −0.50 | +0.33 | solo tiene sentido en tiradas contra vos mismo |
| `Hit`+`Toll` | −0.50 | −0.67 | depende de qué te esté matando primero |

**Ejemplo resuelto:** tirada de 3 estándar que sale `Hit`, `Toll`, `Blank`. Sin rerolls, el
resultado firme es 1 herida y +1 `Ratchet`. Gastando 2 `Reserve` para rerollear el `Toll` y el
`Blank`, la tirada pasa a `E[Hit] = 1 + 0.5 + 0.5 = 2.0` y `E[Toll] = 0 + 0.33 + 0.33 = 0.67`.
Dos `Reserve` compraron +1.0 de daño esperado y −0.33 de `Ratchet` esperado.

### 9. Procedimiento de resolución (esto no es aritmética, es orden)

Entradas: el multiconjunto de caras después de los rerolls, el contexto de la tirada (ataque,
defensa o efecto) y el objetivo. Salidas: heridas aplicadas, efectos disparados y delta de
`Ratchet`.

```
1. Cerrar rerolls. A partir de acá no se toca nada.
2. Aplicar conversiones de cara. Una sola por cara; la cara convertida deja de ser la original.
3. Contar Hit, Wild, Toll, Blank.
4. Aplicar los Hit  → heridas al objetivo, o el requisito que pida la carta.
5. Resolver muertes y efectos disparados por el resultado.
6. Aplicar los Toll → +1 Ratchet cada uno, de a uno.
7. Si el marcador toca un umbral: PARAR ahí, descartar los Toll restantes y resolver el
   umbral completo (Ratchet Card → +1 nivel de skill → dado bonus si ese umbral lo da).
```

**Ejemplo recorrido.** Personaje con `Insight` nivel 1 y `Ratchet` a 1 punto de un umbral,
atacando a un `Servant` de 2 de vida con 3 estándar + 1 bonus. Sale: `Hit`+`Toll`, `Wild`,
`Blank`, y `Hit` en el bonus.

1. No rerollea.
2. `Insight` nivel 1 convierte el único `Wild` en `Hit`. Ese dado **ya no cuenta como `Wild`**.
3. Conteo: 3 `Hit`, 1 `Toll`, 1 `Blank`.
4. 3 heridas al `Servant`, que tiene 2 de vida → **muere**. La herida sobrante se pierde.
5. Se resuelven los efectos de muerte del `Servant`, si tiene.
6. 1 `Toll` → +1 `Ratchet`.
7. El marcador **toca el umbral**: para ahí, dispara la `Ratchet Card`, sube 1 nivel de skill
   y, si ese umbral trae dado, suma +1 bonus permanente por el resto de la partida.

El enemigo murió **antes** de que el personaje pagara el umbral. Ese es exactamente el punto
del orden.

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| Cara doble `Hit`+`Toll` | Se aplican los dos resultados del mismo dado: 1 herida **y** +1 `Ratchet`. El `Hit` se resuelve en el paso 4 y el `Toll` en el 6, así que la herida existe incluso si ese `Toll` te hace cruzar un umbral (manual, pág. 12). |
| 3 `Toll` con el umbral a 1 de distancia | El marcador avanza 1, **para en el umbral**, y los 2 `Toll` restantes se **descartan**. No se acarrean al umbral siguiente ni a nada (manual, pág. 8). |
| Una tirada que atravesaría **dos** umbrales | Imposible por construcción: el marcador para en el primero y el exceso muere ahí. Máximo **un umbral por tirada**. |
| Matar al enemigo y cruzar un umbral con la misma tirada | El enemigo muere igual. Los `Hit` van en el paso 4 y el `Toll` en el 6 (manual, pág. 12). |
| Tirada contra vos mismo teniendo dados bonus permanentes | Los permanentes del trinquete **no se suman** (manual, pág. 11). Los bonus de skills y cartas **sí** se aplican, salvo que el efecto diga lo contrario. |
| Enemigo cuya ficha trae dados bonus | Se tiran igual, y esos bonus no tienen cara `Toll`: un enemigo de 1 estándar + 2 bonus puede cobrarte **como máximo 1** punto de `Ratchet` (manual, pág. 17). La regla de "no sumar bonus" es sobre los dados del personaje, no sobre la ficha del enemigo. |
| `Reserve` en 0 | No hay más rerolls pagos. Siguen disponibles los **rerolls gratis** de skill (`Toughness` nivel 1 da 1 al ser atacado o en `Hazard`; `Brawling` nivel 3 da 2 atacando en tu espacio). |
| Rerollear el mismo dado dos veces seguidas | Permitido mientras quede `Reserve`. El manual lo dice explícito en su ejemplo de la pág. 14. |
| Reroll gratis y reroll pago en la misma tirada | Se combinan, y **el orden lo elige el jugador**. El ejemplo del manual (pág. 17) usa primero el gratis de `Toughness` y después uno pago. |
| Rerollear un `Hit` que ya salió | Permitido. Es la jugada normal en tiradas contra vos mismo (rerolleás los `Hit` del enemigo) y cuando el enemigo castiga los `Hit`. |
| Dos habilidades queriendo convertir la misma cara | **Prohibido.** Una cara no se convierte dos veces, y una cara convertida deja de contar como la original salvo que el efecto lo aclare (manual, pág. 12). |
| `Insight` nivel 1 con 3 `Wild` en la tirada | Solo **1** cuenta como `Hit`. Los otros 2 quedan inertes y no se pueden convertir con nada más. |
| `Toughness` nivel 2+ contra una tirada con 2 `Toll` | Se cuentan los 2 `Toll` y después se **reduce la pérdida en 1**: el `Ratchet` sube 1, no 2. La reducción no baja de 0: 1 `Toll` reducido en 1 es 0, nunca −1 (manual, pág. 25). |
| Se acaban los dados físicos | En la mesa se anota el resultado y se vuelven a tirar los mismos dados (manual, pág. 11). En digital **no hay límite** y la implementación no debe cappear la cantidad. Lo que sí hay que decidir es el límite de layout de la mesa de dados en pantalla: **eso no está en el manual**. |
| Tirada de 0 dados (un efecto deja la tirada en cero) | **El manual no lo cubre.** Decisión a tomar: 0 dados produce 0 `Hit` y 0 `Toll` y **no cuenta como tirada**, así que no dispara efectos del tipo "cuando hacés una tirada". |
| `Hazard` con 0 tokens a fin de turno | No se tira nada. No es una tirada de 0 dados: el paso entero se saltea. |
| `Hit` de sobra sobre un enemigo ya muerto, sin `Brawling` nivel 2+ | Se pierden. El objetivo se elige **antes** de tirar y todas las heridas van a ese objetivo aunque sobren (manual, pág. 14). |
| El `Toll` que llega al final del track | El personaje **se pierde y sale del juego**. La regla de exceso protege umbrales, no el final del track: el manual no marca el final como umbral, así que ahí no hay freno. |
| Cara `Wild` en la tirada de un enemigo | No hace nada, salvo que el personaje **o el enemigo** tengan un efecto que la lea (manual, pág. 17). Es el único caso en que una cara del enemigo puede beneficiarte. |
| Un efecto pide "igualar o superar X `Hit`" y sale exactamente X | Cuenta como éxito: el manual dice "match or exceed" (pág. 12). Los `Toll` de esa tirada se cobran igual, haya éxito o no. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución provisional** declarada
> ahí mismo: `p(Hit) = 0,50`, `p(Toll) = 0,33`.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Caras `Toll` por dado estándar | **No está en el manual.** Provisional: 2 de 6, o sea `p_T = 1/3` | 1–3 de 6 (`p_T` 0.167–0.5) | La velocidad de la progresión y la del reloj de muerte, al mismo tiempo y en el mismo sentido. Es la perilla más peligrosa del juego: mover esto reescribe la duración de la partida y desincroniza la ventana de sacrificio con la manifestación del adversario → rompe [`04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md). |
| Caras `Hit` por dado estándar | **No está en el manual.** Provisional: 3 de 6 | 2–4 de 6 | Cuántas acciones cuesta matar cada enemigo, o sea la densidad de combate del turno. Subirlo hace que el mapa se limpie solo y el referéndum deje de doler. |
| Caras `Wild` por dado estándar | **No está en el manual.** Provisional: 1 de 6 | 1–2 de 6 | El valor de `Insight`, y nada más. Con 0 `Wild` la skill deja de existir; con 2 pasa a ser la mejor del pool común. |
| Dados estándar por tirada | 3 (manual, pág. 11) | 2–4 | Sube el daño y el `Ratchet` **en la misma proporción**, así que no sirve para ajustar letalidad sin tocar progresión. Para eso están las caras. |
| `Toll` en los dados bonus | Ninguno (manual, pág. 11) | No tocar | Si los bonus cobraran `Toll`, el premio de los umbrales dejaría de ser premio y la estrategia de quemar un personaje a propósito se muere. Es un axioma disfrazado de perilla. |
| Costo de reroll | 1 `Reserve` por dado (manual, pág. 11) | 1–2 | Cuánta varianza puede comprar el jugador por turno. A 2, `Reserve` alcanza para dos arreglos y las tiradas malas se vuelven casi definitivas. |
| Máximo de `Reserve` | 4 (manual, pág. 8) | 3–6 | Cuántos rerolls hay entre dos `Recover`. Interactúa con `Recover`: con máximo 6 y curación de 3 puntos, el techo nunca se alcanza de una sola acción. |
| Umbrales que dan dado bonus | 4 (manual, pág. 8) | 2–6 | La pendiente de la curva de poder. Con 6, un personaje quemado llega a eficiencia 4.5 y trivializa el combate tardío. |
| Bonus permanentes en tiradas contra vos mismo | No aplican (manual, pág. 11) | No tocar | Activarlo invierte el signo del trinquete: subir de nivel te haría más frágil al ser atacado y el jugador dejaría de querer `Ratchet`. Rompe [`trinquete.md`](../02-personaje/trinquete.md) entero. |
| Multiplicador de conversión de `Insight` | 1 en niveles 1–3, 2 en nivel 4 (manual, pág. 24) | 1–2 | Cuánto vale la cara inerte. En nivel 4 con multiplicador 2, la skill equivale a 2 dados bonus que además funcionan contra vos mismo. |

## ✅ Criterios de aceptación

1. Una tirada sin ningún modificador usa **exactamente 3 dados estándar**, y la UI no ofrece la opción de tirar menos.
2. Un dado que cae en la cara `Hit`+`Toll` produce **1 herida y +1 `Ratchet`** en la misma tirada, y la herida se aplica antes de mover el marcador.
3. Un personaje con 4 dados bonus permanentes y `Brawling` nivel 1, atacando en su espacio, tira **8 dados**, de los cuales **exactamente 3** pueden producir `Toll`.
4. Ese mismo personaje, atacado por un enemigo de ficha 1 estándar + 2 bonus, tira **exactamente 3 dados**: los 4 bonus permanentes no aparecen en esa tirada.
5. Gastar 1 `Reserve` rerollea exactamente 1 dado; con `Reserve` en 0 el reroll pago queda deshabilitado y los rerolls gratis de skill siguen usables.
6. El mismo dado se puede rerollear 2 veces consecutivas gastando 2 `Reserve`.
7. Un `Servant` de 2 de vida que recibe 3 `Hit` desaparece del tablero **antes** de que el marcador de `Ratchet` se mueva por los `Toll` de esa misma tirada.
8. Con el umbral a 1 punto de distancia y 3 `Toll` en la tirada, el marcador queda **EN** el umbral y los 2 `Toll` restantes se descartan: el track no avanza ni un punto más.
9. Ninguna tirada dispara 2 umbrales: con 3 `Toll` y umbrales a distancia 1 y 2, se resuelve solo el primero.
10. `Insight` nivel 1 sobre una tirada con 3 `Wild` cuenta **1** como `Hit`, y los otros 2 no son convertibles por ningún otro efecto de esa tirada.
11. Una cara convertida de `Wild` a `Hit` **no** satisface un efecto que pida `Wild` en la misma tirada.
12. Simulando 10.000 tiradas de 3 dados estándar con la distribución de caras vigente, la media de `Toll` cae en 1.0 ± 0.05 y la de `Hit` en 1.5 ± 0.05.
