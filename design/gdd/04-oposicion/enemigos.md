# ⚔️ Enemigos

← [Índice](../README.md)

## Taxonomía

Tres categorías, y la distinción importa porque muchos efectos apuntan a una sola.

| ID | Qué es |
|---|---|
| `Servant` | Enemigo humano, ligado al objetivo. Pool limitado a **10** |
| `Creature` | Todo enemigo que no sea `Servant` ni el `Adversary` |
| `Adversary` | El jefe. Solo cuenta como enemigo una vez manifestado |

La palabra paraguas es **enemigo**: cubre las tres. Un efecto que dice "enemigos" toca a
las tres; uno que dice `Creature` no toca `Servant`.

## Datos de cada enemigo

Cada tipo tiene:

- **Vida** — cuántas heridas aguanta antes de morir
- **Dados de ataque** — cuántos estándar y cuántos bonus tira
- **Habilidad especial** — condicional, se dispara en cierta circunstancia
- **Tamaño del pool** — cuántas figuras hay disponibles en total

Las que no están en el mapa quedan en un pool, listas para spawnear.

## Comportamiento

Los enemigos de este juego **no piensan**. Su comportamiento entero son tres reglas:

**1. Te siguen.** Cuando un personaje sale de un espacio, **todos** los enemigos que
estaban ahí se mueven con él. Todos, aunque queden otros personajes en el espacio original.

**2. Te atacan.** En la fase de referéndum, si comparten espacio con el personaje activo,
cada uno lo ataca una vez.

**3. Los mueven las cartas.** Cualquier reposicionamiento que no sea "seguir" viene de un
efecto de carta, que los mueve por el camino más corto hacia un objetivo.

Eso es todo. No patrullan, no buscan, no coordinan.

### Por qué eso es una buena noticia

**Las reglas base no necesitan IA.** Se pueden implementar con lógica directa, y funcionan
igual de bien que con un sistema complejo, porque el diseño no pide más.

La IA real —NavMesh, Behaviour Tree, EQS, Percepción— entra después, reemplazando ese
scripting sin cambiar ninguna regla. Eso permite tener el juego jugable antes y agregar la
capa de IA como una mejora medible, no como un requisito de arranque.

## La regla de "te siguen"

Es la regla espacial más importante del juego, y merece atención.

**No hay desengage.** No podés soltar a un enemigo moviéndote. Si entrás a un espacio con
tres criaturas, esas tres son tuyas hasta que las mates o uses `Stealth`.

Las consecuencias en cadena:

- Un espacio con enemigos es un espacio donde no podés `Recover` ni robar recompensa.
- Moverte no te salva: te llevás el problema puesto.
- Juntar enemigos es fácil y desjuntarlos es caro. Un personaje puede terminar arrastrando
  media pantalla.
- Y como al moverte también agarrás `Hazard`, escapar cuesta doble.

Por eso `Stealth` —la única skill que te deja dejar enemigos atrás— es más valiosa de lo
que parece a primera vista.

## Spawn

Los enemigos aparecen en las **3 `Gate`** del mapa, invocados por la tercera parte de las
cartas de presión.

Reglas:

- La carta dice **qué `Gate`** y **qué enemigo**.
- Si **no hay figura disponible** en el pool, ese spawn **se saltea**. No se sustituye por
  otra cosa.
- Si hay varios spawns en una carta, se hacen **en orden**.
- Si la carta invoca varios sin especificar orden, elige el jugador. Eso importa justo
  cuando no alcanzan las figuras.

## Límites de pool

En el juego de mesa los componentes son finitos, y eso funciona como un **tope blando de
dificultad**: cuando ya hay 10 `Servant` en el tablero, no pueden aparecer más.

**En digital ese límite deja de ser físico.** Se puede replicar, subir o sacar. Sacarlo hace
el juego más duro, y hay que re-balancear.

