# 📊 Barras y recursos

← [Índice](../README.md)

Cada `Character` tiene **tres barras** y **dos skills** (tres en el manual). Las skills están en
[`skills.md`](skills.md); acá van las barras.

## Las tres barras

| Barra | Se cura | Qué hace | Al llegar al final |
|---|---|---|---|
| `Wounds` | Sí, con `Recover` | Es el daño físico. Capacidad **4** | La 5.a herida mata |
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

## 📐 Fórmulas

Antes de las fórmulas, una aclaración de polaridad que hace falta para leer el manual sin
confundirse: **el manual lleva la cuenta al revés que este GDD.** En el manual el recurso de
reroll *sube* hasta un máximo de 4 y descansar lo baja; acá `Reserve` es el pool disponible y
`Recover` lo sube. La equivalencia exacta es `Reserve = 4 − stress_manual`. Los dos modelos
dicen lo mismo; el de acá se lee mejor en pantalla.

### 1. Rerolls disponibles

```
rerolls_pagos_disponibles = Reserve
```

- `Reserve` — puntos de reserva sin gastar. Rango **0–4** (el máximo de 4 sale del manual,
  pág. 8).
- Cada reroll cuesta exactamente 1 y rerollea exactamente 1 dado.
- Los **rerolls gratis** de skill (`Toughness` nivel 1, y las skills de firma que los den) no
  entran en esta cuenta y no consumen `Reserve`.

**Ejemplo:** personaje con `Reserve` 2 y `Toughness` nivel 1 recibe un ataque. Puede
rerollear hasta **3 dados** en esa tirada: 1 gratis por `Toughness` + 2 pagos. Si usa los
tres, queda con `Reserve` 0 y no puede rerollear nada más en el resto del turno hasta hacer
`Recover`.

### 2. Reparto de un `Recover`

Un `Recover` reparte **3 puntos totales** (manual, pág. 14) entre las dos barras
renovables. El jugador elige `w` (puntos a `Wounds`) y `r` (puntos a `Reserve`) sujeto a:

```
w + r ≤ 3
w ≤ Wounds                 # no podés curar heridas que no tenés
r ≤ 4 − Reserve            # no podés pasarte del techo de Reserve
```

- `w` — heridas curadas. Rango 0–3.
- `r` — reserva recuperada. Rango 0–3.
- **`Ratchet` no es una opción del reparto.** No aparece en la fórmula porque no existe el
  caso.

**Ejemplo (es el del manual, pág. 14, traducido al vocabulario de acá):** personaje con
`Wounds` 1 y `Reserve` 0 gasta una acción en `Recover`. Podría hacer `w=1, r=2`, pero elige
`w=0, r=3`. Queda con `Wounds` 1 y `Reserve` 3. Gastó los 3 puntos, ninguno se desperdició, y
las heridas siguen ahí.

### 3. Techo de curación por turno

```
curación_máxima_turno = min( 3 × acciones_gastadas_en_Recover ,  Wounds + (4 − Reserve) )
```

- `acciones_gastadas_en_Recover` — de 0 a `acciones_del_turno`. El default de acciones por
  turno es **3** (manual, pág. 13), y `Swiftness` nivel 4 da 1 acción extra.
- El segundo término es la **capacidad real de daño reparable** que tiene el personaje en ese
  momento. Curar por encima de eso no hace nada.

**Ejemplo:** personaje con `Wounds` 4 y `Reserve` 1, en un espacio `Clear`, sin `Swiftness`.
Primer término: 3 acciones × 3 = 9. Segundo término: 4 + (4 − 1) = 7. El techo es **7**: con
tres `Recover` queda en `Wounds` 0 y `Reserve` 4, y los últimos 2 puntos no tienen dónde ir.
Conclusión operativa: le alcanzaban 3 acciones, pero la tercera solo aprovecha 1 de sus 3
puntos.

### 4. Heridas efectivas de un golpe

