# ⚙️ El trinquete (`Ratchet`)

← [Índice](../README.md)

> **Este es el documento más importante del GDD.** Si algo del juego se rediseña, esta
> mecánica es la que no se toca: es lo que lo hace distinto de cualquier otro táctico por
> turnos.

## La regla

`Ratchet` **solo sube**. No hay forma de bajarla. Descansar no la toca, curarse no la toca,
ningún objeto la toca.

Se llama trinquete porque es exactamente eso: un mecanismo que solo gira para un lado.

## Cómo sube

- **1 por cada cara `Toll`** que salga en cualquier tirada. Cualquiera: atacando, siendo
  atacado, o tirando por un efecto. Ver [`03-resolucion/dados.md`](../03-resolucion/dados.md).
- Por efectos de cartas de presión y de recompensa que lo digan.
- Por cada `Toll` al resolver `Hazard` a fin de turno.

## Los umbrales

El track tiene **umbrales** marcados. Cada vez que el marcador cruza uno, pasan cuatro
cosas, en este orden:

**1. Se ignora el exceso.** Si ibas a perder 3 puntos y el umbral estaba a 1, perdés 1 y
los otros 2 se descartan. El marcador **para justo en el umbral**.

**2. Se dispara la `Ratchet Card` del personaje.** Cada personaje recibe una al empezar la
partida, tomada de un mazo de 8. Es un efecto propio que se ejecuta cada vez que cruza un
umbral, así que un personaje con muchos umbrales cruzados la dispara muchas veces.

> **No es puro castigo.** Cada carta es un condicional de dos ramas: castiga o **recompensa**
> según el estado en que estés al cruzar el umbral. Eso convierte el umbral en una decisión
> posicional y no solo en un resultado de dados. Ver
> [`03-resolucion/vocabulario-de-efectos.md`](../03-resolucion/vocabulario-de-efectos.md).

**3. Sube 1 nivel de un skill, a elección del jugador.** Esta es la progresión.

**4. En 4 de los umbrales, gana +1 dado bonus permanente.** Se suma a **todas** sus tiradas
por el resto de la partida, con una excepción: no aplica a las tiradas contra sí mismo.

Al llegar al final del track, el personaje **se pierde y sale del juego**.

## Por qué esto es el juego

**El jugador quiere recibir daño.** En esta barra, y solo en esta.

Es una inversión que casi ningún juego hace. En la mayoría, el daño es puro costo y el
jugador aprende a evitarlo. Acá el daño en `Ratchet` es el **único** camino a subir skills
y ganar dados. Un personaje que juega perfecto y no cruza ningún umbral es un personaje
débil que va a llegar al final de la partida sin herramientas.

Y al mismo tiempo, cada punto lo acerca a perderse. La misma barra es la fuente de poder y
el reloj de muerte.

Eso convierte cada tirada en una pregunta doble: no es solo "¿me sale bien?", es también
"¿cuánto quiero que me cueste?".

## La estrategia que este juego desbloquea

Los 4 personajes son tuyos. Cada uno tiene su propio `Ratchet` independiente.

Entonces podés **quemar a uno a propósito**: llevarlo deliberadamente hacia el fondo del
track para acumularle niveles de skill y dados bonus, y usarlo como el que entra a los
espacios llenos de enemigos mientras los otros tres se mantienen limpios y seguros.

**En el juego de mesa esa estrategia existe pero nadie la juega**, porque cada jugador
controla su propio personaje y ninguno se ofrece de voluntario para ser el sacrificado. Es
un problema social, no de diseño.

Vos no tenés ese problema. **La adaptación a single player no pierde esa estrategia: la
habilita.**

## El candado que la hace interesante

Si el sacrificio fuera gratis, sería la jugada obvia y el juego se rompería. No lo es, por
una regla que ya está en el diseño:

**Si un personaje se pierde antes de que el adversario se manifieste, se pierde la partida
entera.** Después de que se manifiesta, los demás siguen jugando.

Entonces el sacrificio tiene un **candado temporal**: no podés empujar a nadie al fondo del
track hasta que el adversario esté en el tablero. Y como el momento en que aparece depende
del reloj y de vos, el juego te obliga a calcular cuándo se abre esa ventana.