Qué reemplaza ese límite sigue abierto.
→ [`06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)

## Muerte

Cuando las heridas acumuladas igualan o superan la vida, el enemigo muere y la figura
**vuelve al pool**, disponible para spawnear de nuevo.

El daño sobrante **no se transfiere** a otro enemigo, salvo con `Brawling` de nivel
suficiente.

---

## 📐 Fórmulas

Los enemigos no tienen IA, pero sí tienen contabilidad: pools, heridas, dados y conjuntos de
figuras que se arrastran. Todo lo de abajo es determinista.

Notación:

| Símbolo | Qué es | Rango |
|---|---|---|
| `H(e)` | Vida del enemigo | `Servant` = **2** y un tipo de `Creature` = **3** (los únicos dos valores visibles en el manual, p. 14). **La tabla completa de vidas no está en la transcripción**: sale de las cartas de enemigo |
| `W(e)` | Heridas acumuladas en esa figura | `0 .. H(e)` |
| `Pool(t)` | Figuras totales del tipo `t` | `Servant` = **10** (manual p. 20). `Creature`: **18 figuras en 7 tipos** — 2 tipos de 5, 3 tipos de 2, 2 tipos de 1 (manual p. 3). Minions del adversario: **2 o 4** según cuál sea (manual p. 3) |
| `EnMapa(t)` | Figuras de ese tipo en el tablero | `0 .. Pool(t)` |
| `std_e`, `bonus_e` | Dados de ataque de la figura | El único bloque visible en el manual es **1 estándar + 2 bonus** (p. 17). El resto sale de las cartas |
| `p(Hit)`, `p(Toll)` | Probabilidad por cara | **No está en el manual**: la transcripción lista las 4 caras pero no la distribución del dado, y avisa que algunas caras traen 2 resultados juntos (p. 11). Hay que medirla del dado real y ponerla en Data Table |

---

### F1 — Clasificación y alcance de un efecto

```
Enemigo = Servant  ∪  Creature  ∪  { Adversary : si está manifestado en el mapa }

Alcance("enemigo")  = las tres categorías
Alcance("Servant")  = solo Servant
Alcance("Creature") = solo Creature
Alcance("Adversary")= solo el Adversary
```

Es una jerarquía de una sola capa y **no hay herencia hacia arriba**: `Creature` no incluye
`Servant`, y "enemigo" incluye a los tres (manual p. 7).

**Ejemplo.** En un espacio hay 2 `Servant` y 1 `Creature`. Una carta dice "1 herida a cada
`Creature` de tu espacio" → se aplica **1 sola herida, a una sola figura**. La misma carta
diciendo "enemigo" aplicaría 3 heridas a 3 figuras.

---

### F2 — Muerte y daño desperdiciado

```
Muerto(e)     ⇔  W(e) >= H(e)
Desperdicio   =  max(0, Hits − (H(e) − W(e)))
```

Rango de `Desperdicio`: `0 .. Hits − 1`. Al morir, la figura se remueve del tablero y
**vuelve al pool** (manual p. 14 y p. 20).

**Ejemplo.** `Creature` con `H = 3` que ya tiene `W = 1`. El personaje saca 3 `Hit`.
`W = 4 >= 3` → muere. `Desperdicio = max(0, 3 − 2) = 1`: esa herida **se pierde**.

Con `Brawling` nivel 2 o más, esa herida se puede asignar a otro enemigo del mismo espacio,
pero los objetivos hay que declararlos **antes de tirar** (manual p. 24). Sin la skill, el
objetivo también se declara antes y el sobrante se pierde igual.

---

### F3 — Disponibilidad de pool y decisión de spawn

```
Disponibles(t) = Pool(t) − EnMapa(t)

Spawn(t, gate):
    si Disponibles(t) >= 1  →  colocar 1 figura de t en gate
    si no                   →  saltear este paso, sin sustituir por otra cosa
```

Rango de `Disponibles`: `0 .. Pool(t)`. Cuando llega a 0, la `Gate` está viva pero muda.

**Ejemplo.** Un tipo con `Pool = 5` tiene 4 figuras en el mapa. La carta de presión dice
"invocá 1 en la `Gate` amarilla y 1 en la `Gate` azul", en ese orden.

1. Amarilla: `Disponibles = 1` → se coloca. `EnMapa = 5`.
2. Azul: `Disponibles = 0` → **se saltea**. No se pone otro tipo de enemigo, no queda
   pendiente, no se compensa después.

Resultado: 5 figuras de ese tipo en el mapa y una `Gate` que no produjo nada.

---

### F4 — El pool es un ciclo, no un consumible

```
al morir e de tipo t:  EnMapa(t) −= 1  →  Disponibles(t) += 1  (inmediato)
```

**Matar re-arma la `Gate`.** No hay cooldown ni cola: la figura está disponible en el mismo
turno en que murió.

**Ejemplo.** Tipo con `Pool = 2`, las 2 en el mapa → `Disponibles = 0`. En la fase 1 el
personaje mata una. En la fase 2, la carta de presión invoca ese tipo: `Disponibles = 1` →
**aparece**. El jugador limpió su espacio y con eso habilitó un spawn en la otra punta del
mapa. Es contraintuitivo y es la regla.

---

### F5 — La regla de "te siguen", como conjunto acumulado

```
Seguidores(s_i)  = Enemigos(s_i)  menos  los que se hayan esquivado al salir de s_i
Acompañantes(k)  = unión de Seguidores(s_0 .. s_k−1), descontando los que murieron en camino

SneakBudget por acción Move, según Stealth (manual p. 24):
    sin skill = 0   |   nivel 1 = 1   |   nivel 2 = 3   |   nivel 3 = 3   |   nivel 4 = sin límite
```

Un esquive gasta 1 del budget, deja **1** enemigo (elección del jugador) en el espacio que se
abandona, y con `Stealth` nivel 3+ le hace **1 herida**. Esquivar dos veces al mismo enemigo
—volviendo al espacio y saliendo otra vez— gasta **2** del budget.

**Ejemplo sin `Stealth`.** El personaje arranca en `S1` con 2 enemigos y hace un `Move` de 3
pasos: `S2` tiene 1 enemigo, `S3` tiene 3, `S4` está vacío.

| Paso | Sale de | Lo siguen | Llega a | Enemigos en su espacio |
|---|---|---|---|---|
| — | — | — | `S1` | 2 |
| 1 | `S1` | 2 | `S2` | 2 + 1 = **3** |
| 2 | `S2` | 3 | `S3` | 3 + 3 = **6** |
| 3 | `S3` | 6 | `S4` | **6** |

Termina en un espacio que estaba vacío, con 6 enemigos encima, y en la fase 3 los 6 lo atacan.

**El mismo recorrido con `Stealth` nivel 2** (3 esquives), gastados los tres en la última
salida: llega a `S4` con **3** enemigos y deja **3** en `S3`. Con nivel 3, esos 3 esquivados
además reciben 1 herida cada uno.

---

### F6 — Daño esperado de la fase 3

```
DadosTotales   = suma de (std_e + bonus_e) sobre los enemigos presentes al momento de atacar
DadosConToll   = suma de std_e                      (los dados bonus no tienen cara Toll)
HeridasEsper.  = DadosTotales × p(Hit)
RatchetEsper.  = DadosConToll × p(Toll)
```

**Ejemplo.** 3 enemigos del único bloque conocido del manual, 1 estándar + 2 bonus cada uno:
`DadosTotales = 9`, `DadosConToll = 3`. Con la **distribución provisional declarada** en
[`07-balance`](../07-balance/perillas-y-constantes.md), `p(Hit) = 0,50` —**el valor real no
está en el manual**— eso son 3 heridas esperadas por turno de quedarse sucio, y solo 1
`Toll` esperado.

Consecuencia de diseño: **cargar el ataque enemigo en dados bonus pega fuerte y casi no mueve
el trinquete**; cargarlo en dados estándar te empuja al final del track. Son dos perillas
distintas disfrazadas de una →
[`03-resolucion/dados.md`](../03-resolucion/dados.md)

Los dados enemigos **los tira el jugador**, y los dados bonus permanentes del `Ratchet` no se
suman a estas tiradas porque son tiradas contra vos mismo.

---

### F7 — Procedimiento de un spawn múltiple (orden y salteo)

Entrada: lista de pasos `(tipo, gate)` de la carta. Salida: figuras colocadas.

1. Si la carta **fija un orden**, se usa ese. Si invoca varios "a la vez" sin orden, **el
   orden lo elige el jugador activo** (manual p. 15).
2. Para cada paso, en orden: si `Disponibles(tipo) >= 1`, colocar; si no, saltear ese paso.
3. Seguir con el paso siguiente. Un salteo **no aborta** la carta.

**Ejemplo recorrido.** Carta: "invocá 1 `Servant` en cada `Gate`". `Disponibles(Servant) = 2`,
y hay 3 `Gate`.

- El jugador elige el orden: azul, roja, amarilla.
- Azul: `Disponibles = 2` → coloca. Queda 1.
- Roja: `Disponibles = 1` → coloca. Queda 0.
- Amarilla: `Disponibles = 0` → **se saltea**.

**El jugador acaba de decidir qué `Gate` no escupe nada**, y eso es una decisión real: elige
que la puerta vacía sea la que le queda cerca. El manual marca explícitamente que este caso
es "particularmente relevante" cuando no alcanzan las figuras.

---

### F8 — Techo duro de presión de la misión

```
EnemigosMax = Pool(Servant) + Σ Pool(t) de los tipos de Creature que use la misión + minions
```

**Ejemplo.** Misión con `Servant` (10), dos tipos de `Creature` de pool 5 y 2, y un adversario
con 2 minions: `10 + 5 + 2 + 2 = 19` figuras. **Nunca puede haber 20 enemigos en el mapa**, no
importa cuántas cartas de spawn salgan.

Ese número es el tope blando de dificultad de la misión, y es exactamente lo que la decisión
A-04 pone en discusión: si el límite se saca, `EnemigosMax` pasa a ser infinito y el spawn
nunca se saltea → [`../06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)

