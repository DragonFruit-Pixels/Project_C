# 🔄 Secuencia de turno

← [Índice](../README.md)

> Este es el documento que más se va a consultar durante la implementación.

**Un turno por personaje.** Con 4 personajes, una ronda son 4 turnos completos, uno tras
otro. No hay fase simultánea.

Las cuatro fases van **siempre en este orden**, y ninguna se puede saltear:

```
1. Tres acciones
2. Carta de presión
3. Reckoning
4. Fin de turno
```

---

## Fase 1 — Tres acciones

El personaje activo tiene **3 acciones**. Puede repetir la misma.

Hay **4 acciones base**, más **2 propias de la misión** definidas por el escenario.

### `Move`

Movés hasta **3 espacios** por aristas del grafo.

Dos cosas pasan al salir de un espacio:

- **Todos los enemigos que había te siguen** al espacio nuevo. Todos, aunque haya otros
  personajes ahí.
- Si el espacio tenía `Hazard`, **te llevás uno por cada token**. Los del espacio quedan
  donde estaban; los tuyos se resuelven a fin de turno.

> Si un efecto te mueve fuera de una acción `Move`, aplica lo mismo: los enemigos siguen y
> agarrás `Hazard`. La única excepción son los efectos que **colocan** al personaje en otro
> espacio: ahí aparece directo, sin arrastrar nada.

### `Attack`

Elegís **1 enemigo en tu espacio** y hacés una tirada. Cada `Hit` es 1 herida.

Si las heridas igualan o superan la vida del enemigo, muere. El daño sobrante se pierde.

Sin la skill `Brawling`, el objetivo se elige **antes** de tirar, y todas las heridas van a
ese enemigo aunque sobren.

El adversario solo se puede atacar después de romper el objetivo, incluso si ya está en el
mapa. → [`04-oposicion/objetivo-y-adversario.md`](../04-oposicion/objetivo-y-adversario.md)

### `Recover`

**Solo en un espacio `Clear`.** Curás **3 puntos totales** repartidos entre `Wounds` y
`Reserve` como quieras. Se puede repetir en el mismo turno.

**`Ratchet` no se cura.** Nunca.

### `Trade`

Intercambiás objetos con los personajes que estén en tu espacio. Los `Burden` no se
intercambian.

### Acciones libres y especiales

Algunos efectos son **acciones libres**: se usan en esta fase sin gastar acción.

Otros son **acciones especiales**: se usan en esta fase y **sí** gastan una de las 3.

Las dos solo se pueden usar durante la fase 1.

---

## Fase 2 — Carta de presión

Se roba **1 carta** del `Pressure Deck` y se resuelve **de arriba hacia abajo**. Después va
a una pila de descarte **boca arriba**.

Las cartas tienen una **estructura fija de tres partes**:

**1. `Doom Symbol`.** No hace nada al robarse. Se cuenta a fin de turno. Si la carta lo
tiene, al descartarla **tiene que quedar visible**.

**2. Efecto especial.** Mover enemigos, dañar, forzar tiradas, lo que sea. Salvo que diga
otra cosa, **afecta solo al personaje activo** e ignora a los demás. Toda decisión que haya
que tomar la toma el jugador activo.

Cuando un efecto mueve un enemigo hacia algo, se mueve por el **camino más corto**, todo lo
que le permita el efecto, y **para al llegar**.

**3. Spawn.** Indica una `Gate` y qué enemigo aparece ahí. Si hay varios, se hacen en orden.

### La regla de ejecución

**Un paso que no se puede ejecutar se saltea, no falla.**

Si no hay figuras disponibles para invocar, se saltea ese spawn. Si no hay enemigos del tipo
que la carta quiere mover, se saltea ese movimiento. La carta sigue resolviéndose.

Esta semántica vale para todo el juego y hay que implementarla una sola vez.

---

## Fase 3 — `Reckoning`

La fase más corta y la que más define el juego. Depende de una sola pregunta: **¿tu espacio
está `Clear`?**

| Estado del espacio | Qué pasa |
|---|---|
| `Clear` | Robás una carta del `Reward Deck` |
| Con enemigos | **Todos te atacan**, uno por uno, en el orden que elijas |

**No hay punto medio.** Cada turno termina en premio o en paliza, y lo decide únicamente si
limpiaste tu espacio.