```
Wounds_aplicadas  = max( 0 , Hits − red_W(nivel_Toughness, fuente) )
Wounds_personaje  = Wounds_aplicadas − desviadas_al_Ally
```

`red_W` sale del codex de skills del manual (pág. 25) y es **excluyente por nivel**: cada
nivel de `Toughness` reemplaza al anterior, no se suman.

| Nivel `Toughness` | `red_W` en ataque enemigo o `Hazard` | `red_W` en cualquier otra fuente |
|---|---|---|
| 1 | 0 (da 1 reroll gratis, no reducción) | 0 |
| 2 | 1 | 0 |
| 3 | 1 | 1 |
| 4 | 2 | 2 |

- `Hits` — caras `Hit` de la tirada del atacante, después de rerolls. Depende de cuántos dados
  tire el enemigo, que está en su carta.
- `desviadas_al_Ally` — de 0 a la capacidad libre del `Ally`. Sigue contando como que el
  personaje recibió el daño para todo efecto de carta (manual, pág. 16).

**Ejemplo:** un enemigo con 3 dados saca 3 `Hit`. El personaje tiene `Toughness` nivel 2 y un
`Ally` con capacidad 2 y 1 herida ya encima. `Wounds_aplicadas = 3 − 1 = 2`. Desvía 1 al
`Ally`, que llega a su número y **se descarta**; la segunda no puede ir ahí y va al
personaje. Resultado: `Wounds` del personaje +1, `Ally` fuera del juego, y cualquier carta que
lea "cuando recibís heridas" se dispara igual por las 2.

### 5. Procedimiento de decisión de `Recover`

Esto no es aritmética: es un algoritmo. El reparto óptimo no sale de una fórmula cerrada
porque depende de lo que el jugador planea hacer después. El orden de evaluación que el juego
tiene que respetar es este:

```
1. ¿El personaje está en un espacio Clear?   no → la acción Recover no existe, cortar acá
2. capacidad_W = Wounds
   capacidad_R = 4 − Reserve
3. si capacidad_W + capacidad_R == 0         → la acción es legal pero cura 0: avisar
4. ofrecer reparto libre con los topes de la fórmula 2
5. aplicar. Ratchet NO se toca en ningún caso
6. ¿sobran acciones y sobra capacidad?       sí → volver a 1 (Recover se repite en el turno)
```

**Ejemplo recorrido:** `Wounds` 2, `Reserve` 0, espacio `Clear`, 3 acciones libres.
Paso 2: `capacidad_W = 2`, `capacidad_R = 4`, total reparable 6. Primer `Recover`:
`w=2, r=1` → `Wounds` 0, `Reserve` 1. Segundo `Recover`: `w=0, r=3` → `Reserve` 4. Paso 6 con
1 acción libre y capacidad 0: el juego tiene que marcar el tercer `Recover` como inútil en vez
de dejar que se gaste la acción en silencio.

### Números que el manual NO da

- **La longitud del track de `Wounds`.** El manual solo dice que al llegar a la calavera del
  final el personaje muere (pág. 8); nunca dice cuántas casillas hay. Queda como perilla, más
  abajo.
- **La distribución de caras del dado.** El manual lista las 4 caras y aclara que algunas
  traen dos resultados, pero no dice cuántas caras de cada tipo tiene el dado. Cualquier
  cuenta de daño esperado por turno es especulación hasta que eso se decida, así que acá no
  hay ninguna.
