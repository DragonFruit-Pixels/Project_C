# 👑 Objetivo y adversario

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

---

## 📐 Fórmulas

Las cuatro primeras son aritmética real de la pelea. La quinta y la sexta no son
matemática: son el procedimiento de decisión que el motor tiene que ejecutar, y van con el
mismo detalle.

### F1 — Derrota de un stage

```
stage_defeated(s)  =  W_s >= HP_s
```

| Variable | Qué es | Rango |
|---|---|---|
| `W_s` | Heridas acumuladas **en la Stage card `s`** | 0 … ∞ (entero) |
| `HP_s` | Vida impresa en la Stage card `s` | **No está en el manual.** El único valor concreto que aparece es **12**, en el ejemplo del Stage 2 de un adversario del juego original |

La comparación es `>=`, no `==`: no hace falta clavar el número exacto.

**Ejemplo resuelto** (es el del manual, con vocabulario neutro): el Stage 2 tiene `HP_s` =
12 y ya venía con `W_s` = 9 de ataques anteriores. El personaje ataca y saca **5 `Hit`**.
`W_s` = 9 + 5 = 14. Como 14 >= 12, el stage cae.

### F2 — Daño útil y daño desperdiciado

El daño sobrante no pasa al siguiente stage, así que cada ataque tiene una parte que sirve
y otra que se tira.

```
util        = min(H, HP_s - W_prev)
desperdicio = max(0, H - (HP_s - W_prev))
```

| Variable | Qué es | Rango |
|---|---|---|
| `H` | `Hit` de la tirada, después de rerolls y conversiones | 0 … (3 estándar + dados bonus) |
| `W_prev` | Heridas que el stage ya tenía antes de este ataque | 0 … `HP_s` − 1 |

**Ejemplo resuelto:** `W_prev` = 9, `HP_s` = 12, `H` = 5.
`util` = min(5, 3) = **3**. `desperdicio` = max(0, 5 − 3) = **2**.
Los 2 se pierden: el Stage 3 arranca en 0 heridas.

Corolario que conviene tener presente al balancear: el desperdicio esperado crece con la
cantidad de dados. Un personaje muy avanzado en `Ratchet`, que saca 6 o 7 `Hit` por ataque,
tira más daño a la basura en el golpe de gracia que uno limpio.

### F3 — Pool de dados del adversario

```
std_total(t)   = Σ  std_s      para todo s revelado en el momento t
bonus_total(t) = Σ  bonus_s    para todo s revelado en el momento t
```

| Variable | Qué es | Rango |
|---|---|---|
| `std_s` | Dados estándar que suma la Stage card `s` | **No está en el manual** |
| `bonus_s` | Dados bonus que suma la Stage card `s` | **No está en el manual** |
| `t` | Momento de la partida | — |

Las dos funciones son **monótonas crecientes**: nunca bajan, porque las Stage cards
derrotadas siguen contando. El manual es explícito: los dados de los stages anteriores
quedan en juego hasta el final.

**Ejemplo resuelto:** el manual documenta un solo total concreto. Con el Stage 3 recién
revelado —o sea, con Stage 1, 2 y 3 revelados— ese adversario tira **3 dados bonus + 2
dados estándar** = 5 dados por ataque. De ahí se deduce que el Stage 1 aporta 0 dados,
coherente con que el manual diga que la vida y los dados **aparecen recién a partir del
Stage 2**.

### F4 — Heridas esperadas por ataque del adversario

```
E[Wounds] = std_total * p_hit_std + bonus_total * p_hit_bonus
E[Toll]   = std_total * p_toll_std                    (el dado bonus no tiene cara Toll)
```

| Variable | Qué es | Rango |
|---|---|---|
| `p_hit_std` | Probabilidad de `Hit` en un dado estándar | **No está en el manual.** La transcripción lista las 4 caras y avisa que algunas traen 2 resultados, pero **no da la distribución de caras de ningún dado** |
| `p_hit_bonus` | Probabilidad de `Hit` en un dado bonus | **No está en el manual** |
| `p_toll_std` | Probabilidad de `Toll` en un dado estándar | **No está en el manual** |

**Ejemplo resuelto, con el supuesto marcado:** tomando el pool documentado en F3 —2
estándar + 3 bonus— y con la **distribución provisional declarada** en
[`07-balance`](../07-balance/perillas-y-constantes.md) (`p(Hit)` = 0,50, `p(Toll)` = 0,33,
valores de trabajo, **no** del manual), el ataque pega
`E[Wounds]` = 2·0,50 + 3·0,50 = **2,50 heridas por ataque**.
Cobra además `E[Toll]` = 2·0,33 = **0,67 puntos de `Ratchet`** — solo los 2 dados estándar
pueden sacar `Toll`.

