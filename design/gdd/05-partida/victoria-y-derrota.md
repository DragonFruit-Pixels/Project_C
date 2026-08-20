# 🏁 Victoria y derrota

← [Índice](../README.md)

## Ganar

**Se gana matando el stage final del `Adversary`.** Es la única condición de victoria.

Si varios finales se disparan a la vez, **la victoria tiene precedencia**. Si el último
personaje y el adversario mueren en el mismo momento, ganaste.

## Perder

Se pierde con **cualquiera** de estas tres:

| # | Condición |
|---|---|
| 1 | Un personaje se pierde **antes** de que el `Adversary` se manifieste |
| 2 | **Todos** los personajes se pierden, después de que se manifestó |
| 3 | El marcador llega al **octavo** espacio del `Doom Track` |

"Perderse" es lo mismo por las dos vías: llegar al final de `Wounds` o al final de
`Ratchet`. Las dos barras matan.

## Las dos fases de riesgo

Las condiciones 1 y 2 no son la misma regla escrita dos veces. Son **dos juegos distintos
pegados**, separados por la manifestación del adversario.

### Antes: no puedo perder a nadie

Un solo personaje que caiga **termina la partida**. No hay margen, no hay pérdidas
aceptables, no hay sacrificio.

Durante toda esta fase el jugador tiene que jugar conservador con los cuatro, mientras el
reloj corre y el adversario —que ya puede estar en el mapa— pega sin poder ser respondido.

### Después: bajas aceptables

Manifestado el adversario, la partida sigue mientras **quede alguien vivo**. Perder un
personaje duele muchísimo, pero no termina nada.

Ahí se abre la puerta al [sacrificio deliberado](../02-personaje/trinquete.md): empujar a un
personaje al fondo de su `Ratchet` para convertirlo en el más fuerte del equipo.

## Por qué esto funciona tan bien

El arco de dificultad **no hubo que inventarlo**: sale de dos reglas que ya estaban.

El juego arranca frágil y termina brutal. Al principio sos cuatro personas débiles que no
pueden permitirse un error; al final sos un equipo con builds armadas peleando contra algo
que se fortalece mientras muere.

Y el jugador **controla parcialmente cuándo cruza esa línea**, porque romper el objetivo
adelanta la manifestación. Eso convierte a la mitad 1 en una pregunta estratégica real:

- Romper el objetivo **temprano** te habilita el sacrificio antes y te da más turnos de
  pelea, pero llegás con menos build construida.
- Romperlo **tarde** te deja más tiempo para prepararte, pero el reloj puede llevar al
  adversario al mapa igual, y ahí lo comés sin poder responder mientras seguís trabajando.

Ninguna de las dos es correcta siempre. Eso es exactamente lo que debería hacer una decisión
estratégica.

## Muerte de un personaje

Cuando un personaje se pierde, y la partida continúa:

- Se **descartan** todas sus cartas, tokens y componentes.
- La misión puede definir qué pasa con tokens especiales que estuviera cargando.
- Si murió **durante su propio turno**, se saltean todas las fases restantes **menos** el
  chequeo del descarte de presión. Ese se hace igual: si hay 3 `Doom Symbol`, el adversario
  avanza. Si hace falta saber dónde estaba el personaje para resolver algo, se usa el
  espacio donde murió.
- De ahí en más, **su turno se saltea por completo**.

Ese último punto tiene una consecuencia de ritmo que conviene tener en cuenta: **menos
personajes vivos significa menos cartas de presión por ronda**, o sea que el reloj se
desacelera. Perder gente te hace más débil pero te da más tiempo.

