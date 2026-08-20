# 🎓 Skills

← [Índice](../README.md)

## Estructura

Cada `Character` tiene **3 skills**:

- La primera es su **skill de firma**, única de ese personaje.
- Las otras **2 salen de un pool común de 6**, así que dos personajes pueden compartirlas.

Cada skill tiene **4 niveles**. Todos arrancan en nivel 1.

**Se sube exactamente de una forma: cruzando un umbral de
[`Ratchet`](trinquete.md).** Cada umbral da 1 nivel, a elección del jugador.

## Cómo suben

Hay dos comportamientos distintos, y la diferencia importa para la estrategia:

- Algunos niveles **se suman** al anterior: el personaje conserva las dos habilidades.
- Otros **reemplazan** al anterior: la habilidad de nivel 2 desaparece al llegar a nivel 3.

Cuál es cuál está escrito en cada skill. La consecuencia de diseño es que **subir no siempre
es mejorar**: hay ramas donde conviene quedarse en un nivel intermedio, y eso hace que la
decisión del umbral no sea automática.

Un `Ally` también puede dar un nivel de skill mientras esté con el personaje. Si el
personaje no tenía esa skill, la gana en nivel 1; si ya la tenía, sube 1. Al perder el
`Ally`, baja de vuelta.

## Los 6 skills comunes

| ID | Qué hace | Eje |
|---|---|---|
| `Insight` | Convierte caras `Wild`, que son inertes, en éxitos | Economía de dados |
| `Brawling` | Pega mejor y a más enemigos dentro de tu espacio | Combate cuerpo a cuerpo |
| `Marksman` | Ataca a enemigos que no están en tu espacio | Combate a distancia |
| `Stealth` | Evita que los enemigos te sigan al moverte | Movilidad |
| `Swiftness` | Más movimiento y más acciones por turno | Economía de acciones |
| `Toughness` | Reduce daño recibido y pérdida de `Ratchet` | Defensa |

## Las 24 celdas

**6 skills × 4 niveles = 24 efectos distintos.** Esa es la superficie de maestría real del
juego: es donde el jugador aprende, compara y arma su build.

**El detalle exacto de las 24 celdas ya está transcripto** en
`docs/rulebook/rulebook.md`, páginas 24 y 25. No se duplica acá a propósito: si estuviera en
dos lugares, en algún momento van a divergir. Ese archivo es la fuente.

## Cómo se relaciona cada skill con el resto del juego

**`Insight`** es el único que le da valor a la cara `Wild`, que sin él no hace nada. Es la
skill que cambia la economía de los dados en vez de los números.

**`Brawling`** y **`Marksman`** son las dos respuestas opuestas al mismo problema: los
enemigos te siguen y te atacan si comparten tu espacio. `Brawling` acepta el abrazo y lo
convierte en ventaja; `Marksman` te deja no entrar nunca.

**`Stealth`** es la contra directa de la regla de enemigos pegajosos. En la implementación,
es la que se resuelve con AI Perception.

**`Swiftness`** compra acciones, que es el recurso más escaso del turno.

**`Toughness`** es la única que reduce pérdida de `Ratchet` desde afuera. Eso la hace
extraña: es la skill que te frena la progresión a cambio de mantenerte vivo.

Ese último punto es el más interesante del set y conviene tenerlo presente al balancear: en
un juego donde perder `Ratchet` es cómo te volvés fuerte, una skill que lo previene está
comprando supervivencia con poder futuro.

---

## 📐 Fórmulas

### F0 — Qué habilidades están activas en un nivel dado

No es aritmética: es un procedimiento de decisión, y es el que más se equivoca al
implementar. El manual marca el reemplazo con la palabra **"Instead"** al principio de la
línea del nivel. Sin esa palabra, el nivel **suma**.

La tabla de abajo es **metadata estructural, no el texto de los efectos** — el texto sigue
viviendo solo en `rulebook.md` p.24-25, como dice la sección anterior. `A` = suma al
anterior, `R` = reemplaza al anterior.

| Skill | L1 | L2 | L3 | L4 | Habilidades activas en L4 |
|---|---|---|---|---|---|
| `Insight` | — | R | A | R* | 2 |
| `Brawling` | — | A | A | A | 4 |
| `Marksman` | — | A | A | A | 4 |
| `Stealth` | — | R | A | R | 2 |
| `Swiftness` | — | A | A | A | 4 |
| `Toughness` | — | R | R | R | 1 |