Dos cosas que no hay que olvidar al implementar esto: los dados del adversario **los tira el
jugador** y puede rerollearlos gastando `Reserve`, y los dados bonus permanentes del
personaje **no se suman**, porque es una tirada contra sí mismo.
→ [`../03-resolucion/dados.md`](../03-resolucion/dados.md)

### F5 — Vida total de la pelea y turnos estimados

```
HP_fight     = HP_2 + HP_3 + HP_final
turnos_s     = ceil( HP_s / (a * h) )
turnos_total = Σ turnos_s
```

| Variable | Qué es | Rango |
|---|---|---|
| `HP_2`, `HP_3`, `HP_final` | Vida de los tres stages atacables | **No están en el manual** |
| `a` | Acciones `Attack` que se meten por turno contra el adversario | 0 … 3 por personaje |
| `h` | `Hit` promedio por ataque | ≈ 1 a 5 según build |

El Stage 1 no entra en la suma: no tiene vida.

**Ejemplo resuelto:** con los tres stages en 12 de vida —extrapolando el único número del
manual—, `HP_fight` = **36**. Un personaje que dedica sus 3 acciones a atacar y saca 1,5
`Hit` por ataque hace 4,5 heridas por turno: `turnos_s` = ceil(12 / 4,5) = **3 turnos por
stage**, o sea **9 turnos** para toda la pelea con un solo personaje empujando. Ese número
hay que compararlo contra los espacios que le queden al `Doom Track`, porque el reloj no se
detiene durante la pelea.
→ [`reloj-y-avance.md`](reloj-y-avance.md)

### F6 — Procedimiento: los dos flags del adversario

No hay un solo estado "el adversario está activo". Hay **dos booleanos independientes**, y
casi todos los malentendidos de esta parte del juego salen de confundirlos.

| Flag | Qué habilita | Cómo se prende |
|---|---|---|
| `manifested` | La figura está en el mapa: sigue a los personajes, ataca en la fase de `Reckoning`, suma dados | El marcador entra al **primer espacio marcado** del `Doom Track` **o** se rompe el `Objective` |
| `mortal` | Se puede **elegir como objetivo** de `Attack` y acumular heridas | **Solo** romper el `Objective` |

Los dos son irreversibles: una vez prendidos, no se apagan.

**Entradas:** estado del `Objective`, posición del marcador en el `Doom Track`.
**Salidas:** `manifested`, `mortal`, cantidad de Stage cards reveladas.
**Orden de evaluación** — todo esto vive en el **paso 4 de la fase 4** del turno, después de
chequear el descarte y antes de los efectos de fin de turno del adversario:

```
1. si mortal == false y el Objective se rompió en este turno:
       mortal = true
2. si manifested == false y (mortal == true o el marcador entró al espacio marcado):
       manifested = true
       correr la Stage 1 al costado
       revelar la Stage 2 y resolver su efecto de revelación  (dice dónde aparece la figura)
       poner el marcador de progreso en el espacio que ocupaba la figura
3. resolver los efectos de fin de turno de todas las Stage cards reveladas,
   en orden 1 -> 2 -> 3 -> final
```

El paso 2 corre **una sola vez en toda la partida**, aunque los dos disparadores caigan
juntos.

**Ejemplo recorrido paso a paso.** Turno 7, el `Objective` está intacto y el marcador está a
un espacio del marcado.

1. Fase 2: la carta de presión deja el tercer `Doom Symbol` en el descarte.
2. Fase 4, paso 3: hay 3 `Doom Symbol` → el adversario avanza 1 espacio y **entra al espacio
   marcado**. Se resuelven los efectos del avance y se remezcla el descarte.
3. Fase 4, paso 4, chequeo 1: el `Objective` sigue intacto → `mortal` queda en **false**.
4. Fase 4, paso 4, chequeo 2: `manifested` era false y el marcador entró al espacio marcado →
   `manifested` = **true**. Se corre la Stage 1, se revela la Stage 2, su efecto de
   revelación pone la figura en el mapa, y el marcador de progreso ocupa el lugar de la
   figura en el track.
5. Fase 4, paso 5: se resuelven los efectos de fin de turno de Stage 1 y Stage 2.

Resultado: el adversario está en el mapa, pega con los dados de Stage 1 + Stage 2, y **no se
puede atacar**. Ese es el peor escenario de la mitad 1, y es completamente legal.