Es un balanceo automático que el diseño original tiene y que conviene no romper al
re-balancear el reloj para 4 personajes.
→ [`../04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)

---

## 📐 Fórmulas

Este sistema tiene poca aritmética y mucho **predicado**: lo que hay que documentar con
rigor es el orden de evaluación de las condiciones de fin. F1, F2 y F3 son eso. F4 y F5 sí
son cuentas, y son las que dicen cuánta partida queda.

### F1 — Procedimiento: evaluación del fin de partida

Se evalúa **cada vez que algo pudo haber disparado un fin**: al resolver una tirada, al
aplicar un efecto de carta, al avanzar el reloj, al revelar un stage. El manual dice
"immediately", así que no se espera al final del turno.

**Entradas:** `stage_final_derrotado`, `pos_marcador`, `manifested`, `muertos`, `vivos`.
**Salidas:** `VICTORIA`, `DERROTA` o `CONTINUAR`.
**Orden de evaluación** — y este orden es la regla, no una preferencia de implementación:

```
1. si stage_final_derrotado          -> VICTORIA   (corta acá, no se evalúa nada más)
2. si pos_marcador >= 8              -> DERROTA
3. si manifested == false y muertos >= 1  -> DERROTA
4. si manifested == true  y vivos == 0    -> DERROTA
5. en cualquier otro caso            -> CONTINUAR
```

El chequeo 1 va primero **porque el manual dice que la victoria tiene precedencia** cuando
varios finales se disparan a la vez. El orden entre 2, 3 y 4 no cambia el resultado —los
tres son derrota— pero sí cambia **qué pantalla de derrota se muestra**, así que conviene
fijarlo igual.

**Ejemplo recorrido paso a paso.** El adversario está en su stage final con 11 heridas sobre
12 de vida. Queda **1 personaje vivo**, con el marcador de `Ratchet` a 1 punto del final del
track y **sin ningún umbral en el medio**. El personaje ataca y saca **2 `Hit` y 1 `Toll`**.
El orden de resolución de la tirada es el de [`../03-resolucion/dados.md`](../03-resolucion/dados.md):
primero `Hit`, después efectos disparados, y al final `Toll`.

1. Se aplican los `Hit`: el stage final llega a 13 heridas sobre 12 → `stage_final_derrotado`
   = true.
2. Se disparan los efectos que dependan del resultado.
3. Se aplica el `Toll`: `Ratchet` +1 llega al final del track, no hay umbral que lo frene →
   el personaje **se pierde** → `vivos` = 0.
4. Se evalúa el fin de partida. Chequeo 1: `stage_final_derrotado` = true → **VICTORIA**.
   Los chequeos 2 a 4 no se corren, aunque el chequeo 4 hubiera dado derrota.

Es el "matás primero y pagás después" del sistema de dados, llevado al último instante de la
partida.

Es exactamente el ejemplo del manual: el adversario y el último personaje caen juntos, y la
partida se gana.

### F2 — Predicado de derrota por personajes

Es **una sola condición con dos ramas**, y la rama la elige `manifested`:

```
derrota_por_personajes =
      ( manifested == false  y  muertos >= 1 )
   o  ( manifested == true   y  vivos   == 0 )

vivos = N - muertos
```

| Variable | Qué es | Rango |
|---|---|---|
| `N` | Personajes al empezar la partida | **4** en este proyecto. El manual recomienda **2** para partidas de un solo jugador |
| `muertos` | Personajes perdidos, por cualquiera de las dos vías | 0 … `N` |
| `manifested` | Si el adversario ya está en el mapa → [`../04-oposicion/objetivo-y-adversario.md`](../04-oposicion/objetivo-y-adversario.md) | true / false |

**Ejemplo resuelto.** `N` = 4.

- `manifested` = false, `muertos` = 1 → `1 >= 1` → **derrota**. Con 3 personajes todavía en
  pie.
- `manifested` = true, `muertos` = 3, `vivos` = 1 → ninguna rama da true → **la partida
  sigue** con un solo personaje.

La asimetría es enorme y es a propósito: el mismo evento —perder un personaje— es fin de
partida o es un martes cualquiera según de qué lado de la manifestación caiga.

### F3 — Predicado de eliminación de un personaje

```
eliminado(c) = ( Wounds_c >= Wounds_max )  o  ( Ratchet_c >= Ratchet_max )
```

| Variable | Qué es | Rango |
|---|---|---|
| `Wounds_c` | Posición del marcador de `Wounds` | 0 … `Wounds_max` |
| `Ratchet_c` | Posición del marcador de `Ratchet` | 0 … `Ratchet_max` |
| `Wounds_max` | Largo del track de `Wounds` | **No está en el manual**: la transcripción describe el track y su final, no cuántos espacios tiene |
| `Ratchet_max` | Largo del track de `Ratchet` | **No está en el manual**. Sí está que **4 de los umbrales** dan un dado bonus permanente, así que hay 4 o más umbrales |

Las dos vías tienen **exactamente la misma consecuencia**. No hay muerte "buena" y muerte
"mala".

Detalle que cambia las cuentas: por la regla de umbrales del trinquete, una pérdida grande
de `Ratchet` **no puede saltear un umbral**. El marcador para en el umbral y el resto se
descarta.

```
Ratchet_c' = min( Ratchet_c + p, siguiente_umbral(Ratchet_c) )
```

**Ejemplo resuelto:** `Ratchet_c` = 6, el siguiente umbral está en 7, y la tirada saca 3
`Toll`. El marcador va a **7**, no a 9: **se descartan 2 puntos** y se resuelve el umbral
completo. O sea que solo se puede morir por `Ratchet` cuando **no queda ningún umbral entre
el marcador y el final del track**.
→ [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md)

### F4 — Turnos que le quedan al reloj antes de la derrota

```
avances_restantes = 8 - pos_marcador
E[turnos_por_avance] = 3 * (M + 1) / (k + 1)
E[turnos_restantes] = avances_restantes * E[turnos_por_avance]
```

| Variable | Qué es | Rango |
|---|---|---|
| `pos_marcador` | Espacio actual en el `Doom Track` | 1 … 8. En **8** se pierde |
| `M` | Cartas del `Pressure Deck` | **16** (8 de la misión + 8 del adversario) |
| `k` | Cuántas de esas 16 llevan `Doom Symbol` | **No está en el manual**: dice que "la mayoría" lo tiene, sin dar el número |
| `3` | `Doom Symbol` necesarios para que avance | **3**, del manual |

La segunda expresión es la esperanza de cuántas cartas hay que robar hasta juntar el tercer
`Doom Symbol` en un mazo de `M` cartas con `k` marcadas, sin reposición. Vale porque el
descarte **se remezcla completo en cada avance**: cada avance arranca de un mazo entero, y
el proceso no tiene memoria de los anteriores.

Como se roba **1 carta por turno de personaje**, esa cuenta está directamente en turnos.

**Ejemplo resuelto.** `pos_marcador` = 5, `M` = 16 y **asumiendo** `k` = 12 (valor de
trabajo, no del manual):
`E[turnos_por_avance]` = 3 · 17 / 13 = **3,92 turnos**.
`avances_restantes` = 8 − 5 = **3**.
`E[turnos_restantes]` = 3 · 3,92 = **11,8 turnos** antes de la derrota por reloj.

Ese número es el que hay que comparar contra los turnos que pide la pelea final
→ [`../04-oposicion/objetivo-y-adversario.md`](../04-oposicion/objetivo-y-adversario.md)

### F5 — Cómo la muerte de un personaje frena el reloj

```
turnos_por_ronda = vivos
rondas_por_avance = E[turnos_por_avance] / vivos
```

Perder gente te debilita **y te compra tiempo**, porque el turno de un personaje perdido se
saltea por completo y esa carta de presión no se roba nunca.

**Ejemplo resuelto**, con `E[turnos_por_avance]` = 3,92 del ejemplo anterior:

| `vivos` | Cartas por ronda | Rondas por avance | Cambio |
|---|---|---|---|
| 4 | 4 | 0,98 | — |
| 3 | 3 | 1,31 | **+33% de rondas** por avance |
| 2 | 2 | 1,96 | **+100%** |
| 1 | 1 | 3,92 | **+300%** |

Con 4 personajes el reloj avanza casi una vez por ronda. Con 1 solo, tarda cuatro. Ese
autobalanceo es gratis y **hay que no romperlo** al re-tunear el reloj para 4 personajes.
→ [`../04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)