- **La capacidad de heridas de cada `Ally`.** Está impresa en cada carta, no en el manual.
- **El precio en `Reserve` de cada `Reward Card`.** Ídem: es por carta.

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| `Reserve` en 0 y el jugador quiere rerollear | No puede. El manual lo pone como límite duro (pág. 11: se rerollea hasta que el recurso se agota). Los rerolls gratis de skill siguen funcionando. |
| `Recover` con `Wounds` 0 y `Reserve` 4 | La acción es legal —el manual dice "hasta 3", no "exactamente 3"— y cura 0. Se gasta la acción igual. El juego debe advertirlo antes de confirmar. |
| `Recover` con capacidad 1 y 3 puntos disponibles | Se cura 1. Los otros 2 se pierden: **no se guardan ni se arrastran al próximo `Recover`**. |
| `Recover` en un espacio con 1 o más enemigos | Ilegal. La acción no está disponible: el manual la restringe a espacios sin enemigos (pág. 14). |
| Un enemigo entra al espacio justo después de un `Recover` | El `Recover` ya resuelto no se revierte. La condición se chequea al usar la acción, no de forma continua. |
| Intentar curar `Ratchet` con `Recover` | Imposible por regla, no por falta de opción en la UI. El manual lo pone en mayúsculas (pág. 14). |
| Heridas desviadas a un `Ally` | Se marcan en el `Ally`, pero **cuentan como daño recibido por el personaje** para todo efecto de carta. Lo único que cambia es dónde se anotan. |
| El `Ally` llega a su número de heridas | Se descarta y se pierde de inmediato todo beneficio que daba. Las heridas que no entraron van al personaje. |
| El `Ally` daba un nivel de skill y se descarta | El marcador de esa skill **baja exactamente 1 nivel**. Si la skill existía solo por el `Ally`, el personaje la pierde entera (manual, pág. 16). |
| Más heridas que capacidad libre del `Ally` | El manual dice que las heridas "pueden aplicarse al `Ally`" pero **no explicita el reparto**. Decisión de este proyecto: se asigna herida por herida, y en cuanto el `Ally` alcanza su número se descarta y las restantes van al personaje. |
| Un efecto cobra `Reserve` y el personaje está en 0 | El efecto no se puede pagar. Solo se convierte en `Wounds` **si el efecto lo dice explícitamente** (manual, pág. 8: algunos efectos causan heridas en su lugar). Si no lo dice, la parte impagable se ignora. |
| `Reward Card` cuyo precio en `Reserve` no se puede pagar | No se reclama. Si ninguno de los dos lados de la carta se puede reclamar, la carta se descarta. |
| `Wounds` llega al final del track | El personaje muere. Si el adversario **todavía no se manifestó**, se pierde la partida entera en ese instante; si ya está en el tablero, los otros tres siguen jugando (manual, pág. 19). |
| El personaje muere durante su propio turno | Se saltean todas las fases restantes del turno **excepto** el chequeo del descarte del `Pressure Deck`, que se resuelve igual y puede hacer avanzar el reloj. Desde ahí en adelante ese turno se saltea entero. |
| Se acaban los tokens de herida | En el manual las heridas sobre el personaje son el único componente sin límite (pág. 20). En digital no hay tope de tokens: el único tope es la longitud del track. |
| Se acaban los tokens de `Hazard` del tablero | **No se emula el límite: no hay tope.** El manual limita los del tablero al stock físico (12 en esta caja) y ya no limita los que están sobre el personaje. Es una restricción de cartón, no de diseño, y replicarla en digital sería copiar un defecto. Consecuencia aceptada y anotada: **el juego queda más duro que la mesa** y esa dureza se compensa con las perillas de [07-balance](../07-balance/perillas-y-constantes.md), no reponiendo el tope. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Techo de `Reserve` | 4 (manual, pág. 8) | 3–6 | Cuánta varianza puede corregir el jugador entre dos curaciones. Subirlo aplana los dados y devalúa `Toughness` nivel 1 y las `Reward Card` que cobran `Reserve` como precio. |
| Puntos por `Recover` | 3 (manual, pág. 14) | 2–5 | Cuántos turnos de castigo hacen falta para volver a estar entero. Es la perilla más directa sobre el ritmo: bajarla obliga a encadenar turnos pasivos, subirla hace que el mapa deje de importar. |
| Acciones por turno | 3 (manual, pág. 13) | 3; 2–4 solo con mucho testeo | Toca todo el juego a la vez: cuántos `Recover`, ataques y movimientos entran en un turno. `Swiftness` nivel 4 ya regala +1, así que subirla duplica un efecto que ya existe y devalúa esa skill. |
| Reparto libre de los 3 puntos | Libre entre las dos barras | Libre, o con tope de 2 por barra | Flexibilidad táctica de la curación. Un tope por barra fuerza a repartir y hace que quedarse sin `Reserve` sea más caro de arreglar. |
| `Recover` exige espacio `Clear` | Sí (manual, pág. 14) | No tocar | Es lo que le da sentido al mapa entero. Si se saca, limpiar espacios deja de valer y `Stealth` pierde la mitad de su razón de ser. |
| Longitud del track de `Wounds` | **No está en el manual** | 5–8 casillas | Cuántos golpes aguanta un personaje antes de morir, y con eso cuán viable es la estrategia de sacrificio. Es la perilla más riesgosa: bajarla acerca la derrota temprana por muerte antes de que el adversario se manifieste. |
| Exceso de `Reserve` convertido en `Wounds` | Solo si la carta lo dice (manual, pág. 8) | No generalizar | Si se vuelve regla general, `Reserve` en 0 empieza a matar y el recurso táctico se convierte en un segundo track de daño. |
| Capacidad de heridas del `Ally` | Por carta, no en el manual | 1–3 | Cuánto daño se puede tercerizar. Por encima de 3 el `Ally` es un segundo track de `Wounds` gratis y desarma la tensión de la fórmula 4. |