Detalles:

- Investigar es **obligatorio** si estás limpio, aunque no quieras.
- Los enemigos te atacan **solo a vos**, ignoran a los otros personajes.
- Si un efecto mete un enemigo en tu espacio **durante** esta fase, ese enemigo también
  ataca.
- Si un enemigo se va durante la fase, deja de atacar.
- Si el espacio queda limpio a mitad de la fase, los enemigos dejan de atacar, **pero no
  podés investigar**: lo que cuenta es cómo estaba al empezar la fase.

---

## Fase 4 — Fin de turno

Cinco pasos, en orden.

**1. Efectos de fin de turno.** Todos los que haya, menos los del adversario. El jugador
elige el orden.

**2. `Hazard`.** Tirás **1 dado estándar por cada token** que tengas encima, como una sola
tirada. Podés gastar `Reserve` para rerollear. Cada `Hit` es 1 herida, cada `Toll` es +1
`Ratchet`. Después **se descartan todos** los tokens.

> Los enemigos no agarran `Hazard` al moverse. Solo los personajes.

**3. Chequear el descarte.** Si hay **3 o más `Doom Symbol`** en la pila:

- El adversario **avanza** en el `Doom Track`
- Se resuelven los efectos del avance
- **Todo el descarte se remezcla** de vuelta en el mazo