---

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| Spawn con el pool en 0 | El paso **se saltea**. No se sustituye por otro tipo de enemigo, no queda encolado, no se compensa más adelante (manual p. 15) |
| Carta con varios spawns y pool para algunos | Se resuelven en orden hasta agotar figuras; los pasos restantes se saltean uno por uno. La carta termina de resolverse igual |
| Carta que invoca varios sin especificar orden | El **jugador activo** elige el orden, y con eso elige qué `Gate` queda sin figura (manual p. 15) |
| Un enemigo muere en la fase 1 y la carta de la fase 2 invoca ese tipo | La figura ya está de vuelta en el pool → **aparece**. Limpiar tu espacio puede habilitar un spawn en la otra punta del mapa (manual p. 20) |
| Daño sobrante al matar | **Se pierde.** Sin `Brawling` nivel 2+, ni siquiera se puede repartir, y el objetivo se declara antes de tirar (manual p. 14 y p. 24) |
| Heridas de una figura que muere | Vuelven al pool de tokens junto con la figura. Una figura re-invocada entra con **0 heridas**: el manual no lo dice con esas palabras, pero se deduce de que los componentes removidos vuelven al pool (p. 20) |
| No quedan tokens de herida | No puede pasar: los tokens de herida y los dados son **la única excepción** al límite de componentes (manual p. 20). Aplicar daño nunca se saltea por falta de tokens |
| Personaje sale de un espacio con 3 enemigos y otro personaje se queda ahí | Los **3** se van con el que se movió. El que se queda termina con 0 enemigos. Sí, es abusable a propósito (manual p. 13) |
| Movimiento fuera de una acción `Move` | Los enemigos **siguen igual**, y el personaje también agarra `Hazard`. Es el default, no la excepción (manual p. 13) |
| Un efecto que **coloca** al personaje en otro espacio | **Nadie lo sigue** y no agarra `Hazard`. Es la única excepción del manual (p. 13). Tampoco consume esquives de `Stealth` |
| `Stealth` fuera de una acción `Move` | No se puede usar: los enemigos siguen como siempre (manual p. 24) |
| Esquivar al mismo enemigo dos veces | Legal si volvés al espacio y salís de nuevo, y **gasta 2** del budget (manual p. 24) |
| Un esquive con `Stealth` nivel 3+ que mata al enemigo | La herida se aplica, el enemigo muere, la figura vuelve al pool y **no** queda en el espacio abandonado |
| Enemigo que sale de un espacio con `Hazard` | No se lleva nada. **Los enemigos no agarran `Hazard`** (manual p. 17) |
| Un enemigo entra a tu espacio a mitad de la fase 3 | **Ataca**, aunque no estuviera al empezar la fase (manual p. 16) |
| Un enemigo se va de tu espacio a mitad de la fase 3 | **No ataca** más, incluso si le tocaba (manual p. 16) |
| El espacio queda limpio a mitad de la fase 3 | Los ataques se detienen, pero **no se roba `Reward`**: cuenta el estado al empezar la fase (manual p. 16) |
| Una carta manda mover enemigos de un tipo que no hay en el mapa | Ese paso se saltea y la carta sigue (manual p. 15) |
| `Adversary` todavía en el `Doom Track` | **No es enemigo**: no ataca, no cuenta para `Clear`, no lo tocan los efectos que dicen "enemigo" (manual p. 7) |
| `Adversary` en el mapa antes de romper el `Objective` | Ataca a quien termine el turno en su espacio, pero **no se lo puede atacar ni dañar** (manual p. 19) |
| ¿El `Adversary` te sigue al salir de su espacio? | **Sí, sigue**, como cualquier otro enemigo. El manual dice que una vez en el mapa "actúa como cualquier enemigo" y solo ejemplifica el ataque (p. 19), así que es una lectura del GDD y no una cita. Se elige seguir porque la alternativa —un jefe estacionario— devuelve zonas seguras permanentes al mapa y desactiva la presión de la mitad 2. Consecuencia aceptada: el jefe es arrastrable y se puede kitear, y `Stealth` sirve contra él |
| Un enemigo con permiso de cruzar pasajes bloqueados | Su distancia se mide con **sus** permisos, incluso para decidir si es "el más cercano" (manual p. 20) → [`../01-fundamentos/mapa-y-espacios.md`](../01-fundamentos/mapa-y-espacios.md) |
| El personaje muere con enemigos encima | Las figuras enemigas **se quedan** en el espacio. Si algún efecto necesita la posición del personaje muerto, se usa el espacio donde murió (manual p. 19) |
| Se saca el límite de pool (A-04) | El spawn **nunca** se saltea y `EnemigosMax` deja de existir. El tope blando de dificultad desaparece y hay que reemplazarlo por otro mecanismo —cooldown de `Gate`, cap por espacio, o presupuesto por turno—; el manual no ofrece ninguno porque no le hace falta |