Eso genera dos fases de riesgo completamente distintas dentro de la misma partida, y no hubo
que inventarlas: salen de las reglas que ya estaban.
→ [`05-partida/victoria-y-derrota.md`](../05-partida/victoria-y-derrota.md)

## El motor de rejugabilidad

Con [una sola misión](../00-vision/alcance.md), lo que cambia entre partidas es **qué skills
subiste y en qué orden**. Cada umbral es una decisión de build, y las decisiones tempranas
condicionan a las tardías porque algunos niveles de skill reemplazan al anterior en vez de
sumarse.

El track tiene **6 umbrales**, en las casillas 4, 8, 12, 15, 18 y 19 de 20 — verificado, ver
[`07-balance`](../07-balance/perillas-y-constantes.md). Con 4 personajes eso son **hasta 24
decisiones de build por partida**, y ninguna se puede deshacer.

Y el espaciado **acelera**: los saltos son 4-4-4-3-3-1. Los tres primeros umbrales cuestan 4
puntos cada uno; el último cuesta **1**. O sea que el final del track entrega dos niveles de
skill casi juntos y después mata. El sacrificio deliberado no es una pendiente pareja: es una
rampa que se empina, y el tramo más rentable es también el que no tiene vuelta.

## Riesgo de diseño conocido

**El trinquete no se elige: te pasa.** Sube por resultados de dados, no por una decisión.
En la mesa eso es drama y funciona; en pantalla puede leerse como arbitrario.

El upgrade más grande que el medio digital habilita es **convertirlo en palanca**: hoy
gastás `Reserve` para rerollear, y se podría permitir gastar `Ratchet` también. Ahí lo que
te pasaba pasa a ser algo que decidís pagar.

