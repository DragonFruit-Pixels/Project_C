# ⏳ El reloj (`Doom Track`)

← [Índice](../README.md)

## La estructura

El `Doom Track` tiene **8 espacios**. El `Adversary` arranca a la izquierda y avanza hacia
la derecha.

Si el marcador llega al **octavo espacio, se pierde la partida**. Sin excepciones y sin
apelación.

## Cómo avanza

Al final de cada turno se cuentan los **`Doom Symbol`** en la pila de descarte del
`Pressure Deck`.

**Si hay 3 o más**, tres cosas pasan, en este orden:

1. El `Adversary` **avanza 1 espacio** en el track
2. Se resuelven los efectos de ese avance
3. **Todo el descarte se remezcla** de vuelta en el mazo

Ese tercer paso es el que hace interesante todo el sistema.

## Por qué el ritmo es impredecible

Como el mazo **se remezcla en cada avance**, el contador vuelve a cero y las cartas con
símbolo se redistribuyen al azar.

Entonces nunca sabés cuándo viene el próximo avance. Puede caer en 3 turnos si salen tres
cartas con símbolo seguidas, o en 6 o más si no salen. **La presión se autorregula y no se
puede planificar con precisión.**

Eso es distinto de un contador fijo. Un reloj que avanza cada N turnos se optimiza: el
jugador cuenta y calcula. Este no se puede contar — solo se puede leer la pila y estimar.

Y por eso **la pila de descarte tiene que estar visible**. En la mesa es información pública
gratis: ves crecer los símbolos y sabés que estás cerca. En pantalla, si no se construye esa
UI, el jugador no tiene forma de sentir la presión y el mecanismo entero se pierde.

## El problema de escala

La carta de presión se roba **una por turno de personaje**. Con 4 personajes, eso es **4
cartas por ronda**.

El juego original, para partidas de un solo jugador, recomienda controlar **2** personajes.
Con 4, el reloj corre **al doble de velocidad** que en esa configuración.

**Este es el primer número que hay que re-balancear**, y no es un ajuste menor: cambia el
largo de la partida, cuántos umbrales de `Ratchet` alcanza a cruzar cada personaje, y por
lo tanto cuánta build llega a construirse.