---

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución provisional** declarada
> ahí mismo: `p(Hit) = 0,50`, `p(Toll) = 0,33`.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| `Pool(Servant)` | 10 (manual p. 20) | 6–16, o sin límite | El techo de presión de la mitad de la partida ligada al `Objective`. Sin límite, los spawns nunca se saltean y desaparece el tope blando de dificultad (A-04) |
| `Pool` por tipo de `Creature` | 5 / 2 / 1 según el tipo (18 figuras en 7 tipos, manual p. 3) | 1–6 | Cada cuánto un spawn se saltea. Los tipos de pool 1 son casi únicos: matarlos se siente como un logro y su ausencia se nota el resto de la partida |
| Tipos de `Creature` por misión | Lo define el episodio; las reglas opcionales del manual agregan 2 tipos más (p. 21) | 2–4 | La variedad de amenazas y, directamente, cuántos stat blocks y cuántos modelos hay que producir → [`../05-partida/contenido-de-una-mision.md`](../05-partida/contenido-de-una-mision.md) |
| `H` de un enemigo | `Servant` = 2, un tipo de `Creature` = 3. **El resto no está en la transcripción** | 1–6 | Cuántas acciones cuesta limpiar un espacio. A 4+, un solo enemigo se come un turno entero y `Recover` deja de ser alcanzable, porque nunca llegás a tener el espacio `Clear` |
| Dados de ataque por enemigo | 1 estándar + 2 bonus (el único bloque visible, manual p. 17) | 1–4 dados totales | El costo por turno de quedarse sucio. **Estándar y bonus no son intercambiables**: los bonus no tienen cara `Toll`, así que subirlos aumenta el daño sin acelerar el trinquete → [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md) |
| Enemigos invocados por carta | 1 a 2 en los ejemplos del manual (p. 15) | 1–3 | La pendiente de la curva de presión. A 3 por carta, el pool se agota en pocos turnos y el juego se vuelve más fácil de lo esperado justo cuando debería apretar |
| Proporción de cartas con spawn | Un tercio (afirmación de este documento; **la transcripción no da la composición del mazo**, solo que son 8 cartas por adversario) | 1/4 – 1/2 | Cuán a menudo la presión es "aparecen enemigos" en vez de "te pasa algo". Arriba de 1/2 el mapa se llena antes de que el reloj importe |
| Budget de `Sneak` por nivel de `Stealth` | 0 / 1 / 3 / 3 / sin límite (manual p. 24) | ±1 por nivel; el nivel 4 es el peligroso | Si la regla de "te siguen" tiene contra o no. El nivel 4 **elimina la regla entera** para ese personaje, que es justamente lo que lo hace un premio de fin de progresión |
| Herida del esquive (`Stealth` 3+) | 1 por enemigo esquivado (manual p. 24) | 0–2 | Si escapar también limpia. A 2, un personaje de `Stealth` mata pool completo corriendo y el `Attack` pierde sentido |
| ¿El `Adversary` sigue al personaje? | **Sin resolver en el manual** | Sí / No | Si el jefe es kiteable. "Sí" permite arrastrarlo lejos del `Objective` y trivializa su presencia; "No" crea zonas seguras a un paso de él |