---

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| El stage final cae y el último personaje se pierde en el mismo instante | **Victoria.** Es el ejemplo textual del manual. La precedencia no se negocia y no se muestra ninguna pantalla de derrota |
| El stage final cae en el mismo instante en que el marcador llega al espacio 8 | **Victoria**, por la misma precedencia: el chequeo de victoria corre antes que el del reloj |
| Se rompe el `Objective` y en ese mismo turno, **antes** del paso 4 de la fase 4, se pierde un personaje | **Derrota.** Romper el objetivo no protege a nadie: hasta que la manifestación se resuelva al final del turno, `manifested` sigue en false y la condición 1 está activa. Es la trampa más fea del juego y hay que respetarla |
| Un personaje se pierde con el adversario **en el mapa** pero con el `Objective` intacto | **Derrota igual.** Lo que corta la condición 1 es `manifested`, y `manifested` ya es true si la figura está en el mapa por reloj. Con el objetivo intacto e invulnerable, la partida sigue con los que queden |
| Dos personajes se pierden en el mismo instante antes de la manifestación | **Derrota.** Con uno alcanzaba; dos no cambian nada |
| Los cuatro personajes se pierden en el mismo instante después de la manifestación | **Derrota** por la condición 2. `vivos` = 0 |
| Un personaje llega al final de `Wounds` y otro al final de `Ratchet` | Los dos se pierden y las dos vías cuentan igual para las condiciones 1 y 2. El manual no distingue |
| Una tirada de 3 `Toll` con un umbral de `Ratchet` a 1 punto de distancia | El marcador queda **en el umbral**, se descartan los 2 restantes y se resuelve el umbral completo. **El personaje no se pierde**: no se puede morir salteando un umbral |
| Un `Ally` absorbe la herida que hubiera sido letal | El personaje **no se pierde**: las heridas van al `Ally`. Sigue contando como que el personaje recibió el daño para todo efecto de carta. Cuando las heridas del `Ally` igualan su número, se descarta |
| El personaje se pierde **durante su propio turno** | Se saltean todas las fases restantes **menos el chequeo del descarte**. Ese se hace igual: si hay 3 `Doom Symbol`, el adversario avanza. Si hace falta la posición del personaje para resolver algo, se usa **el espacio donde murió**. De ahí en más su turno se saltea por completo |
| El personaje se pierde durante su turno y el chequeo del descarte lleva el marcador al espacio 8 | **Derrota por reloj.** El chequeo del descarte se resuelve igual que si el personaje estuviera vivo, así que un personaje puede morir y llevarse la partida en el mismo turno |
| El personaje se pierde llevando un token especial del objetivo | **No lo resuelve el manual de forma general**: dice que la misión define qué pasa con esos tokens. **Hay que decidirlo por escenario** y dejarlo en la definición del `Objective` |
| Sacrificio deliberado antes de la manifestación | **Derrota inmediata.** La estrategia de empujar un personaje al fondo del `Ratchet` para volverlo fuerte **es ilegal en la mitad 1**: el personaje que se pierde termina la partida. Solo existe después de la manifestación |
| Queda 1 personaje vivo y su `Ratchet` está a un paso del final | La partida sigue. Cada `Toll` que saque es un tiro a la cabeza, pero no hay ninguna regla que lo frene ni que le prohíba tirar: **el jugador no puede elegir tirar menos dados** |
| El marcador llega al espacio 8 con el stage final en 11 de 12 heridas | **Derrota.** No hay tolerancia, no hay último turno de gracia. El manual es explícito: llegar al último espacio del track pierde |
| Dos condiciones de derrota distintas se disparan a la vez | Es **una sola derrota**. No hay diferencia de efecto; lo único que hay que decidir es qué mensaje se muestra, y para eso vale el orden de F1 |