**R\*** — el nivel 4 de `Insight` **no lleva "Instead" en el manual**. Se lee como reemplazo
porque contar cada `Wild` como 2 éxitos subsume contarla como 1, pero es una interpretación
nuestra, no una regla escrita. Además la transcripción repite esa línea dos veces, lo que
sugiere un artefacto de transcripción. Es el único punto ambiguo de las 24 celdas.

**Procedimiento al subir un nivel** (entrada: skill elegida; salida: set de habilidades
activas):

1. Si el nivel actual es 4 → la subida no se puede aplicar (ver casos borde).
2. Nivel += 1.
3. Si el nivel nuevo es `R` → se desactiva la habilidad que reemplaza. **No se desactiva
   nada más**: los niveles `A` anteriores siguen activos. `Stealth` 4 es el caso explícito
   del manual — conserva la herida del nivel 3 aunque reemplace al nivel 2.
4. Recalcular el set activo y, con él, los dados bonus de F1.

**Ejemplo recorrido — `Toughness`, que es reemplazo puro:**
L1 = {1 reroll gratis cuando te atacan o tirás por `Hazard`}. Subo a L2: se desactiva el
reroll y queda {reducir 1 `Wound` y/o 1 `Ratchet` cuando te atacan o tirás por `Hazard`}.
Subo a L3: se desactiva eso y queda la misma reducción pero de cualquier fuente. Subo a L4:
queda {reducir 2 `Wound` y 1 `Ratchet` de cualquier fuente}. En ningún nivel `Toughness`
tiene más de **una** habilidad, mientras `Brawling` en L4 tiene cuatro. Eso no es un error de
transcripción: es la razón por la que `Toughness` tiene que ser potente por línea.

### F1 — Tamaño de la tirada

Es la misma fórmula de [`../03-resolucion/dados.md`](../03-resolucion/dados.md), acotada al
aporte de las 6 skills comunes:

```
N = 3 + B_skill + B_carta + B_ratchet
B_ratchet = 0 si la tirada es contra vos mismo
```

- **3** — dados estándar, fijo para toda tirada (`rulebook.md` p.11).
- **B_skill** — dados bonus de skills activas *en esa tirada*. De las 6 comunes solo dos dan
  dados: `Brawling` L1+ da **+1** atacando a un objetivo en tu espacio, y `Marksman` L2+ da
  **+2** atacando a un objetivo fuera de tu espacio. **Son mutuamente excluyentes** — dependen
  de dónde está el objetivo, y un objetivo no puede estar en tu espacio y fuera al mismo
  tiempo. Rango real de las comunes: **0-2**, nunca 3.
- **B_carta** — dados de cartas y efectos puntuales. **0-2**, fuera del alcance de este doc.
- **B_ratchet** — dados bonus permanentes de umbrales. Rango **0-4**: el manual dice que
  exactamente **4** de los umbrales dan uno (`rulebook.md` p.8).

Rango de `N` por aporte de skills: **3 a 9** atacando, y **3** en tiradas contra vos mismo —
ninguna skill común da dados en defensa, y los del trinquete no aplican ahí.

**Ejemplo:** personaje con `Brawling` 2 y 3 umbrales-con-dado cruzados, atacando a dos
enemigos en su espacio. `N = 3 + 1 + 0 + 3 = 7`. De esos 7, **solo 3 pueden sacar `Toll`**:
los bonus no tienen esa cara. Un personaje limpio tira 3 dados que cobran de 3; este tira 3
que cobran de 7.

### F2 — Éxitos efectivos con `Insight`

```
S = H + k(L) × min(W, m(L))
R = min(W, m(L))     solo si L ≥ 3, tope: máximo de Reserve
```

- **H** — caras `Hit` en la tirada. **W** — caras `Wild`.
- **k(L)** — cuánto vale cada `Wild` convertida: `k(1)=1, k(2)=1, k(3)=1, k(4)=2`.
- **m(L)** — cuántas `Wild` podés convertir: `m(1)=1`, `m(2..4)` sin tope.
- **R** — `Reserve` que recuperás; el nivel 3 cura 1 por cada `Wild` contada como éxito. El
  tope es el máximo de `Reserve`, que es **4** según
  [`barras-y-recursos.md`](barras-y-recursos.md). El manual no da el largo de esa barra.