---

## ✅ Criterios de aceptación

1. Con un tipo de `Pool = 5` y 4 figuras en el mapa, una carta con 2 spawns de ese tipo
   coloca **1** figura, saltea el segundo paso, termina de resolver la carta, y el mapa queda
   con exactamente 5 figuras de ese tipo.
2. Con un tipo de `Pool = 2` y las 2 en el mapa, matar 1 en la fase 1 y resolver en la fase 2
   una carta que lo invoque coloca **1** figura nueva en la `Gate` indicada.
3. Una carta que invoca 1 `Servant` en cada una de las 3 `Gate` con `Disponibles = 2` deja
   **2** figuras colocadas y **1 `Gate` vacía**, elegida por el jugador, no por el orden
   interno de las puertas.
4. Un personaje que sale de un espacio con 3 enemigos y otro personaje presente se lleva los
   **3**; el espacio original queda con **0** enemigos y `Clear = true`.
5. Un `Move` de 3 pasos desde un espacio con 2 enemigos, atravesando espacios con 1 y 3
   enemigos, termina con **6** enemigos en el espacio del personaje, y la fase 3 encola 6
   ataques.
6. El mismo recorrido con `Stealth` nivel 2, gastando los 3 esquives en la última salida,
   termina con **3** enemigos encima y **3** en el espacio anterior.