→ [`04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)

**4. Chequear la manifestación** del adversario.

**5. Efectos de fin de turno del adversario**, resolviendo los stages revelados en orden.

El turno terminó. Pasa al siguiente personaje.

---

## Notas de implementación

**El descarte tiene que ser visible.** En la mesa, ver la pila crecer es información pública
y gratis: es lo que genera la tensión de la fase 4. En pantalla, si no se construye esa UI,
**se pierde el mecanismo de presión entero**.

**Los empates los decide el jugador.** El manual repite decenas de veces que ante varias
formas de resolver algo, elige el jugador activo. Cada una de esas es o un prompt (tedioso)
o una regla automática (pierde agencia). Cuántas se automatizan sigue abierto.

**El orden de las fases es rígido.** No hay atajos, no hay fases opcionales, no hay
reacciones fuera de turno. Eso hace que la máquina de estados sea chica y verificable.

---

## 📐 Fórmulas

El turno es una **secuencia**, no una cuenta. La mayor parte de esta sección documenta la
máquina de estados y los procedimientos de decisión con el mismo rigor con el que se
documentaría una fórmula: entradas, salidas, orden de evaluación y un ejemplo recorrido. La
aritmética real del turno son cuatro presupuestos (acciones, movimiento, `Hazard` y
`Doom Symbol`), y esos sí son fórmulas.

### 1. La máquina de estados

| Estado | Se entra cuando | Se sale cuando | Qué decide el jugador |
|---|---|---|---|
| `TurnStart` | Terminó el turno del personaje anterior | Inmediato | Nada |
| `P1_Actions` | Siempre, arrancando el turno | Se agotó el presupuesto de acciones **o** el jugador pasa | Qué acción, en qué orden, cuándo parar |
| `P2_Pressure` | Al salir de `P1` | Se resolvieron los 3 tipos de paso de la carta y se descartó | Las elecciones que pida el efecto (empates, objetivos, orden de spawn) |
| `P3_Referendum` | Al salir de `P2` | Se robó la `Reward` **o** todos los enemigos atacaron | Solo el **orden** de los atacantes. La rama no se elige |
| `P4_1_Effects` | Al salir de `P3` | No quedan efectos de fin de turno pendientes | El orden de resolución |
| `P4_2_Hazard` | Al salir de `P4_1` | Se tiró y se descartaron todos los tokens | Rerolls |
| `P4_3_DoomCheck` | Al salir de `P4_2`, **o por interrupción de muerte** | Se evaluó el contador | Nada: es automático |
| `P4_4_Manifest` | Al salir de `P4_3` | Se evaluó la condición | Nada |
| `P4_5_Adversary` | Al salir de `P4_4` | Se resolvieron los stages revelados en orden | Lo que pidan los efectos |
| `TurnEnd` | Al salir de `P4_5` | Inmediato | Nada |

**Transiciones especiales — son solo dos:**

- **Muerte del personaje en cualquier punto del turno** → salto directo a `P4_3_DoomCheck`,
  se saltea todo el resto, y de ahí se va a `TurnEnd`. Los turnos siguientes de ese personaje
  se saltean enteros (manual, pág. 19).
- **No hay ninguna otra.** No hay reacciones fuera de turno, no hay fases opcionales y ninguna
  fase se puede saltear por elección del jugador. La máquina es un camino único con un solo
  atajo.

Pseudocódigo de referencia:

```
turno(C):
  P1: a = 3 + acciones_extra(C)
      mientras a > 0 y el jugador no pase:
          elegir accion de {Move, Attack, Recover, Trade, Mision_1, Mision_2, especial}
          validar precondiciones            # Recover exige Clear, Attack exige objetivo
          resolver
          a -= 1
          si C murio -> ir a P4_3
      # las acciones libres se intercalan aca y NO consumen a
  P2: carta = robar(PressureDeck)
      para cada paso de la carta, de arriba hacia abajo:
          si el paso no se puede ejecutar -> saltear y seguir con el siguiente
      descartar boca arriba, con el Doom Symbol visible si lo tiene
      si C murio -> ir a P4_3
  P3: clear_inicial = (enemigos_en_espacio(C) == 0)     # se evalua UNA sola vez, aca
      si clear_inicial: robar 1 Reward                   # obligatorio
      si no:
          mientras queden enemigos que no atacaron:
              el jugador elige el proximo -> ese enemigo ataca
              # los que entran durante la fase tambien atacan; los que se van, no
  P4: 1. efectos de fin de turno, menos los del adversario. Orden: jugador
      2. Hazard: 1 dado estandar por token, UNA sola tirada, y descartar todos los tokens
      3. si doom(descarte) >= 3: avanzar adversario -> efectos -> remezclar TODO el descarte
      4. chequear manifestacion del adversario
      5. efectos de fin de turno del adversario, por stage revelado en orden
  TurnEnd -> siguiente personaje
```

### 2. Presupuesto de acciones

```
acciones_gastables = 3 + acciones_extra
acciones_efectivas = acciones_gastables + acciones_gratis
```

| Variable | Qué es | Rango |
|---|---|---|
| `3` | acciones base del turno (manual, pág. 13) | constante |
| `acciones_extra` | +1 por `Swiftness` nivel 4 (manual, pág. 25). ⚠️ *nivel 4 fuera de alcance en esta versión*, así que hoy vale **0** | 0–1 |
| `acciones_gratis` | 1 `Move` gratis con `Swiftness` nivel 2, 1 ataque a distancia gratis con `Marksman` nivel 4, más lo que den cartas | 0–3 |

Las **acciones libres** de efectos y cartas no entran en ninguna de las dos cuentas: no
consumen presupuesto y solo se pueden usar en `P1`.

**Ejemplo (con el manual completo, no con el alcance de esta versión):** personaje con `Swiftness` nivel 4 y `Marksman` nivel 4.
`acciones_gastables = 3 + 1 = 4`. `acciones_gratis = 1` (`Move` de `Swiftness` nivel 2, que se
suma porque los niveles de `Swiftness` no se reemplazan) `+ 1` (ataque de `Marksman` nivel 4)
`= 2`. Total: **6 acciones efectivas**, el doble del default. Ese es el techo de la economía de
acciones del juego y hay que asumirlo al balancear la duración del turno.

### 3. Presupuesto de movimiento

```
espacios_por_Move = 3 + (1 si Swiftness >= nivel 1)
```

Rango: **3–4**. Se cuenta por aristas del grafo, y el movimiento se puede detener antes.

**Ejemplo:** `Swiftness` nivel 1 recorriendo `S1 → S3 → S5 → S7 → S9`: son 4 aristas, es
legal, y hubo **4 salidas de espacio**, cada una con su chequeo de enemigos que siguen y de
`Hazard` que se agarra.

### 4. Acumulación de `Hazard`

```
tokens_agarrados = Σ tokens_en_el_espacio(e)   para cada espacio e que ABANDONÁS
```

- Se cuenta por **salida**, no por espacio visitado: entrar no cobra nada, salir sí.
- Los tokens del espacio **quedan donde están**: no se mueven, se copian.
- Volver a entrar y volver a salir del mismo espacio **cobra de nuevo**.
- Rango: 0 a (tokens del mapa × salidas del turno). En la práctica 0–4.

**Ejemplo:** el personaje arranca en `S1`, que tiene 1 token de `Hazard`. Hace `Move` a `S3`
(2 tokens) y sigue hasta `S5` (limpio). Salió de `S1` → +1. Salió de `S3` → +2. Termina el
`Move` con **3 tokens encima**, y `S1` y `S3` siguen con 1 y 2 respectivamente. En `P4_2` va a
tirar **3 dados estándar** en una sola tirada.

### 5. El `Reckoning` en números

```
n_atacantes = enemigos_al_empezar_P3 + entraron_durante_P3 − salieron_durante_P3
E[Wounds]   = Σ E[Hit] de la tirada de cada atacante
E[Ratchet]  = Σ E[Toll] de la tirada de cada atacante
```

Las esperanzas por tirada salen de [`dados.md`](dados.md) y dependen de la distribución de
caras, que **el manual no publica**. Los dados de cada enemigo salen de su ficha; el manual
solo trae un ejemplo suelto de 1 estándar + 2 bonus (pág. 17) y **las fichas de este proyecto
no están definidas**.

**Ejemplo** (con la distribución provisional declarada en `dados.md`, `p(Hit) = 0.5`): dos
enemigos de 1 estándar + 2 bonus cada uno atacando al mismo personaje. `n_atacantes = 2`, se
tiran 2 tiradas de 3 dados, `E[Wounds] = 2 × 1.5 = 3.0` y `E[Ratchet] = 2 × 0.33 = 0.67`. La
rama de al lado, con el espacio `Clear`, entrega **1 carta de `Reward Deck` y cero riesgo**.
Ese salto —de 3 heridas esperadas a un premio— es todo el peso de la fase 3 en un solo número.

### 6. El contador de `Doom Symbol`

```
avanza_el_adversario  ⟺  Σ Doom Symbol visibles en el descarte ≥ 3
```

El chequeo se hace **una sola vez por turno**, en `P4_3`, nunca en el momento de robar la
carta. Al avanzar, **todo** el descarte se remezcla y el contador vuelve a 0.

Cuántos turnos pasan entre dos avances:

```
E[turnos_por_avance] = 3 / f
Var                  = 3 × (1 − f) / f²
```

- `f` — fracción de cartas del `Pressure Deck` que traen `Doom Symbol`. **El manual no da este
  número**: solo dice que el mazo son 16 cartas (pág. 10). Es la perilla más importante del
  reloj.
- Es la aproximación con reposición, y sirve porque el descarte se remezcla en cada avance.
  Sin reposición la varianza real es un poco menor.

**Ejemplo:** con `f = 0.5`, `E[turnos_por_avance] = 3 / 0.5 = 6 turnos` y
`Var = 3 × 0.5 / 0.25 = 6`, o sea una desviación estándar de **2.45 turnos**. Con 4 personajes
(4 turnos por ronda) eso es **un avance cada 1.5 rondas**, con un rango típico de 3.5 a 8.5
turnos. Esa varianza es la que hace que el reloj se sienta impredecible; bajar `f` la agranda
en términos absolutos.

### 7. Procedimiento de resolución de una `Pressure Card`

Entradas: la carta y el estado del tablero. Salidas: el tablero modificado y una carta en el
descarte, con el `Doom Symbol` visible si lo tiene.

```
1. Doom Symbol  → NO se resuelve al robar. Solo tiene que quedar visible al descartar.
2. Efecto especial → afecta SOLO al personaje activo, salvo que la carta diga otra cosa.
                     Todas las decisiones las toma el jugador activo.
                     Movimiento de enemigos: camino más corto, todo lo que permita el
                     efecto, y para al llegar. Empate de caminos: elige el jugador.
3. Spawn → por cada Gate indicada, en el orden de la carta. Si no hay figura del tipo
           pedido, se saltea ESE spawn y se sigue con el siguiente.
Regla transversal: un paso que no se puede ejecutar SE SALTEA, no falla ni aborta la carta.
```

**Ejemplo recorrido.** Carta con `Doom Symbol`, efecto "mover cada `Creature` 2 espacios hacia
el personaje activo" y spawn "1 `Servant` en la `Gate` roja". En el tablero hay 1 `Creature` a
3 espacios de distancia y **cero figuras de `Servant` disponibles** en la reserva.

1. El `Doom Symbol` no hace nada ahora. Queda anotado para `P4_3`.
2. La `Creature` se mueve 2 espacios por el camino más corto y queda a 1 de distancia: se movió
   todo lo que el efecto permitía y no llegó, así que no para antes.
3. No hay figuras de `Servant` → **el spawn se saltea**. La carta no falla.
4. La carta va al descarte boca arriba con el símbolo visible. El contador sube en 1.

### 8. Ejemplo recorrido de un turno completo

Estado inicial: `C1` está en `S4` con 1 `Servant` (2 de vida); `Wounds` 2, `Reserve` 3,
`Ratchet` a 2 puntos de un umbral; tiene `Swiftness` nivel 1 y 1 dado bonus permanente; el
descarte de presión tiene **2 `Doom Symbol`**; el adversario todavía no se manifestó.

| Estado | Qué pasa | Resultado |
|---|---|---|
| `P1` acción 1 | `Attack` al `Servant`. Tira 3 estándar + 1 bonus: `Hit`, `Hit`+`Toll`, `Wild`, `Blank` | 2 `Hit` = el `Servant` muere. Después, 1 `Toll` → `Ratchet` queda a **1 del umbral** |
| `P1` acción 2 | `Move` de 4 espacios (3 + `Swiftness` 1) de `S4` a `S8`, pasando por `S6` que tiene 2 `Hazard` | Al salir de `S4` no arrastra nada (ya no hay enemigos) y al salir de `S6` se lleva **2 tokens**. `S6` sigue con sus 2 |
| `P1` acción 3 | `S8` está `Clear` → `Recover`, 3 puntos: 2 a `Wounds` y 1 a `Reserve` | `Wounds` 0, `Reserve` 4. `Ratchet` **no se toca** |
| `P2` | Carta con `Doom Symbol`; efecto mueve cada `Creature` 2 espacios hacia `C1` y una llega a `S8`; spawn de `Servant` sin figuras disponibles | El spawn se saltea, la carta se descarta boca arriba. Descarte: **3 `Doom Symbol`** |
| `P3` | `clear_inicial` se evalúa **ahora**: hay una `Creature` en `S8` → no está `Clear`, **no** se roba `Reward`. La `Creature` (1 estándar + 2 bonus) ataca: `Hit`, `Hit`, `Blank`. `C1` gasta 1 `Reserve` para rerollear un `Hit` y sale `Blank` | `Wounds` 1, `Reserve` 3, 0 `Toll` |
| `P4_1` | No hay efectos de fin de turno pendientes | Nada |
| `P4_2` | `Hazard`: 2 tokens → **una sola tirada** de 2 dados estándar: `Hit` y `Toll` | `Wounds` 2. El `Toll` **cruza el umbral**: el marcador para ahí, se dispara la `Ratchet Card`, sube 1 nivel de skill y, si ese umbral trae dado, +1 bonus permanente. Se descartan los 2 tokens |
| `P4_3` | 3 `Doom Symbol` ≥ 3 | El adversario **avanza**, se resuelven sus efectos, y **todo** el descarte se remezcla en el mazo: el contador vuelve a **0** |
| `P4_4` | ¿Entró al primer espacio rojo o se rompió el objetivo? No | No se manifiesta |
| `P4_5` | No hay stages revelados | Nada |
| `TurnEnd` | — | Pasa a `C2` |

Saldo del turno: 1 enemigo muerto, 2 `Wounds`, 1 `Reserve` gastado, **1 umbral cruzado** y
**1 avance del adversario**. Un turno normal mueve las cuatro barras del juego a la vez, y eso
es lo que hay que poder leer en pantalla al terminarlo.

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| El personaje muere en medio de su turno | Se saltean todas las fases restantes **excepto `P4_3`**. Si hay 3 o más `Doom Symbol`, el adversario avanza igual, y si algún efecto necesita su posición se usa **el espacio donde murió**. De ahí en adelante, su turno se saltea entero (manual, pág. 19). |
| El espacio queda `Clear` a mitad de `P3` | Los enemigos **dejan de atacar**, pero **no se roba `Reward`**: lo que cuenta es cómo estaba el espacio al **empezar** la fase (manual, pág. 16). |
| Un enemigo entra al espacio durante `P3` | **Ataca igual**, aunque no estuviera ahí al empezar la fase (manual, pág. 16). |
| Un enemigo se va del espacio durante `P3` | **No ataca.** Si ya había atacado, lo hecho queda hecho. |
| Estás `Clear` y no querés robar `Reward` | No es opcional: **es obligatorio** robar (manual, pág. 16). |
| El `Reward Deck` se quedó sin cartas | No se puede robar más: la fase 3 termina sin efecto (manual, pág. 16). No es derrota ni penalización. |
| Un spawn de la carta no tiene figuras disponibles | Se saltea **ese** spawn y se sigue con los pasos siguientes. Si la carta invoca en varias `Gate` y las figuras se acaban, se saltean las `Gate` restantes (manual, pág. 15). |
| La carta invoca en varias `Gate` sin especificar orden | El **jugador activo elige el orden**, y eso importa justamente cuando no alcanzan las figuras (manual, pág. 15). |
| Empate de camino más corto al mover un enemigo | Elige el jugador activo (manual, pág. 20). Lo mismo para "el más cercano" y para el orden de efectos simultáneos. |
| El camino más corto pasa por un pasaje bloqueado | No se puede usar: el bloqueo cuenta al medir distancias. La excepción son las figuras que por regla propia ignoran ese bloqueo, y para ellas se mide ignorándolo (manual, pág. 20). |
| Te mueven fuera de una acción `Move` | Aplica todo lo de `Move`: los enemigos te siguen y agarrás `Hazard`. La **única** excepción son los efectos que "colocan" al personaje: ahí aparece directo, sin arrastrar enemigos ni tomar tokens (manual, pág. 13). |
| Salir de un espacio con `Hazard` pero sin enemigos | Igual te llevás un token por cada token del espacio. El `Hazard` no depende de que haya enemigos. |
| Entrar a un espacio con `Hazard` y quedarte ahí | **No agarrás nada.** Solo cobra la salida. |
| Salís de un espacio con 3 enemigos y en el destino ya hay otro personaje | Los 3 te siguen igual. Que haya otro personaje en el destino no cambia nada (manual, pág. 13). |
| `Recover` en un espacio que dejó de estar `Clear` | La condición se chequea **al usar la acción**. Si un efecto mete un enemigo antes, `Recover` ya no es legal hasta volver a limpiar el espacio; un `Recover` ya resuelto no se revierte. |
| `Hazard` con 0 tokens en `P4_2` | El paso se saltea entero: no se tira nada. |
| El contador llega a 3 `Doom Symbol` a mitad del turno | No pasa nada en ese momento. El único chequeo es `P4_3`, y ahí puede haber 3, 4 o más: el efecto es el mismo, un solo avance. |
| Se acaban las cartas del `Pressure Deck` sin haber llegado a 3 `Doom Symbol` | **El manual no lo cubre.** El descarte solo se remezcla al avanzar, así que el mazo puede vaciarse. Hay que decidirlo: la opción coherente es remezclar el descarte de inmediato **sin** avanzar el adversario y **sin** resetear el contador de símbolos visibles, pero es una decisión abierta, no una regla del manual. |
| Se agota el pool de tokens de `Hazard` | En la mesa: si hay que poner un token en el tablero y no quedan, **se ignora el efecto**; sobre el personaje no hay límite y se sustituye por un token de herida (manual, pág. 17). En digital no existe el límite físico, así que **hay que decidir** si se conserva el cap del tablero como regla de balance o se elimina. |
| Querés usar una acción libre en `P3` o `P4` | No se puede. Las acciones libres y las especiales **solo existen en `P1`** (manual, pág. 13). |
| El personaje termina el turno en el espacio del adversario ya manifestado | El adversario ataca como cualquier enemigo en `P3`, tirando la suma de dados de **todos** sus stages revelados (manual, pág. 19). |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Acciones por turno | 3 (manual, pág. 13) | 2–4 | La densidad de decisiones del turno y, de rebote, la velocidad del trinquete: más acciones son más tiradas, y más tiradas son más `Toll`. Subirlo a 4 acelera la progresión sin tocar los dados. |
| Espacios por `Move` | 3 (manual, pág. 13) | 2–5 | Qué tan pegajoso se siente el mapa. Con 5, los enemigos que te siguen dejan de ser un problema y `Stealth` pierde razón de existir. |
| Puntos de `Recover` | 3, repartibles (manual, pág. 14) | 2–4 | El ritmo de recuperación, y por lo tanto la letalidad real. Interactúa con el máximo de `Reserve` (4): con 2 puntos, volver a tope cuesta dos acciones. |
| Umbral de `Doom Symbol` en el descarte | 3 (manual, pág. 18) | 2–5 | La velocidad del reloj. Bajarlo a 2 acorta la partida y **adelanta la ventana de sacrificio** del trinquete, porque el adversario se manifiesta antes → toca [`04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md) y la estrategia central de [`trinquete.md`](../02-personaje/trinquete.md). |
| Fracción de cartas con `Doom Symbol` | **No está en el manual.** Solo dice que el mazo son 16 cartas | 0.3–0.6 | La misma velocidad de reloj que la perilla anterior, pero por la vía de la varianza: con `f` bajo el reloj es lento y errático, con `f` alto es rápido y predecible. |
| Tamaño del `Pressure Deck` | 16 cartas (manual, pág. 10) | 12–20 | Cuánto se repiten los efectos dentro de una partida y qué tan pronto se agota el mazo. Con 12 hay que resolver ya el caso borde del mazo vacío. |
| Cartas de presión por turno | 1 (manual, pág. 15) | 1–2 | La presión total del juego. A 2 se duplican el reloj y los spawns al mismo tiempo: es la perilla más brusca de todas. |
| Cartas de `Reward` por `Reckoning` | 1 (manual, pág. 16) | 1–2 | Cuánto paga limpiar el espacio. Subirlo hace que valga la pena invertir dos acciones en limpiar, y baja la presión de la rama de pelea. |
| Acciones propias de la misión | 2 (manual, pág. 13) | 2–3 | Cuánto del turno se dedica al objetivo en vez de al combate. Con 3 el escenario compite con las acciones base por el presupuesto. |
| Personajes por ronda | 4 (decisión del GDD, ver [`00-vision/alcance.md`](../00-vision/alcance.md); **el manual no fija la cantidad**) | 1–4 | Cuántos turnos hay por avance del reloj. Con 2 personajes, cada avance del adversario cae cada media ronda y la partida se acorta a la mitad en tiempo real sin cambiar ningún número. |