## ✅ Criterios de aceptación

1. Con `Reserve` en 0, la UI no ofrece rerolls pagos y ninguna tirada se puede rerollear
   pagando; los rerolls gratis de skill siguen disponibles y dejan `Reserve` en 0.
2. `Recover` con `Wounds` 2 y `Reserve` 1 acepta el reparto `w=2, r=1` (total 3) y **rechaza**
   tanto `w=2, r=2` (total 4) como `w=3, r=0` (más heridas que las que tiene).
3. Reproducir el caso del manual: `Wounds` 1, `Reserve` 0, un `Recover` con `w=0, r=3` deja
   `Wounds` 1 y `Reserve` 3.
4. Tres `Recover` en el mismo turno son legales y curan como máximo 9 puntos en total, nunca
   más de 3 por acción.
5. Después de cualquier secuencia de `Recover`, el valor de `Ratchet` leído antes y después es
   **idéntico**.
6. `Recover` no aparece en la lista de acciones disponibles cuando hay 1 o más enemigos en el
   espacio del personaje, y sí aparece cuando hay 0.
7. Desviar 1 herida a un `Ally` deja `Wounds` del personaje sin cambio y **de todas formas**
   dispara los efectos de carta que leen "cuando este personaje recibe heridas".
8. Un `Ally` que otorgaba un nivel de skill, al descartarse por heridas, baja el marcador de
   esa skill exactamente 1 nivel, y la elimina si existía solo por ese `Ally`.
9. `Reserve` nunca supera 4 por ningún camino: `Recover`, efecto de carta o resolución de
   umbral. Un efecto que otorgue 2 con `Reserve` en 3 deja 4, no 5.
10. Con `Wounds` en la última casilla del track y el adversario **sin manifestarse**, la
    partida termina en derrota en ese mismo instante. Con el adversario ya en el tablero, la
    partida sigue y los otros 3 personajes conservan su estado completo.
11. `Toughness` nivel 2 reduce 1 herida en un ataque enemigo y en la resolución de `Hazard`, y
    reduce **0** en una pérdida causada por una `Pressure Card`. `Toughness` nivel 3 reduce 1
    en los tres casos.
12. Un personaje que muere en su propio turno saltea las fases restantes del turno salvo el
    chequeo del descarte del `Pressure Deck`, que queda registrado en el log igual.