**Ejemplo resuelto**, misma tirada de 6 dados: 2 `Hit`, 3 `Wild`, 1 `Blank`.

| Nivel | Éxitos | `Reserve` recuperada |
|---|---|---|
| Sin `Insight` | 2 | 0 |
| L1 | 2 + 1×1 = **3** | 0 |
| L2 | 2 + 1×3 = **5** | 0 |
| L3 | **5** | **3** |
| L4 | 2 + 2×3 = **8** | **3** |

De 2 a 8 éxitos con los mismos dados. Eso es lo que quiere decir "cambia la economía en vez
de los números".

### F3 — Heridas repartidas con `Brawling`

Con `S` éxitos y `T` objetivos elegidos **antes** de tirar:

```
Sin Brawling o L1:  T = 1,  heridas = min(S, HP_objetivo),  el resto se pierde
L2 y L3:            Σ w_i = S             (repartís S entre los T objetivos)
L4:                 w_i = S para todo i   →  heridas totales = S × T
```

`T` va de 1 a la cantidad de enemigos en tu espacio. El multiplicador efectivo de L4 es
exactamente `T`.

**Ejemplo:** `S = 2`, dos objetivos con 2 y 3 de vida.

- **L2:** reparto 2 heridas. O mato al de 2 y el otro no recibe nada, o dejo a los dos a 1 de
  morir. Total aplicado: **2 heridas**.
- **L4:** cada uno recibe 2. El de 2 muere, el de 3 queda con 1. Total: **4 heridas** con 2
  éxitos. Con 3 objetivos y `S = 3` serían **9**.

### F4 — Presupuesto de niveles de la partida

```
Niveles ganados = U    (U = umbrales de Ratchet cruzados; 1 nivel por umbral)
Σ (Lv_i − 1) = U       con Lv_i ∈ [1, 4] y 3 skills  →  techo de U útil = 9
```

**Cuántos umbrales tiene el track NO está en el manual.** La transcripción solo dice que 4 de
ellos dan dado bonus (p.8); el total está impreso en el tablero de personaje, que la
transcripción no incluye. `trinquete.md` estima 4-6 y esa estimación **no tiene fuente**. Es
el número que decide si las 24 celdas son alcanzables o decorativas: con 5 umbrales, un
personaje toca 5 de las 9 subidas posibles y no ve un nivel 4 salvo que se dedique a uno solo.

**Ejemplo con U = 5.** Las reparticiones legales son exactamente tres: `(4,3,1)`, `(4,2,2)` y
`(3,3,2)`. Y la decisión no es cosmética: si una de las skills es `Toughness`, dejarla en 1
(opción `4,3,1`) es **mejor** para un personaje que quiere subir `Ratchet`, porque
`Toughness` 2+ le frena la progresión. Ahí está la rama donde subir es empeorar.

### F5 — Mitigación de `Toughness`

Por **fuente**, no por turno:

```
W_final = max(0, W − rw(L))
T_final = max(0, T − rt(L))
```

- **rw** (reducción de `Wounds`): `L1=0, L2=1, L3=1, L4=2`.
- **rt** (reducción de `Ratchet`): `L1=0, L2=1, L3=1, L4=1`.
- **Alcance:** L1 y L2 solo cuando te atacan o tirás por `Hazard`; L3 y L4, cualquier fuente.
- L1 no reduce nada: da 1 reroll gratis en esas mismas tiradas.
- El manual aclara (p.25) que en L2 y L3 podés prevenir **1 `Wound`, 1 `Ratchet`, o los dos**,
  de la misma fuente.

**Ejemplo:** un enemigo te ataca y salen 2 `Hit` + 1 `Toll`.

| Nivel | Resultado |
|---|---|
| Sin skill | 2 `Wounds`, +1 `Ratchet` |
| L1 | rerolleás gratis 1 `Hit`; si sale `Blank`, 1 `Wound` y +1 `Ratchet` |
| L2 / L3 | 1 `Wound`, **+0 `Ratchet`** |
| L4 | **0 `Wounds`, +0 `Ratchet`** |

Ahí se ve el costo escondido: L4 te ahorra 1 punto de `Ratchet` **por fuente**. Con 3 enemigos
atacándote son 3 puntos de progresión que no compraste.

### F6 — Economía de acciones y movimiento con `Swiftness`

```
A = 3 + 1[L ≥ 4]                          acciones del turno
M = 3 + 1[L ≥ 1]                          espacios por acción Move
Espacios máx. por turno = (A + 1[L ≥ 2]) × M
```