## ✅ Criterios de aceptación

1. Las cuatro fases se ejecutan siempre en el orden 1 → 2 → 3 → 4, y no existe una transición que entre a la fase 3 sin haber resuelto y descartado la carta de presión.
2. Con 3 acciones y sin skills de economía, el cuarto intento de acción está bloqueado; una acción libre usada entre medio **no** consume presupuesto.
3. Un `Move` con `Swiftness` nivel 1 permite exactamente **4** espacios y bloquea el quinto.
4. Salir de un espacio con 2 tokens de `Hazard` deja **2 tokens en el espacio** y agrega **2 al personaje**.
5. Salir de un espacio con 3 enemigos mueve **los 3** al espacio destino, incluso si ahí ya hay otro personaje.
6. `Recover` está deshabilitada con 1 o más enemigos en el espacio, y cuando está habilitada cura **exactamente 3 puntos** repartibles entre `Wounds` y `Reserve`, sin tocar `Ratchet`.
7. Una carta de presión cuyo spawn no tiene figuras disponibles **completa los pasos restantes** y termina en el descarte: la carta no aborta.
8. Si el espacio tiene al menos 1 enemigo al **empezar** la fase 3, no se roba `Reward` ni aunque el espacio quede limpio a mitad de la fase.
9. Un enemigo que entra al espacio durante la fase 3 ejecuta su ataque; uno que se va antes de atacar no lo ejecuta.
10. `Hazard` con 2 tokens produce **una sola tirada de 2 dados** (no dos tiradas de 1), acepta rerolls pagos, y al terminar el paso el personaje tiene **0 tokens**.
11. Con exactamente 3 `Doom Symbol` visibles al llegar a `P4_3`, el adversario avanza **una** vez y el descarte queda en **0 cartas** con el contador en **0**.
12. Con 2 `Doom Symbol` no pasa nada y las cartas siguen en el descarte, visibles.
13. Si el personaje muere durante la fase 1, se ejecuta **solo** `P4_3` y ninguna otra fase, y su siguiente turno se saltea por completo.