No está decidido. → [`06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)

## 📐 Fórmulas

### 1. Avance bruto de una tirada

```
ΔRatchet_bruto = cantidad de caras Toll en la tirada, después de todos los rerolls
```

- Solo cuentan los **dados estándar**: los dados bonus no tienen cara `Toll` (manual,
  pág. 11), así que no pueden aportar nada acá.
- Una cara puede traer **dos resultados a la vez** (por ejemplo `Hit` + `Toll`); en ese caso
  aporta 1 al `Toll` igual que si fuera sola.
- Rango en una tirada base de 3 dados estándar: **0–3**. Con dados estándar extra por efecto,
  el techo sube en la misma proporción.

El manual no publica la distribución de caras, pero está **verificada** en
[`07-balance`](../07-balance/perillas-y-constantes.md): `p(Toll) = 2/6`. Entonces

    E[ΔRatchet] = dados_estándar × p(Toll) = 3 × 0,33 = 1,0

**Una tirada base cuesta, en promedio, exactamente 1 punto de `Ratchet`.** Ese número redondo
es la escala de todo el juego: el primer umbral está a 4 puntos, o sea a unas **4 tiradas**, y
el track completo son 20, o sea unas 20 tiradas antes de descontar lo que se descarta en cada
umbral. Los dados bonus no mueven esta cuenta porque no tienen cara `Toll`.

### 2. El algoritmo del umbral

Este es el cálculo central del juego. Se ejecuta **una vez por evento de pérdida**, no una vez
por punto.

```
aplicar_pérdida(P):
    d = pos_umbral_siguiente − pos_actual      # distancia al próximo umbral

    si P < d:
        pos_actual += P                        # avance normal, no pasa nada más
        terminar

    pos_actual = pos_umbral_siguiente          # PARA JUSTO EN EL UMBRAL
    exceso = P − d
    descartar(exceso)                          # el exceso se ignora: NO se guarda
    resolver_umbral(pos_actual)
```

```
resolver_umbral(u):
    1. el exceso ya fue descartado en el paso anterior
    2. disparar la Ratchet Card del personaje
    3. el jugador elige 1 skill y le sube 1 nivel
    4. si u tiene marca de dado bonus → +1 dado bonus permanente
```

- `P` — puntos de `Ratchet` que el evento hace perder. Rango 1–3 en una tirada de 3 dados
  estándar; una `Pressure Card` o `Reward Card` puede pedir cualquier número.
- `d` — casillas hasta el próximo umbral. Rango ≥ 1.
- `exceso` — siempre `max(0, P − d)`. **Nunca se aplica a nada.**
- Salida: nueva `pos_actual`, y 0 o 1 umbral resuelto.

**Ejemplo numérico resuelto.** El marcador está en la casilla 2, el próximo umbral está en la
3, y una tirada saca **3 `Toll`**.

| Paso | Cuenta | Resultado |
|---|---|---|
| `P` | 3 caras `Toll` | 3 |
| `d` | 3 − 2 | 1 |
| ¿`P < d`? | 3 < 1 → no | se cruza umbral |
| `pos_actual` | = 3 | **3, no 5** |
| `exceso` | 3 − 1 | 2, descartados |
| Umbral resuelto | 1 | `Ratchet Card` ×1, +1 nivel de skill ×1, dado bonus solo si la casilla 3 tiene la marca |

Los 2 puntos descartados no se acumulan, no se aplican en el próximo evento, y no acercan el
marcador al umbral siguiente.

### 3. Techo de umbrales por evento y por turno

```
umbrales_por_evento_de_pérdida ≤ 1
umbrales_por_turno            ≤ cantidad de eventos de pérdida del turno
```

Es corolario directo de la fórmula 2: como el marcador **para** en el umbral y el resto se
descarta, ningún evento puede cruzar dos. Pero cada evento se evalúa por separado, así que un
turno con varios eventos sí puede cruzar varios umbrales.

**Ejemplo:** turno con dos eventos. Marcador en 2, umbral A en 3, umbral B en 4.
Evento 1 (ataque, 3 `Toll`): cruza A, marcador queda en 3, se descartan 2.
Evento 2 (resolución de `Hazard` a fin de turno, 1 `Toll`): `P=1`, `d=1`, cruza B, marcador
queda en 4. **Total del turno: 2 umbrales, 2 niveles de skill, 2 activaciones de la
`Ratchet Card`** — con 4 `Toll` tirados en total.

### 4. Pérdida efectiva con reducción

```
P_efectiva = max( 0 , P_bruta − red_R(nivel_Toughness, fuente) )
```

`red_R` sale del codex de skills del manual (pág. 25). Los niveles son **excluyentes**: cada
uno reemplaza al anterior.

| Nivel `Toughness` | `red_R` en ataque enemigo o `Hazard` | `red_R` en cualquier otra fuente |
|---|---|---|
| 1 | 0 (da 1 reroll gratis, no reducción) | 0 |
| 2 | 1 | 0 |
| 3 | 1 | 1 |
| 4 | 1 | 1 |

Si `P_efectiva = 0`, el algoritmo de la fórmula 2 no se ejecuta: no hay avance y no hay umbral.

**Ejemplo:** personaje con `Toughness` nivel 2, marcador a 1 casilla del umbral, recibe un
ataque que saca 1 `Toll`. `P_efectiva = max(0, 1 − 1) = 0`. **No cruza el umbral**, no sube
ninguna skill y no gana el dado bonus. La misma pérdida de 1 `Toll` desde una `Pressure Card`
sí lo cruza, porque el nivel 2 solo cubre ataques y `Hazard`.

Ojo con lo que esto significa: `Toughness` es la única skill que **frena la progresión** del
personaje. Está comprando supervivencia con poder futuro.

### 5. Poder acumulado

```
niveles_de_skill_ganados = umbrales_cruzados
dados_bonus_permanentes  = umbrales_cruzados_con_marca        # techo: 4
dados_de_una_tirada      = 3 + bonus_skills_y_cartas + (dados_bonus_permanentes si la tirada NO es contra sí mismo)
```

- El techo de **4** dados bonus permanentes sale del manual (pág. 8: 4 de los umbrales tienen
  la marca).
- `3` es la tirada base, también del manual (pág. 11).

**Ejemplo:** personaje que cruzó los 4 umbrales marcados y no tiene bonus de skill activo.
Atacando tira **3 + 4 = 7 dados**, de los cuales 4 no pueden sacar `Toll`.

En las tiradas contra sí mismo los 4 bonus **no se suman**, y la cantidad de dados **no es 3**:
sale de otra fuente según el caso.

| Tirada contra sí mismo | Cuántos dados |
|---|---|
| Ataque de un enemigo | Los que diga la ficha de **ese** enemigo. El único bloque visible en el manual es 1 estándar + 2 bonus (pág. 17) — o sea 3 dados, pero **solo 1 puede sacar `Toll`** |
| `Hazard` | **1 dado estándar por token** sobre el personaje (pág. 17). Puede ser 0, 1 o muchos |

La asimetría sigue siendo el premio —el personaje quemado es mucho mejor atacando de lo que es
aguantando— pero no porque tire menos dados defendiéndose, sino porque los 4 bonus solo le
suman cuando ataca.

### 6. Presupuesto de decisiones de build por partida

```
decisiones_de_build = 4 personajes × umbrales_cruzados_por_personaje
```

Con los 4-6 umbrales que asume este GDD, son **16 a 24** decisiones por partida. Aviso
importante: **el manual no dice cuántos umbrales tiene el track**, solo que 4 de ellos dan
dado bonus. El 4-6 es una asunción de este proyecto, no un dato del manual, y hasta que se
mida en el tablero real ese rango es provisional.

### Números que el manual NO da

- **La cantidad total de umbrales del track.** Solo dice que 4 tienen marca de dado bonus.
- **La longitud del track de `Ratchet`** ni el espaciado entre umbrales.
- **La distribución de caras del dado**, y por lo tanto el avance esperado por tirada.
- **Cuántos umbrales cruza un personaje en una partida típica.**

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| 3 `Toll` en una tirada con el umbral a 1 casilla | El marcador **para en el umbral** y los otros 2 puntos se descartan. Se resuelve **1** umbral: 1 activación de `Ratchet Card`, 1 nivel de skill, y el dado bonus solo si ese umbral tiene la marca. |
| 3 `Toll` con el umbral a 3 casillas | El marcador avanza 3 y cae **exactamente** en el umbral. No hay exceso. Se resuelve el umbral igual. |
| 3 `Toll` con el umbral a 5 casillas | El marcador avanza 3 y no pasa nada más. No se resuelve ningún umbral. |
| Dos eventos de pérdida en el mismo turno | Cada uno se evalúa por separado. Se pueden cruzar 2 umbrales en un turno: uno por evento. Lo que no se puede es cruzar 2 con un solo evento. |
| Un `Toll` sale en un dado bonus | Imposible: los dados bonus no tienen cara `Toll` (manual, pág. 11). Si aparece, es un bug de la tabla de caras. |
| Rerollear un `Toll` | Legal, y borra el `Toll`. El `Ratchet` se aplica recién en el paso 3 de la resolución de la tirada, después de **todos** los rerolls: hasta ese momento el `Toll` no cobró nada. |
| La tirada mata al enemigo y cruza umbral | El enemigo muere igual. `Hit` se resuelve en el paso 1 y `Toll` en el paso 3 (manual, pág. 12): matás primero y pagás después. |
| La `Ratchet Card` hace perder `Ratchet` al activarse | **El manual no lo cubre.** Decisión de este proyecto: es un evento de pérdida nuevo, así que puede avanzar el marcador y disparar el umbral siguiente. Hay que ponerle tope de recursión — propuesta: 1 encadenamiento, y el resto se descarta. |
| El personaje cruza umbral y todas sus skills están al tope | **No cubierto por el manual.** Decisión: el paso 3 se pierde sin sustituto, y los pasos 2 y 4 (la `Ratchet Card` y el dado bonus) se resuelven igual. **Con el alcance recortado esto pasa en toda partida completa**, dos veces: 6 umbrales contra 4 subidas — ver F4 en [`skills.md`](skills.md). |
| El evento de pérdida llegaría al final del track | El final del track **no es un umbral**. Como el algoritmo para en cada umbral y descarta el resto, es imposible saltearse un umbral para llegar al final: siempre hay que cruzar el último umbral en un evento y llegar al final en otro. |
| El marcador llega al final del track | El personaje se pierde y sale del juego. Si el adversario **todavía no se manifestó**, se pierde la partida entera en ese instante (manual, pág. 19). |
| El personaje muere por `Wounds` en la misma tirada que traía `Toll` | **No cubierto por el manual.** Decisión: la eliminación se resuelve donde se aplican los `Hit` (pasos 1-2) y los `Toll` del paso 3 ya no se aplican a un personaje que salió del juego. |
| Varios personajes cruzan umbral a la vez (una `Pressure Card` que golpea a todos) | El manual resuelve en sentido horario desde el jugador activo (pág. 8). Traducción a single player: **en orden de turno, empezando por el personaje activo**. Cada uno resuelve sus 4 pasos completos antes de pasar al siguiente. |
| `Toughness` reduce la pérdida a 0 | No hay avance y **no se cruza el umbral**, aunque estuviera a 1 casilla. Es la única forma de esquivar el trinquete, y el precio es no progresar. |
| Un `Recover`, una carta o un efecto parece bajar el `Ratchet` | Es un bug. `Ratchet` no baja nunca por ninguna vía: el manual es explícito en que no se cura descansando (pág. 14) y no da ninguna otra forma. |
| Un personaje cruzó muchos umbrales | La `Ratchet Card` se dispara **una vez por umbral cruzado, sin tope**. Un personaje con 5 umbrales cruzados la activó 5 veces en la partida. |
| Un umbral con marca de dado bonus | El +1 dado se suma a **todas** las tiradas del personaje por el resto de la partida, con la única excepción de las tiradas contra sí mismo: ataque enemigo y resolución de `Hazard`. |
| El jugador quiere posponer la elección de skill | No se puede. El umbral es una resolución atómica: el turno no avanza con una elección de skill pendiente. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Se ignora el exceso al cruzar un umbral | **Sí** (manual, pág. 8 y pág. 28) | No tocar | Es el corazón del sistema. Si se apaga, una tirada de 3 `Toll` puede cruzar dos umbrales, la progresión se dispara y el personaje se pierde en la mitad de los turnos. Todo el ritmo de la partida cuelga de esta regla. |
| Cantidad de umbrales del track | **No está en el manual**; este GDD asume 4-6 | 4–6 | Cuántos niveles de skill llega a comprar un personaje, y con eso las 16-24 decisiones de build de la partida. Es la perilla que más cambia la rejugabilidad. |
| Umbrales con marca de dado bonus | 4 (manual, pág. 8) | 2–5 | El techo de poder de un personaje quemado. Subirla hace que la segunda mitad de la partida sea trivial atacando; bajarla mata la estrategia del sacrificio, porque el premio deja de compensar. |
| Longitud del track de `Ratchet` | **No está en el manual** | Que dé para 4-6 umbrales más margen | Cuántos turnos aguanta un personaje al que se lo está quemando a propósito, y por lo tanto qué tan tarde se puede abrir la ventana de sacrificio. |
| Espaciado entre umbrales | **No está en el manual** | Uniforme, o creciente | El ritmo de la progresión. Espaciado creciente hace que las primeras subidas lleguen rápido y las últimas se paguen caro; uniforme hace la curva plana y previsible. |
| *(métrica, no perilla)* Umbrales que cruza un personaje por partida | Emergente | **Objetivo 4–6** | No se configura: se **mide**. Es el indicador que dice si el resto de las perillas están bien. Por debajo de 4 los personajes llegan al final sin build; por encima de 6 el track es demasiado corto y el sacrificio deja de tener costo. |
| Niveles de skill por umbral | 1 (manual, pág. 8) | 1; 2 solo como modo de dificultad baja | Velocidad a la que se arma la build. Ponerlo en 2 hace que las celdas de skill se agoten antes de la mitad de la partida y la decisión de umbral deje de doler. **Con el alcance recortado ya se agotan al 4.º umbral con el valor 1**, así que esta perilla queda sin margen hacia arriba. |
| `Toll` por cara | 1 (manual, pág. 11) | 1–2 | El costo de agarrar los dados. En 2 el trinquete se vuelve el reloj dominante del juego y desplaza al `Doom Track`. |
| Tamaño del mazo de `Ratchet Card` | 8 (manual, componentes: 8 cartas) | 8–12 | Variedad entre partidas y qué tan distinto se juega cada personaje. Ampliarlo es contenido nuevo, no rebalanceo. |
| Candado de la manifestación del adversario | Perder un personaje antes de la manifestación = derrota (manual, pág. 19) | No tocar | Es lo único que impide que el sacrificio sea la jugada obvia desde el turno 1. Si se saca, el trinquete pasa de ser una decisión a ser un recurso a explotar. |
| Gastar `Ratchet` como moneda de reroll | Apagado — **no está en el manual**, es una propuesta abierta | Off / On | Convierte el trinquete de algo que te pasa en algo que decidís pagar. Toca directamente la economía de `Reserve`: si está encendido, los rerolls dejan de ser un recurso escaso y `Toughness` nivel 1 pierde valor. Ver `06-decisiones/abiertas.md`. |

## ✅ Criterios de aceptación

1. **Se ignora el exceso.** Con el marcador a 1 casilla de un umbral, una tirada con 3 `Toll`
   deja el marcador **exactamente en el umbral** y descarta 2 puntos. El valor final leído es
   el del umbral, no umbral+2.
2. **Un evento, un umbral.** Ese mismo evento resuelve exactamente 1 umbral: 1 activación de
   `Ratchet Card`, 1 elección de nivel de skill, y +1 dado bonus **solo si** ese umbral tiene
   la marca. No hay 2 de nada, en ningún caso, con ningún valor de `P`.
3. **El exceso no se acumula.** Después del criterio 1, el próximo evento de pérdida de 1
   punto avanza el marcador exactamente 1 casilla. Los 2 puntos descartados no reaparecen.
4. **Orden de los 4 efectos, verificable en el log:** (a) exceso descartado, (b) `Ratchet
   Card` activada, (c) elección de nivel de skill, (d) dado bonus otorgado. Si la `Ratchet
   Card` modifica el estado del personaje, ese cambio ya está aplicado cuando se abre la
   pantalla de elección de skill.
5. **La elección de skill bloquea.** No se puede pasar a la siguiente acción ni a la siguiente
   fase del turno con un umbral pendiente de elección de skill.
6. **`Ratchet` es monótono no decreciente.** En un log de 20 turnos con `Recover` repetido,
   cartas reclamadas, enemigos muertos y fin de turno, el valor de `Ratchet` de cada personaje
   nunca es menor que el del frame anterior.
7. **Dos eventos sí cruzan dos umbrales.** Con el marcador a 1 del umbral A y el umbral B a 1
   casilla de A: una tirada con 3 `Toll` cruza A y para; una resolución de `Hazard` posterior
   con 1 `Toll` cruza B. Al final del turno hay 2 umbrales resueltos y 2 niveles de skill
   ganados.
8. **El dado bonus no cobra.** 1000 tiradas simuladas compuestas solo de dados bonus dejan
   `Ratchet` inalterado: 0 avance, 0 umbrales.
9. **El reroll salva.** Rerollear el único `Toll` de una tirada y sacar `Blank` deja `Ratchet`
   igual al valor previo a la tirada.
10. **Se mata antes de pagar.** Una tirada que saca los `Hit` justos para matar a un enemigo y
    además 1 `Toll` que cruza umbral: el enemigo queda muerto y el umbral se resuelve, en ese
    orden, en el mismo log.
11. **`Toughness` frena el umbral.** Con `Toughness` nivel 2 y el marcador a 1 casilla del
    umbral, un ataque enemigo con 1 `Toll` deja `Ratchet` sin cambio y no resuelve el umbral.
    El mismo `Toll` desde una `Pressure Card` sí lo resuelve.
12. **Los 4 dados bonus se suman salvo contra sí mismo.** Un personaje que cruzó los 4
    umbrales marcados tira 7 dados atacando (3 + 4). Cuando lo ataca un enemigo de ficha
    1 estándar + 2 bonus, tira exactamente **3** (los de la ficha, sin sumarle los 4). Con
    2 tokens de `Hazard`, tira exactamente **2**.
13. **El final del track es final.** Con el marcador en la última casilla, el personaje sale
    del juego; si el adversario todavía no se manifestó, la partida termina en derrota en ese
    mismo instante, sin completar el turno.
14. **Una activación por umbral.** Un personaje con 5 umbrales cruzados en la partida tiene
    exactamente 5 activaciones de su `Ratchet Card` en el log, y 5 niveles de skill sumados.