| Nivel | A | `Move` gratis | M | Espacios máx. |
|---|---|---|---|---|
| Sin skill | 3 | no | 3 | **9** |
| L1 | 3 | no | 4 | **12** |
| L2 | 3 | sí | 4 | **16** |
| L3 | 3 | sí | 4 | **16** (L3 no da movimiento, da carga de personajes) |
| L4 | 4 | sí | 4 | **20** |

**Ejemplo resuelto — `Swiftness` 4:** 3 acciones base + 1 extra = 4 acciones, más 1 `Move`
gratis = 5 movimientos de 4 espacios = **20 espacios en un turno**, contra 9 sin la skill. Es
×2,22.

**Esto hay que chequearlo contra el diámetro del grafo del mapa**
([`../01-fundamentos/mapa-y-espacios.md`](../01-fundamentos/mapa-y-espacios.md)): si el mapa
se cruza de punta a punta en menos de 20 espacios, `Swiftness` 4 vuelve la posición
irrelevante y con ella la mitad de la tensión del juego. El diámetro del mapa todavía no está
decidido, así que es un chequeo pendiente, no un problema confirmado.

### F7 — Alcance y ataques por turno de `Marksman`

```
d_max = 0 (sin skill) | 1 (L1, L2) | 2 (L3, L4)
Objetivo válido  ⟺  dist(vos, objetivo) ≤ d_max  ∧  el espacio es accesible
Ataques a distancia por turno = acciones gastadas en Attack + 1[L ≥ 4]
```

`dist` se cuenta en movimientos por aristas del grafo, no en línea recta (`rulebook.md`
p.24). Un espacio inaccesible por cualquier motivo no se puede apuntar aunque la distancia
alcance.

**Ejemplo — el pico de output del juego.** `Marksman` 4, 4 umbrales-con-dado cruzados, las 3
acciones gastadas en `Attack` a distancia más el ataque gratis = 4 ataques. Cada uno tira
`N = 3 + 2 + 4 = 9` dados. Son **36 dados en un turno**, de los cuales solo 12 pueden cobrar
`Toll`. Un personaje así hace daño a escala y casi no paga trinquete: es el que hay que
vigilar al balancear.

### F8 — Esquives de `Stealth`

El manual llama a esta maniobra **"Sneak"**, que todavía no tiene ID neutro en
[`../01-fundamentos/vocabulario.md`](../01-fundamentos/vocabulario.md). Acá se la nombra en
prosa como "esquive" a propósito, para no inventar un ID por la ventana.

```
s(L) = 1 (L1) | 3 (L2, L3) | sin tope (L4)       esquives por acción Move
Enemigos que te siguen = max(0, E_total − esquives gastados)
Heridas repartidas = 1 × esquives gastados,  solo si L ≥ 3
```

Cada esquive saca **1** enemigo del grupo que te sigue al salir de un espacio. Esquivar dos
veces al mismo enemigo gasta **2** (p.24). Los esquives se pueden usar en cualquier punto de
la acción `Move` y en salidas de espacios distintos.

**Ejemplo:** `Stealth` 3, salís de un espacio con 4 enemigos y gastás los 3 esquives. Te
sigue **1**, y los 3 esquivados reciben **1 herida cada uno**.

Ojo con eso: son 3 heridas **sin tirar un dado**, o sea sin posibilidad de `Toll`. `Stealth` 3
es la única fuente de daño del set que no pasa por el trinquete, y con `Stealth` 4 — esquives
sin tope conservando la herida — escala con la cantidad de enemigos. Es lo contrario de lo que
hace el resto del juego y es la celda más peligrosa de las 24.

### F9 — Nivel efectivo con `Ally`

```
Lv_ef = min(4, Lv_base + a)      a = cantidad de Ally en posesión que dan ese skill
```

Cada `Ally` que da un skill da **1** nivel (`rulebook.md` p.16). Si `Lv_base = 0` — el
personaje no tenía esa skill — la gana en nivel 1 mientras tenga el `Ally`, y **pasa a tener
4 skills**. Al perderlo, `Lv_ef` baja 1.