---

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| El adversario está en el mapa, el `Objective` está intacto, y el jugador intenta atacarlo | La acción **no es legal**: no se puede elegir como objetivo. Cubierto por el manual, que lo repite tres veces ("even if it's been summoned to the board"). En digital: la figura no se ofrece como target. No es tirar y que no pase nada |
| Un ataque hace más heridas de las que le quedan al stage | El stage cae y **el sobrante se descarta**. 9 + 5 contra 12 de vida derrota el stage y **pierde 2 heridas**. El stage siguiente arranca en 0 |
| Un ataque enorme —10, 15 `Hit`— contra un stage con poca vida | Cae **un solo stage**. Nunca se pueden derrotar dos stages con la misma tirada, porque el excedente no viaja. Es un tope duro al burst |
| Se rompe el `Objective` y en el mismo turno el marcador entra al espacio marcado | **Una sola manifestación**: se revela la Stage 2 una vez. `mortal` y `manifested` quedan los dos en true. No se saltea ningún stage |
| Se rompe el `Objective` a mitad de la fase 1 y el adversario todavía no está en el mapa | Queda `mortal` = true pero sin figura en el mapa: **no hay nada para atacar** en lo que resta del turno. La manifestación se resuelve en el paso 4 de la fase 4 y la pelea real empieza el turno siguiente |
| El marcador entra al espacio marcado **después** de que el adversario ya se manifestó por objetivo | No hay segunda manifestación: lo que avanza es el marcador de progreso y solo se resuelven los efectos normales del avance. **El manual no lo dice de forma explícita** —se deduce de que el chequeo de manifestación ya está resuelto—, así que conviene dejarlo escrito en el código |
| Atacar al adversario mientras la Stage 1 está arriba | Imposible por dos razones a la vez: con Stage 1 arriba el adversario no se manifestó, y **el manual dice que la vida y los dados aparecen recién desde el Stage 2**. La Stage 1 no tiene vida: es solo efecto permanente. Ojo con esto al leer "4 stages con vida propia" — **hay 3 stages atacables**, no 4 |
| El efecto de revelación de un stage mueve la figura fuera del espacio del personaje activo | Se resuelve en el momento, durante la fase 1. Si el espacio queda `Clear`, en la fase 3 el personaje **roba recompensa**, porque lo que se mide es el estado del espacio al empezar la fase. Matar un stage puede sacarte al jefe de encima **y** darte carta el mismo turno |
| El efecto de revelación de un stage lastima al personaje activo y lo mata | Se aplica igual: primero se resuelve la revelación completa, después la muerte. Si el adversario ya estaba manifestado, la partida sigue mientras quede alguien vivo → [`../05-partida/victoria-y-derrota.md`](../05-partida/victoria-y-derrota.md) |
| Cae el stage **final** | **Victoria inmediata**, sin importar el excedente ni qué más se disparó en ese instante. La victoria tiene precedencia sobre cualquier condición de derrota simultánea |
| Heridas ya puestas sobre un stage derrotado | Se van con la carta al costado. **No se reciclan, no se transfieren, no se cuentan de nuevo.** Cada Stage card lleva su contador propio desde 0 |
| El adversario está en el mapa y el personaje se va del espacio | El adversario **lo sigue**, como cualquier enemigo, y el personaje agarra el `Hazard` del espacio que deja. No hay desengage salvo con `Stealth` → [`enemigos.md`](enemigos.md) |
| Dos efectos permanentes de stages distintos se contradicen | **Gana el stage de número más alto**, y si eso no alcanza para desempatar, elige el jugador activo. El manual no resuelve conflictos entre stages —solo trae la regla general de que ante varias formas de resolver algo elige el jugador activo— así que esto es una decisión del GDD. Se elige el número más alto porque los stages se revelan en orden creciente y el más nuevo es el que describe el estado actual del adversario |
| Un efecto de carta le pone heridas al adversario sin ser una acción `Attack`, con el `Objective` intacto | **Caso no cubierto por el manual.** La regla dice que no puede ser atacado **ni dañado**, así que la lectura recta es que el daño se descarta. Hay que decidirlo y escribirlo, porque condiciona el diseño de las 15 cartas de recompensa |