Las opciones obvias son alargar el track, subir el umbral de símbolos, o robar menos
seguido. Cuál se elige no está decidido.
→ [`06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)

## Qué pasa en cada avance

El avance no es solo mover un marcador. Dispara efectos de dos fuentes a la vez:

- **Los del `Adversary`**, escritos en sus Stage cards reveladas
- **Los de la misión**, escritos en la carta de escenario

Se resuelven **primero los del adversario**.

Y da igual si lo que avanza es la figura del adversario o el marcador que la reemplaza
después de manifestarse: los efectos se resuelven igual.

## Las dos mitades del track

El track tiene un punto de quiebre marcado.

**Antes:** avanza la figura del `Adversary`. Todavía no está en el mapa.

**Al llegar al primer espacio marcado:** el adversario **se manifiesta**. La figura pasa al
mapa y en su lugar en el track se pone un marcador de progreso.

**Después:** avanza el marcador. El adversary ya está en el mapa peleando.

De ahí en adelante el reloj sigue corriendo igual, con la diferencia de que ahora tenés al
jefe encima además del reloj.
→ [`objetivo-y-adversario.md`](objetivo-y-adversario.md)

## El reloj como diseño

Vale entender qué logra este sistema, porque condiciona todas las decisiones del jugador:

**Castiga la pasividad sin usar un timer.** No hay reloj real, pero cada turno que jugás
acerca el final. No podés esperar a estar listo.

**Hace que la velocidad sea un recurso.** Ir rápido y sucio te deja peor preparado para la
pelea final; ir lento y prolijo te deja sin tiempo. No hay una respuesta correcta fija.

**Y le pone un techo a la estrategia del sacrificio.** Quemar un personaje para volverlo
fuerte lleva turnos, y cada turno gastado en eso es reloj que corre.
→ [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md)

---

## 📐 Fórmulas

**Aviso previo, obligatorio.** El manual **no dice cuántas de las 16 cartas del
`Pressure Deck` llevan `Doom Symbol`**. Dice "la mayoría" (p.7) y nada más. Ese número es la
variable que gobierna todo el ritmo del juego, así que abajo va como parámetro `q` y los
ejemplos usan un valor **provisional, declarado acá y no sacado del manual**: `q = 12` de
`n = 16`, o sea 75%. Cuando se decida el contenido del mazo hay que recorrer estas cuentas de
nuevo.

Lo que **sí** sale del manual: el mazo son **16** cartas (8 del adversario + 8 de la misión,
mezcladas, p.10), el umbral son **3** símbolos (p.18), el track tiene **8** espacios (p.6), y
se roba **1** carta por turno de personaje (p.15).

### R1 — El disparo del avance (algoritmo)

No hay aritmética acá, hay orden de evaluación, y es donde se rompen las implementaciones.

**Entrada:** el descarte del `Pressure Deck` al terminar la fase 2 del fin de turno.
**Salida:** posición del marcador y estado del mazo.

Al final de **cada turno de personaje**, en este orden (manual p.17-18):

1. Efectos de fin de turno que no sean del adversario.
2. `Hazard`.
3. **Chequeo del descarte:** `D = ` cantidad de cartas en el descarte con `Doom Symbol`.
   Si `D ≥ 3`: (a) el marcador avanza **1** espacio, (b) se resuelven los efectos del avance
   —adversario primero, misión después—, (c) **todo** el descarte vuelve al mazo y se mezcla.
   Si `D < 3`: no pasa nada. **Tampoco se remezcla**: el descarte sigue creciendo.
4. Chequeo de manifestación.
5. Efectos de fin de turno del adversario, por stage: 1, 2, 3, final.

**Ejemplo recorrido.** Termina el turno del personaje 3. El descarte tiene 4 cartas y 3
llevan símbolo. `D = 3 ≥ 3` → el marcador pasa del espacio 2 al 3, se resuelven los efectos
del adversario y después los de la misión, y **las 4 cartas** —incluida la que no tenía
símbolo— vuelven al mazo. El turno del personaje 4 arranca con un mazo de 16 y `D = 0`,
aunque en el mazo sigan existiendo 12 cartas con símbolo. El contador es del descarte, no del
mazo.

Consecuencia de implementación: como el remezclado devuelve **todo**, cada ciclo de avance
arranca desde un mazo idéntico de 16 cartas. Los ciclos son independientes entre sí, y por eso
las cuentas de abajo se pueden multiplicar sin corregir nada.

### R2 — Cartas por avance

Sacar el símbolo número `T` de un mazo de `n` cartas con `q` marcadas, robando sin reponer,
es una hipergeométrica negativa. La esperanza es exacta:

```
E[robos por avance] = T × (n + 1) / (q + 1)
```

| Variable | Qué es | Rango |
|---|---|---|
| `T` | símbolos necesarios para avanzar | **3** (manual). Tope duro: `T ≤ q` |
| `n` | tamaño del `Pressure Deck` | **16** (manual) |
| `q` | cartas del mazo con `Doom Symbol` | **no está en el manual**; provisional 12 |

- **Mínimo posible:** `T` robos (tres cartas con símbolo seguidas) → **3**.
- **Máximo posible:** `(n − q) + T` → con `q = 12`, **7**.
- **Esperanza con los valores provisionales:** `3 × 17 / 13 = ` **3,92 robos por avance**.

**Ejemplo:** con `q = 12`, el avance llega en promedio cada **3,9 turnos de personaje**, con
una banda real de 3 a 7. Si el mazo tuviera solo 8 cartas con símbolo, la esperanza subiría a
`3 × 17 / 9 = 5,67`: un 45% más lento sin tocar una sola regla. Por eso `q` es una perilla y
no un detalle de contenido.

### R3 — Largo de la partida, y el problema de escala en números

El marcador arranca en el espacio 1 y se pierde al llegar al 8, así que hacen falta
**7 avances**.

```
Turnos de personaje por partida = 7 × E[robos por avance]
Rondas por partida              = turnos de personaje / P
Turnos propios de cada personaje = rondas
Velocidad del reloj             = P / E[robos]   (avances por ronda)
```

`P` es la cantidad de personajes controlados: **4** en este proyecto, **2** en la
recomendación del manual para un solo jugador (p.10).

Con los valores provisionales, `7 × 3,92 = 27,5` turnos de personaje por partida:

| | `P = 2` (manual) | `P = 4` (este proyecto) |
|---|---|---|
| Turnos de personaje por partida | 27,5 | 27,5 |
| **Rondas por partida** | **13,7** | **6,9** |
| **Turnos propios de cada personaje** | **13,7** | **6,9** |
| Avances por ronda | 0,51 | 1,02 |

Y acá está el punto que hay que entender antes de tocar cualquier perilla: **la partida no se
acorta. Se aplana.**

Los 27,5 turnos de personaje son los mismos en las dos columnas. El `Ratchet` total que
acumula el equipo entero es el mismo, porque el `Ratchet` lo generan los turnos y los turnos
son los mismos. Lo que cambia es **cómo se reparte**: con `P = 4` cada personaje juega la
mitad de turnos propios, acumula la mitad de `Ratchet`, cruza la mitad de umbrales y compra
la mitad de niveles de skill.

### R4 — Qué le hace eso a la build

```
Umbrales cruzados por personaje ∝ turnos propios ∝ 1/P
```

No puedo dar el número absoluto: **el largo del track de `Ratchet` y la posición de los
umbrales no están en el manual** (están impresos en el tablero de personaje, que la
transcripción no incluye; el manual solo dice que 4 de los umbrales dan dado bonus, p.8).

Pero el **cociente** sí es exacto y no depende de esos números desconocidos, porque las dos
configuraciones escalan linealmente con los turnos propios: **con 4 personajes, cada personaje
llega a la mitad de la build que llegaría con 2.**

Y eso pega justo donde más duele, por dos razones que no son obvias:

1. **Las skills tienen 4 niveles y el techo se paga entero.** Llevar una skill a nivel 4
   cuesta 3 umbrales de un mismo personaje. Media build no son "skills más flojas": son
   **skills que nunca llegan al nivel 4**. La mitad de arriba de las 24 celdas —la que
   `skills.md` llama "la superficie de maestría real del juego"— no se ve nunca.
   → [`../02-personaje/skills.md`](../02-personaje/skills.md)
2. **El sacrificio deliberado se queda sin pista.** La estrategia insignia de esta adaptación
   (quemar un personaje a propósito) solo se habilita después de que el adversario se
   manifiesta. Esa ventana se abre siempre a la misma **fracción** de la partida —ver R6— pero
   con `P = 4` lo que queda después son ~4 turnos propios en vez de ~8. Con 4 turnos no se
   quema a nadie a fondo.
   → [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md)

### R5 — Las tres compensaciones, y la elección que nadie puede esquivar

Cualquier arreglo tiene que llevar la velocidad del reloj de 1,02 a 0,51 avances por ronda,
o sea dividirla por `P / P_ref = 4/2 = 2`. Las tres opciones que menciona la sección "El
problema de escala" hacen exactamente eso, con estas fórmulas:

| Opción | Fórmula | Valor con `P = 4` |
|---|---|---|
| **A —** alargar el track | `S_new = 1 + (S_base − 1) × P/P_ref` | `1 + 7×2 = ` **15 espacios** |
| **B —** subir el umbral | `T_new = T_base × P/P_ref` | `3 × 2 = ` **6 símbolos** |
| **C —** robar menos seguido | `k = P/P_ref` turnos por robo | **1 carta cada 2 turnos** |

Las tres devuelven las 13,7 rondas. Ninguna es gratis, y **las tres cuestan lo mismo en un
punto que hay que decir de frente: la partida pasa a durar 55 turnos de personaje en vez de
27,5.** El doble de tiempo real de sesión.

Ese es el nudo de la decisión, y no es un problema de encontrar el número: **son dos objetivos
incompatibles y hay que elegir uno.**

- Si querés conservar **la build** (13,7 turnos propios por personaje), la sesión se duplica.
- Si querés conservar **el largo de sesión** (27,5 turnos de personaje), la build se queda a
  la mitad y el nivel 4 de las skills queda decorativo.

No hay perilla que dé las dos cosas, porque las dos se miden en la misma unidad: turnos.

**Y cada opción rompe algo distinto:**

- **A (track de 15)** obliga a definir qué pasa en 7 espacios nuevos. Los efectos de avance
  salen de las Stage cards del adversario y de la carta de misión, espacio por espacio; 7
  avances sin contenido definido son 7 avances que no se sienten, y el reloj deja de escalar
  en amenaza. También hay que reubicar el espacio de manifestación proporcionalmente (R6).
- **B (umbral de 6)** es la más barata de implementar y la que menos toca el resto del
  sistema: mantiene 1 robo por turno, así que la presión de enemigos y de efectos queda
  intacta y el `Ratchet` sigue fluyendo igual. Su costo está escondido: el descarte llega a 8
  o 10 cartas antes de cada remezclado, o sea que el jugador ve **más de la mitad del mazo**
  en cada ciclo y el mazo **se vuelve predecible**. Eso ataca de frente la propiedad que la
  sección "Por qué el ritmo es impredecible" describe como el corazón del sistema. Tope duro:
  `T ≤ q`.
- **C (robar cada 2 turnos)** es la que parece más limpia y es la más peligrosa. El
  `Pressure Deck` no es solo el reloj: es también el motor de spawn de enemigos y de efectos.
  Robar la mitad de cartas es la mitad de enemigos, la mitad de ataques recibidos, la mitad de
  `Toll`, y por lo tanto **menos umbrales cruzados todavía**. Compensa el reloj y **empeora el
  problema de la build al mismo tiempo**. Además deja vacía la fase 2 del turno en la mitad de
  los turnos, y el turno pierde su latido.
  → [`../03-resolucion/turno.md`](../03-resolucion/turno.md)

### R6 — El auto-balanceo que ya existe, y qué opción lo rompe

```
Velocidad del reloj = P_vivos / E[robos por avance]
```

La velocidad depende de los personajes **vivos**, no de los que empezaron: un personaje
perdido no roba carta (manual p.19, su turno se saltea entero). Perder gente te da tiempo, y
eso ya está en las reglas sin que nadie lo diseñe.

**Ejemplo:** se pierde 1 personaje de 4 al final de la ronda 3. Desde ahí la velocidad pasa de
`4/3,92 = 1,02` a `3/3,92 = 0,77` avances por ronda: **un 25% menos de velocidad**, que es
**un 33% más de rondas** por avance (`1,02/0,77 = 1,31`) para los que quedan.

Las opciones A y B conservan ese lazo intacto. La opción C también, porque los robos siguen
atados a los turnos de personaje. **Lo que lo destruye es mover el chequeo o el robo a la
ronda** en vez del turno: ahí el reloj deja de saber cuánta gente queda viva, y perder un
personaje pasa a ser puro costo. Es la razón para no elegir esa variante aunque sea la más
fácil de programar.

### R7 — La ventana de manifestación

```
Avances hasta la manifestación = M − 1
Fracción de la partida bloqueada = (M − 1) / (S − 1)
```

`M` es el índice del primer espacio marcado del track. **No está en el manual**: la
transcripción dice "el primer espacio rojo" (p.6, p.18) pero no da su posición, que está
impresa en el Story Board de cada adversario.

Lo interesante es que esta fracción **es inmune al problema de escala**, porque se mide en
avances y no en turnos. Con `M = 4` el candado del sacrificio dura `3/7 = ` **43% de la
partida** con 2 personajes y también con 4. Es lo único que `P` no deforma.

Lo que sí se deforma es el resto absoluto: **4 turnos propios de ventana con `P = 4` contra 8
con `P = 2`** (R4, punto 2).

### R8 — La banda de varianza, que es lo que la UI tiene que comunicar

Con `T = 3` y `q = 12`, cada avance tarda entre 3 y 7 robos, así que la partida entera dura:

```
Mínimo = 7 × T          = 21 turnos de personaje = 5,25 rondas con P = 4
Máximo = 7 × ((n−q)+T)  = 49 turnos de personaje = 12,25 rondas con P = 4
```

Un factor **2,33** entre la partida más afortunada y la más desafortunada. Esa banda es la
que hace que el reloj no se pueda contar, y es exactamente la información que el jugador
tiene que poder leer de la pila de descarte. Si la UI no muestra `D` y `T`, el jugador queda
parado en una distribución que no ve.

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| Hay exactamente 3 símbolos | **Avanza.** La condición es `≥ 3`, no `= 3` (manual p.18). |
| Hay 5 símbolos en el descarte | Avanza **1 solo espacio**. No hay avance doble. Las 5 cartas —y las que no tienen símbolo— vuelven al mazo, y el contador queda en **0**: los 2 símbolos excedentes **se pierden**. |
| El descarte tiene cartas pero ningún símbolo | No avanza y **no se remezcla nada**. El descarte sigue creciendo hasta el próximo avance. El remezclado es consecuencia del avance, no del fin de turno. |
| Un personaje se pierde durante su propio turno | Se saltean todas las fases restantes **excepto el chequeo del descarte** (manual p.19). Si hay 3 símbolos, el adversario avanza y sus efectos se aplican; si alguno necesita la posición del personaje, se usa **el espacio donde se perdió**. |
| …y el adversario todavía está en el track | La partida se pierde **en el acto** (manual p.19), así que el chequeo del párrafo anterior solo importa cuando el adversario ya se manifestó. |
| Desde la ronda siguiente a esa pérdida | El turno de ese personaje **se saltea entero**, así que **no se roba su carta**: con 3 personajes vivos son 3 cartas por ronda y el reloj se frena un 25% en velocidad, o sea un 33% más de rondas por avance (R6). |
| El avance y la manifestación caen en el mismo turno | La fase 3 se resuelve **completa** —avance, efectos y remezclado— antes de la fase 4. El marcador de progreso se coloca en el espacio que la figura acaba de alcanzar (manual p.18). |
| Se rompe el `Objective` antes de llegar al espacio marcado | El adversario se manifiesta igual, al final de ese turno (manual p.18-19). El marcador de progreso se pone en el espacio que la figura ocupa **en ese momento**, sea cual sea, y de ahí en adelante avanza el marcador. |
| El avance deja el marcador en el espacio 8 | Se pierde la partida. **Si los efectos de ese avance se resuelven antes o no, el manual no lo dice.** Decisión: la derrota tiene prioridad y los efectos **no se resuelven** — no hay partida sobre la que aplicarlos. |
| Efectos del adversario y de la misión en el mismo avance | **Adversario primero**, misión después (manual p.18, explícito). El orden interno entre stages del adversario para efectos de *avance* el manual no lo fija; se aplica por analogía el de fin de turno: 1, 2, 3, final. |
| El mazo se queda sin cartas antes de completar el umbral | **Imposible en la configuración base:** el peor caso son `(16−12)+3 = 7` robos de 16. **Pasa a ser posible si se sube `T` o se baja `q`, y el manual no da regla para un mazo vacío.** Decisión: se remezcla el descarte de inmediato **sin avanzar y sin resetear el contador de símbolos** — lo que obliga a llevar el contador como número aparte y no como "cartas en la pila" en cuanto `T > 3`. |
| Se configura `T` mayor que la cantidad de cartas con símbolo (`T > q`) | El adversario **nunca avanza** y la derrota por reloj se vuelve inalcanzable. Es un error de configuración, no una dificultad baja: tiene que fallar ruidosamente. |
| Dos avances en la misma ronda | Posible y legal. Después de un remezclado alcanzan 3 cartas con símbolo seguidas, así que con `P = 4` puede haber dos avances dentro de una ronda. Es el extremo bueno de la banda de R8, visto desde el lado malo. |
| Una carta de presión obliga a robar otra carta | Cada carta extra va al descarte y **cuenta para el chequeo**, así que un solo turno puede aportar 2 o más símbolos (manual p.20, reglas opcionales). No está en el alcance base; si se incluyen, la compensación de R5 hay que calcularla con el **promedio de robos por turno**, no con 1. |
| Un `Ratchet` cruza umbral por `Hazard` en el mismo turno del avance | El `Hazard` es la fase 2 y el chequeo del reloj es la fase 3: **el umbral se resuelve primero**. Importa si el efecto del umbral o de la `Ratchet Card` toca el descarte de presión. |
| Un efecto quita una carta del juego en vez de descartarla | **No cubierto por el manual.** Decisión: no cuenta para el chequeo y **no vuelve** en el remezclado. Ojo con el efecto lateral: quitar una carta sin símbolo baja `n` y **acelera** el reloj; quitar una con símbolo baja `q` y **lo frena** (R2). Los efectos de remoción son una perilla de reloj escondida. |
| Se pierden todos los personajes después de la manifestación | La partida termina por la otra condición de derrota (→ [`../05-partida/victoria-y-derrota.md`](../05-partida/victoria-y-derrota.md)). Es el extremo degenerado del lazo de R6: sin nadie vivo no hay robos y el reloj se congela, pero ya no hay partida. |
| La UI esconde la pila de descarte | El mecanismo entero se pierde: el jugador queda dentro de la banda de R8 sin poder leerla. No es un caso borde de reglas, es un requisito: el contador `D` de `T` va siempre a la vista. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución provisional** declarada
> ahí mismo: `p(Hit) = 0,50`, `p(Toll) = 0,33`.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Umbral de `Doom Symbol` (`T`) | **3** (manual) | 3-6, tope duro `T ≤ q` | La velocidad del reloj, lineal. Es la compensación más barata para `P = 4`, y su costo es la **predecibilidad**: cuanto más alto, más del mazo ve el jugador antes de cada remezclado. Arriba de `q` el reloj se apaga. |
| Espacios del `Doom Track` (`S`) | **8** (manual) | 8-15 | El largo de la partida, lineal en `S − 1`. Cada espacio agregado necesita un efecto de avance definido en la carta de misión y en las Stage cards; sin eso, se agregan avances vacíos y el reloj deja de escalar en amenaza. |
| Cartas robadas por turno de personaje | **1** (manual) | 1, o 1 cada 2 turnos | El reloj **y** el spawn de enemigos **y** los efectos de presión, todo junto. Bajarlo frena el reloj y al mismo tiempo corta la fuente de `Ratchet`: es la única perilla de esta tabla que empuja los dos problemas de R3 en la misma dirección equivocada. |
| Personajes controlados (`P`) | 4 (decisión del proyecto; el manual recomienda **2**) | 2-4 | Todo. Es la perilla que genera el problema: la velocidad del reloj es lineal en `P` y los turnos propios de cada personaje van con `1/P`. Cambiarla es cambiar el juego, no balancearlo. |
| Cartas con `Doom Symbol` (`q` de `n`) | **no está en el manual** — provisional 12/16 | 8/16 a 14/16 | El ritmo y su varianza a la vez (R2, R8). Bajar `q` frena el reloj **y** ensancha la banda de impredecibilidad, que es la propiedad que el sistema quiere. Es la perilla más elegante de las cinco y la única que mejora dos cosas a la vez. |
| Tamaño del `Pressure Deck` (`n`) | **16** (manual: 8 + 8) | 12-24 | La repetición de contenido por partida y, de rebote, el ritmo: con `q/n` fijo, subir `n` casi no mueve la esperanza pero **ensancha la varianza** y hace que el jugador vea menos veces la misma carta. |
| Espacio de manifestación (`M`) | **no está en el manual** | 3-5 de 8 | Cuánto dura el candado del sacrificio, como fracción de la partida (R7), y por lo tanto cuándo se puede empezar a jugar la estrategia central de [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md). Muy bajo, el candado casi no existe y perder un personaje deja de dar miedo; muy alto, la mitad de la partida se juega con el jefe encima. |
| Espacios por disparo | **1** (manual) | 1 | Subirlo a 2 no acelera el doble: multiplica el efecto de la varianza, porque una racha de símbolos se traduce en saltos de 2 espacios. Sube el techo de derrotas que se sienten injustas. |
| Alcance del remezclado | todo el descarte (manual) | todo / solo las de símbolo | Si volvieran solo las cartas con símbolo, el mazo se iría vaciando de amenazas y el reloj se frenaría solo con el tiempo. Suena a arreglo del problema de escala y en realidad es una rampa de dificultad invertida. |
| Los símbolos excedentes se acumulan | no, se descartan (manual) | no / sí | Si acumularan, las rachas se propagarían de un ciclo al siguiente y el reloj tendría memoria. Sube la varianza y rompe la independencia de ciclos que hace válidas las cuentas de R2 y R3. |
| Frecuencia del chequeo | 1 por turno de personaje (manual) | por turno | Pasarlo a "una vez por ronda" **rompe el auto-balanceo de R6**: el reloj deja de depender de cuántos personajes quedan vivos, y perder gente pasa a ser puro costo sin la compensación de tiempo que las reglas ya te daban. |

## ✅ Criterios de aceptación

1. El `Doom Track` tiene **8** espacios; el marcador arranca en el **1** y la partida se pierde
   en el instante en que llega al **8**. Con el marcador en el 7 la partida sigue.
2. Con exactamente **3** cartas con `Doom Symbol` en el descarte al terminar un turno de
   personaje, el marcador avanza **1** espacio. Con **2**, no avanza y **no se remezcla nada**.
3. Con **5** cartas con símbolo, el marcador avanza **exactamente 1** espacio y el contador
   arranca el turno siguiente en **0**.
4. Después de un avance, el descarte queda en **0 cartas** y el mazo en **16**: vuelven también
   las cartas sin símbolo.
5. En un avance, los efectos del adversario se resuelven **antes** que los de la misión, y
   dentro del adversario en orden de stage 1 → 2 → 3 → final.
6. El orden de las fases de fin de turno es: efectos de fin de turno → `Hazard` → chequeo del
   descarte → chequeo de manifestación → efectos de fin de turno del adversario. Un umbral de
   `Ratchet` cruzado por `Hazard` se resuelve **antes** de que el reloj avance.
7. Si un personaje se pierde en su propio turno con el adversario ya manifestado, se saltean
   las fases restantes **salvo** el chequeo del descarte, y ese chequeo resuelve normal usando
   el espacio donde se perdió.
8. Desde la ronda siguiente, el turno de ese personaje se saltea y **su carta no se roba**: con
   3 personajes vivos se roban **3** cartas por ronda, no 4.
9. El avance que deja el marcador en el espacio 8 termina la partida de inmediato y **no**
   resuelve los efectos de ese avance.
10. El contador de símbolos está visible en pantalla en todo momento como `D` de `T`, sin abrir
    ningún menú, y la pila de descarte se puede inspeccionar carta por carta.
11. Con `P = 4`, `T = 3`, `n = 16` y `q = 12`, una partida completa dura entre **21 y 49**
    turnos de personaje (**5,25 a 12,25** rondas) y **27,5** en promedio. Una build que caiga
    fuera de esa banda falla el check. El valor de `q` es provisional: este criterio se
    recalcula cuando se defina el contenido del mazo.
12. Configurar `T` por encima de la cantidad de cartas con símbolo del mazo (`T > q`) produce un
    **error de configuración explícito** en vez de una partida donde el adversario no avanza
    nunca.