**Ejemplo:** `Brawling` 2 base más un `Ally` que da `Brawling` → `Lv_ef = 3`, o sea 2 rerolls
gratis atacando en tu espacio. El `Ally` absorbe una herida y se descarta en mitad del turno →
`Lv_ef` vuelve a 2 al instante. Los rerolls ya usados no se devuelven.

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| Cruzás un umbral con las 3 skills en nivel 4 | **El manual no lo cubre.** Decisión: el umbral dispara igual la `Ratchet Card` y el dado bonus si corresponde, y la subida de nivel **se pierde sin compensación**. Solo puede pasar si el track tiene más de 9 umbrales, número que el manual no da. |
| Un `Ally` da nivel de un skill que ya está en 4 | **No cubierto.** Decisión: queda en 4 (el `min` de F9 corta), el nivel se desperdicia, y al perder el `Ally` **no baja a 3**. La implementación tiene que guardar `Lv_base` y `Lv_ef` separados o se rompe justo acá. |
| Un `Ally` da un skill que el personaje no tiene | La gana en nivel 1 y tiene **4 skills** mientras lo conserve (manual p.16, explícito). |
| Se pierde ese `Ally` | La skill **desaparece entera**, no queda en nivel 1. Vuelve a tener 3 skills. |
| Se pierde el `Ally` después de haber usado el efecto del nivel que daba | **No cubierto.** Decisión: lo ya aplicado no se revierte; el nivel baja para la próxima tirada. |
| `Insight` en una tirada contra vos mismo | **Permitido explícitamente** (p.24: "including rolls against themself, if the player wants"). Cada `Wild` que contás como éxito ahí te pega a vos. Con L3 es un canje deliberado: heridas a cambio de `Reserve`. |
| Dos efectos quieren convertir la misma cara | Prohibido. Una cara convertida deja de contar como la original y no se puede convertir dos veces ([`../03-resolucion/dados.md`](../03-resolucion/dados.md)). |
| `Insight` 1 con 3 `Wild` en la tirada | Solo **1** cuenta. El tope es 1 por tirada, no 1 por dado: las otras 2 `Wild` no hacen nada. |
| Subís `Toughness` de 1 a 2 | **Perdés el reroll gratis** y ganás la reducción. Es legal, es irreversible, y para un personaje que quiere `Ratchet` es un downgrade. La UI tiene que mostrar qué se apaga **antes** de confirmar. |
| Querés bajar un nivel comprado con umbral | Imposible. El manual no da ninguna forma de bajar niveles; los únicos que bajan son los que da un `Ally`. |
| `Brawling` 2+: elegiste 3 objetivos y los éxitos alcanzan para matar al primero | Los objetivos se cierran **antes** de tirar. El exceso sobre la vida de un objetivo **se pierde**, no se reasigna. Con L4 cada objetivo recibe el total completo y el exceso se descarta por objetivo, por separado. |
| `Brawling` 2+ contra varios enemigos con efectos "cuando es atacado" | Cada objetivo dispara los suyos y **el jugador elige el orden** (p.24). |
| `Marksman` apuntando a un espacio inaccesible a distancia válida | **No es objetivo válido** (p.24). La distancia no alcanza para habilitarlo. |
| `Marksman` 4: el ataque gratis contra un enemigo en tu espacio | **Ilegal.** El ataque gratis solo sirve contra objetivos fuera de tu espacio (p.24). Si no hay ninguno, se pierde. |
| Un efecto te mueve y querés esquivar | No podés. El esquive existe **solo durante una acción `Move`**; en cualquier otro movimiento los enemigos te siguen normal (p.24). |
| Esquivás dos veces al mismo enemigo con `Stealth` 3 | Gasta **2** esquives (p.24, explícito). Si eso le hace 1 o 2 heridas **no está en el manual**. Decisión: **2**, una herida por esquive gastado. |
| `Stealth` 2: repartís los 3 esquives en salidas de espacios distintos del mismo `Move` | Legal (p.24). También es legal usarlos todos en la misma salida. |
| `Swiftness` 3: te llevás un personaje y ese personaje deja enemigos atrás | **No cubierto por el manual.** Decisión: aplica la regla general de [`../03-resolucion/turno.md`](../03-resolucion/turno.md) — al personaje llevado los enemigos **lo siguen** y **agarra `Hazard`** del espacio que deja, porque no está haciendo su propia acción `Move` pero tampoco está siendo *colocado*. |
| `Swiftness` 3 con L4: la decisión de llevar o no | Es **independiente en cada salida** de la misma acción `Move` (p.25, con ejemplo). |
| `Swiftness` 2: cuándo se puede usar el `Move` gratis | Solo en la fase de acciones, antes o después de las otras (p.13, free actions). No se puede guardar para otra fase. |
| `Toughness` 2/3 y 3 enemigos atacándote | Son **3 fuentes distintas**: la reducción aplica **a cada ataque**, no una vez por turno. La tirada completa de `Hazard`, en cambio, cuenta como **una** sola fuente. |
| `Toughness` reduce un `Toll` que iba a cruzar un umbral | El `Toll` reducido **nunca se aplica**, así que el umbral no se cruza. Primero se reduce, después se aplica, y recién ahí el marcador para en el umbral. La reducción y el descarte del exceso del umbral **no se acumulan**. |
| La reducción supera lo recibido | `max(0, …)`: nunca es negativa y **no cura nada**. |
| Las skills de firma | **No están en el manual transcripto.** Cada personaje tiene 1 firma más 2 comunes, y las firmas están impresas en los tableros de personaje, que la transcripción no incluye. Todo lo de arriba cubre las 6 comunes; las firmas son contenido a diseñar de cero. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Niveles por skill | 4 | 3-5 | El largo de la curva de maestría. En 3 se borran 6 celdas y la build queda resuelta a mitad de partida; en 5 hay que inventar 6 efectos que el manual no tiene. |
| Skills por personaje | 3 (1 firma + 2 comunes) | 2-4 | Cuánto se diferencian dos personajes y cuánto tiene que leer el jugador antes de decidir un umbral. Con 4, dos personajes cualesquiera comparten casi todo el pool. |
| Tamaño del pool común | 6 | 5-8 | La variedad entre partidas. Con menos de 5, dos de los 4 personajes comparten las dos comunes y las builds se repiten. |
| Niveles por umbral | 1 | 1-2 | La velocidad de la progresión. En 2 se duplica la build y el presupuesto de F4 se agota antes de la mitad: los umbrales tardíos dejan de comprar nada. |
| Dados bonus de `Brawling` L1 | 1 | 1-2 | Cuánto premia meterse en el abrazo. Cada dado extra es ~33% más de éxitos esperados y **no** trae `Toll`: sube el output sin subir el costo. |
| Dados bonus de `Marksman` L2 | 2 | 1-3 | Lo mismo desde afuera, y decide si pelear a distancia domina. En 3, `Marksman` 4 llega a 10 dados por ataque (F7). |
| Rerolls gratis de `Brawling` L3 | 2 | 1-3 | Cuánta varianza absorbe el personaje sin gastar. Compite con `Reserve` (máximo 4): con 3 rerolls gratis por ataque, `Reserve` deja de ser escaso y `Recover` pierde sentido. |
| Multiplicador de `Insight` L4 | 2 | 1-3 | El techo de éxitos de una sola tirada. En 3, una tirada con 3 `Wild` da 9 éxitos de la nada y supera la vida de cualquier enemigo del manual. |
| `Wild` convertibles en `Insight` L1 | 1 | 1-2 | Cuánto se siente el nivel 1 antes del salto a ilimitado. En 2, el nivel 2 casi no se nota y la rama pierde su decisión. |
| Esquives de `Stealth` L2 | 3 | 2-4 | Cuánto se puede escapar de un grupo. Interactúa con la cantidad de enemigos por espacio: si el promedio es 2, cualquier valor ≥ 2 ya es "esquive total" y el nivel 4 deja de significar algo. |
| Herida por esquive de `Stealth` L3 | 1 | 0-1 | La existencia de una ruta de daño **sin dados y sin `Toll`**. En 2 compite con `Attack` y le saca sentido a atacar; en 0 el nivel 3 queda vacío y `Stealth` pasa a tener 2 niveles útiles de 4. |
| Espacios extra de `Swiftness` L1 | 1 | 1-2 | La movilidad base. **Rompe el mapa** si el total de F6 supera el diámetro del grafo: ahí la posición deja de ser una decisión. |
| Acciones extra de `Swiftness` L4 | 1 | 0-1 | El output total del turno, ~+33%. **Rompe el reloj**: el `Doom Track` avanza por turno, no por acción, así que más acciones por turno es más juego por unidad de reloj. Ver [`../04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md). |
| Reducción de `Wounds` de `Toughness` L4 | 2 | 1-3 | Cuánto castigo aguanta un personaje parado en un espacio lleno. En 3, la mayoría de los ataques del manual hacen 0. |
| Reducción de `Ratchet` de `Toughness` L2-L4 | 1 | 0-1 | **La perilla más delicada del set.** En 0, `Toughness` deja de frenar la progresión y deja de ser una decisión interesante. En 2, un personaje con `Toughness` 4 casi no sube `Ratchet`, no cruza umbrales y nunca construye build: rompe [`trinquete.md`](trinquete.md) de raíz. |
| Estructura suma/reemplazo por nivel (F0) | Según manual | **No tocar.** Es la única fila de esta tabla sin rango seguro: cualquier cambio es rediseño de la skill, no balanceo | Es el ajuste más barato de hacer y el más peligroso. Volver `Toughness` sumativa acumula reroll gratis + reducción + alcance total y la convierte en la mejor skill del juego sin tocar un solo número. |
| Niveles que da un `Ally` | 1 | 1 | Si sube a 2, el `Ally` compite con el umbral como fuente de progresión, y el umbral es la **única** fuente que el diseño quiere tener. |
| Tope de nivel efectivo con `Ally` | 4 | 4 | Permitir 5 abre una celda 25 que no existe en el manual y obligaría a inventar 6 efectos nuevos. |

## ✅ Criterios de aceptación

1. Un personaje nuevo arranca con exactamente **3 skills, todas en nivel 1**: 1 de firma y 2
   del pool común de 6, sin repetir.
2. Cruzar un umbral de `Ratchet` sube **exactamente 1 nivel de 1 skill** elegida por el
   jugador. Ninguna otra fuente — matar enemigos, `Reward Deck`, cumplir el `Objective`,
   `Recover` — sube niveles.
3. Con una tirada que saca 2 `Hit` y 3 `Wild`, los éxitos totales son **3 con `Insight` 1, 5
   con `Insight` 2, 5 con `Insight` 3 y 8 con `Insight` 4**; con `Insight` 3 o 4 el personaje
   además recupera **3 de `Reserve`** (tope 4).
4. Subir `Toughness` de 1 a 2 deja los rerolls gratis de la skill en **0** y la reducción en
   **1 `Wound` y/o 1 `Ratchet` por fuente**. La confirmación de subida muestra explícitamente
   la habilidad que se apaga.
5. Con `Toughness` 4, un ataque que saca 2 `Hit` y 1 `Toll` deja al personaje con **+0
   `Wounds` y +0 `Ratchet`**. Si en la misma fase lo atacan 3 enemigos, la reducción se aplica
   **3 veces**, una por ataque.
6. Con `Brawling` 4, 2 éxitos y 3 objetivos en el espacio, cada objetivo recibe **2 heridas**
   —6 aplicadas en total— y el exceso sobre la vida de cada uno se descarta por separado.
7. Con `Brawling` 2 o más, la lista de objetivos queda cerrada antes de la tirada: la UI **no
   permite** agregar ni cambiar un objetivo después de tirar los dados.
8. Con `Stealth` 3 y 4 enemigos en el espacio de salida, gastar los 3 esquives deja **1
   enemigo siguiendo** y aplica **1 herida a cada uno de los 3 esquivados**, sin tirar ningún
   dado y sin generar ni 1 punto de `Ratchet`. Con `Stealth` 4 los esquives por `Move` no
   tienen tope y la herida por esquive **sigue aplicando**.
9. Con `Swiftness` 4 el personaje tiene **4 acciones más 1 `Move` gratis**, y el máximo de
   espacios que puede recorrer en un turno es **20**.
10. Con `Marksman` 3, un enemigo a **2** espacios por el camino más corto es objetivo válido,
    uno a **3** no lo es, y uno a 1 espacio marcado como inaccesible **no lo es** aunque la
    distancia alcance. Con `Marksman` 4, el ataque gratis se rechaza si el único objetivo
    disponible está en el espacio del personaje.
11. Perder un `Ally` que daba `Brawling` a un personaje **que no tenía `Brawling`** le quita
    la skill entera: vuelve a tener 3 skills, no queda con `Brawling` 1. Si el personaje
    tenía `Brawling` 4 antes del `Ally`, sigue en **4** después de perderlo.
12. Cruzar un umbral con las 3 skills en nivel 4 dispara la `Ratchet Card` y el dado bonus si
    corresponde, y registra la subida como **perdida**: el juego no bloquea el umbral ni
    guarda el nivel para después.