---

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Vida de cada stage atacable | **No está en el manual.** Único dato: 12 para un Stage 2 | 8 – 20 por stage | El largo de la mitad 2 en turnos. Es la perilla que más pelea con el `Doom Track`: subir vida sin alargar el track hace que la pelea no se pueda terminar a tiempo → [`reloj-y-avance.md`](reloj-y-avance.md) |
| Dados que suma cada stage | **No está en el manual.** Único dato: 3 bonus + 2 estándar con tres stages revelados | +0 a +2 dados por stage | Cuánto castiga quedarse en el espacio del adversario. Como los dados se acumulan, cada punto acá se paga tres veces: la presión de la última parte de la pelea sube de forma no lineal |
| Cantidad de stages | **4** (3 con vida) | 3 – 5 | La cantidad de picos dramáticos y de reposicionamientos de la figura. Bajar a 3 mata uno de los cuatro momentos de cámara; subir a 5 exige rebalancear vida por stage para no alargar la pelea |
| El excedente pasa al stage siguiente | **OFF** (no pasa) | ON / OFF | Prenderlo premia el burst y acorta la pelea mucho más de lo que parece. Rompe además el ritmo de "cada revelación es un momento": con overflow, dos stages pueden caer en la misma tirada |
| Los efectos y dados previos quedan en juego | **ON** | ON / OFF | Es **la** regla que invierte la curva de tensión del boss fight. Apagarla convierte la pelea en una barra de vida normal y deja sin sentido la decisión de "aguanto lo que viene o no". No es una perilla de tuning: es la identidad del sistema |
| El adversario es invulnerable antes de romper el `Objective` | **ON** | ON / OFF | Es la bisagra entre las dos mitades. Apagarla borra la mitad 1: el jugador puede ignorar el `Objective` y pegarle desde el turno 1. Rompe también las 2 acciones propias de la misión, que existen para servir al objetivo |
| Posición del primer espacio marcado del `Doom Track` | **No está numerada en el manual** (dice solo "primer espacio marcado") | Espacio 3 a 5 de 8 | Cuánto dura el peor escenario: jefe en el mapa y todavía intocable. Más temprano = más turnos comiendo golpes sin poder responder |
| Acciones propias de la misión | **2** | 2 – 3 | Cuánto ayuda el escenario a romper el objetivo. Subirla acelera la mitad 1 y adelanta la manifestación, lo que a su vez adelanta el acceso al sacrificio → [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md) |

---

## ✅ Criterios de aceptación

1. Con el `Objective` intacto y el adversario en el mapa, seleccionar `Attack` **no ofrece la
   figura del adversario como objetivo válido**, y el contador de heridas del stage sigue en
   0 después del intento.
2. Con el `Objective` roto, un stage de 12 de vida con 9 heridas y un ataque de 5 `Hit` queda
   **derrotado**, el stage siguiente arranca en **0 heridas**, y el log registra **2 heridas
   descartadas**.
3. Un ataque de 15 `Hit` contra un stage de 12 de vida con 0 heridas derrota **exactamente 1
   stage**, y el siguiente queda revelado con 0 heridas.
4. Cuando el marcador entra al primer espacio marcado con el `Objective` intacto, al cerrar el
   turno el adversario está en el mapa, `mortal` = false, y hay **2 Stage cards reveladas**.
5. Cuando el `Objective` se rompe en el mismo turno en que el marcador entra al espacio
   marcado, al cerrar el turno hay **exactamente 2 Stage cards reveladas** (no 3) y la
   manifestación quedó registrada **una sola vez**.
6. Con Stage 1, 2 y 3 reveladas y los valores de ejemplo del manual, el ataque del adversario
   tira **5 dados: 3 bonus + 2 estándar**, y ninguno de los bonus muestra cara `Toll`.
7. Después de derrotar un stage, el efecto permanente del stage derrotado **sigue aplicando**
   y sus dados **siguen sumando** en el ataque siguiente.
8. En una tirada de ataque del adversario contra un personaje con 2 dados bonus permanentes de
   `Ratchet`, la cantidad de dados tirados **no incluye esos 2**.
9. Derrotar el stage final termina la partida en **victoria**, aunque en el mismo instante el
   último personaje quede eliminado.
10. Si el efecto de revelación de un stage mueve la figura fuera del espacio del personaje
    activo y no queda ningún otro enemigo ahí, la fase 3 de ese turno **roba una carta del
    `Reward Deck`** y no se resuelve ningún ataque.
11. El contador de heridas de un stage derrotado queda congelado con la carta al costado: no
    aparece sumado en ningún otro stage en ningún momento posterior.
12. Los flags `manifested` y `mortal` nunca vuelven a false en toda la partida, ni por efecto
    de carta, ni por muerte de personaje, ni por avance del reloj.