7. Con `Stealth` nivel 3, cada enemigo esquivado recibe exactamente **1** herida; un enemigo
   con `H = 1` esquivado muere, su figura vuelve al pool y **no** queda en el espacio
   abandonado.
8. Un efecto que **coloca** al personaje en otro espacio termina con **0** enemigos
   siguiéndolo, **0** tokens de `Hazard` agarrados y **0** esquives de `Stealth` gastados.
9. Un enemigo con `H = 3` y 1 herida que recibe 3 `Hit` muere, y sin `Brawling` los otros
   enemigos del espacio terminan con **exactamente las mismas heridas** que tenían antes de
   la tirada.
10. En la fase 3 con 3 enemigos de 1 estándar + 2 bonus se hacen **3 tiradas separadas de 3
    dados**, una por enemigo, en el orden que elige el jugador — no una sola tirada de 9. Eso
    importa: `Toughness` reduce **por ataque**, los rerolls se deciden por tirada, y el
    marcador puede frenar en un umbral entre un ataque y el siguiente. El `Ratchet` solo puede
    subir por los `Toll` de los **3** dados estándar (1 por enemigo).
11. Un enemigo que entra al espacio a mitad de la fase 3 ataca; uno que se va antes de su
    turno de ataque no ataca; y si el espacio queda `Clear` a mitad de la fase, los ataques
    se detienen y **no** se roba carta de `Reward`.
12. Un enemigo que sale de un espacio con 2 tokens de `Hazard` termina con **0** tokens de
    `Hazard` asociados, y los 2 tokens siguen en el espacio.