---

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución provisional** declarada
> ahí mismo: `p(Hit) = 0,50`, `p(Toll) = 0,33`.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Cantidad de personajes (`N`) | **4** en este proyecto; el manual recomienda **2** para un jugador | 2 – 4 | Todo a la vez: cuántas cartas de presión se roban por ronda, cuánta build se construye, y cuán frágil es la mitad 1 (con `N` = 4 hay cuatro maneras distintas de perder de golpe). Es la perilla que obliga a re-tunear el reloj → [`../04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md) |
| Muertes tolerables antes de la manifestación | **0** | 0 – 1 | Si toda la mitad 1 se juega a cero errores o admite una pérdida. Subirla a 1 habilita el sacrificio temprano y **borra el arco de dificultad**: las dos fases de riesgo dejan de ser dos juegos distintos |
| Espacios del `Doom Track` | **8** | 8 – 12 | El techo duro de largo de partida. Alargarlo es la salida obvia al problema de 4 personajes, y arrastra la vida de los stages del adversario: más track = más turnos = la pelea final puede pedir más vida para seguir siendo tensa |
| `Doom Symbol` necesarios para avanzar | **3** | 3 – 5 | La velocidad del reloj y su varianza. Subirlo alarga la partida sin tocar el track, pero también **aplana la impredecibilidad**, que es el punto del sistema |
| Cartas con `Doom Symbol` en el mazo (`k` de 16) | **No está en el manual** ("la mayoría") | 10 – 14 de 16 | La varianza del reloj más que su promedio. Bajarlo mucho hace que haya rondas enteras sin presión y el jugador puede planificar; subirlo a 16 lo vuelve un contador fijo cada 3 turnos |
| Cartas de presión por turno | **1** | 1 | Es la unidad de medida de todo el reloj. Cambiarla invalida cualquier tuneo previo de las dos perillas anteriores y rompe el autobalanceo de F5 |
| Precedencia de la victoria en empates | **ON** (gana la victoria) | ON / OFF | Casi no mueve la dificultad —los empates exactos son rarísimos— pero define la fantasía entera del final: "salvaste el mundo y morís" contra "morís y no sirvió de nada". Es una decisión de tono, no de balance |
| Largo del track de `Ratchet` | **No está en el manual** | ≥ 4 umbrales + espacio final | Cuántas veces cruza umbrales cada personaje, o sea cuánta build se construye. **Tocarlo rompe el trinquete**: cambia la cantidad de dados bonus permanentes y de niveles de skill de toda la partida → [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md) |
| Largo del track de `Wounds` | **No está en el manual** | **5–8** ([07-balance](../07-balance/perillas-y-constantes.md)) | Cuántos golpes aguanta un personaje antes de terminar la partida en la mitad 1. Es la perilla más directa sobre la fragilidad de la primera fase, y la más fácil de romper: bajarla un punto puede volver la mitad 1 injugable con `N` = 4 |

---

## ✅ Criterios de aceptación

1. Con el adversario **sin manifestar** y `N` = 4, perder 1 personaje termina la partida en
   **derrota** con los otros 3 en pie.
2. Con el adversario **manifestado** y 3 personajes perdidos, la partida **sigue** y el
   personaje restante juega su turno completo.
3. Derrotar el stage final en el mismo instante en que se pierde el último personaje muestra
   **victoria** y ninguna pantalla de derrota.
4. Derrotar el stage final en el mismo instante en que el marcador entra al espacio 8 muestra
   **victoria**.
5. Con el marcador en el espacio 8, la partida termina en **derrota** aunque el stage final
   tenga 11 heridas de 12.
6. Romper el `Objective` en la fase 1 y perder un personaje en la fase 3 del **mismo turno**
   termina la partida en **derrota**: la manifestación no había llegado a resolverse.
7. Un personaje con `Ratchet` a 1 punto de un umbral que recibe 3 `Toll` queda **en el
   umbral**, con 2 puntos descartados, y **sigue vivo**.
8. Un personaje que llega al final de `Ratchet` cuenta como perdido **igual** que uno que
   llega al final de `Wounds`: las condiciones 1 y 2 no distinguen la vía.
9. Un personaje que se pierde durante su propio turno **sí** ejecuta el chequeo del descarte:
   con 3 `Doom Symbol` en la pila, el marcador avanza 1 espacio y el descarte se remezcla.
10. Después de perder un personaje, la ronda roba **exactamente `vivos` cartas** del
    `Pressure Deck`: con 3 vivos, 3 cartas por ronda y ningún turno fantasma.
11. Todos los componentes, cartas y tokens de un personaje perdido quedan **descartados** y
    no vuelven a estar disponibles para el resto del equipo.
12. La evaluación del fin de partida corre en el instante del evento —no al final del turno—:
    una derrota disparada en la fase 1 no deja jugar las fases 2, 3 ni 4.
