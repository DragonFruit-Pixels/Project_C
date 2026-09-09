# 🧩 PROPUESTA — A-08: qué 4 personajes y qué 2 skills lleva cada uno

← [Índice](../README.md) · Responde a [A-08](../06-decisiones/abiertas.md#a-08--qué-4-personajes-entran-y-qué-2-skills-lleva-cada-uno)

> ## ⚠️ Esto es una propuesta, no una decisión registrada
>
> No reemplaza nada del GDD ni de `06-decisiones/`. Es el insumo para que el dueño del
> proyecto cierre A-08. Si se aprueba, alguien tiene que: mover A-08 a `registro.md`, actualizar
> el link de A-08 en `skills.md`, `high-concept.md` y `alcance.md`, y dar de alta las 4 filas de
> `DT_Skills`. Nada de eso lo hace este documento.
>
> **Nota aparte, para no confundir a quien lea el historial:** el ID `A-08` ya se usó antes para
> otra decisión (el título del juego, cerrada por
> [D-16](../06-decisiones/registro.md#d-16--el-título-no-bloquea-y-sale-de-la-lista-de-decisiones-de-diseño)).
> El `A-08` vigente hoy —el que responde este documento— es el de personajes y skills, tal
> como está escrito en `abiertas.md` a fecha de este documento.
>
> **Lo que esta propuesta NO reabre**, porque ya está cerrado por
> [D-28](../06-decisiones/registro.md#d-28--2-skills-por-personaje-y-3-niveles-provisional):
> 2 skills por personaje, 3 niveles por skill, techo de build `(3,3)` para las dos. Esta
> propuesta trabaja **dentro** de ese marco, no lo cuestiona.

## 1. Overview

A-08 tiene dos partes. La primera —2 skills por personaje, 3 niveles cada una— ya está cerrada
por D-28. Lo que queda abierto, y lo que responde este documento, es:

1. **Qué 4 personajes** entran al roster.
2. **Qué 2 skills** lleva cada uno, del pool común de 6 (`Insight`, `Brawling`, `Marksman`,
   `Stealth`, `Swiftness`, `Toughness`).
3. La pregunta que `abiertas.md` deja explícita dentro de A-08: **si esas 2 son *firma + 1
   común* o *2 comunes*.**

Esta propuesta cierra la 3.ª pregunta primero, porque condiciona a las otras dos: **2 comunes**,
sin firma. Ver [§3.1](#31-decisión-previa-2-comunes-no-firma--1-común) para el argumento.
Con esa decisión tomada, el resto del documento arma los 4 personajes combinando el pool de 6
sin inventar ninguna habilidad nueva.

## 2. Player Fantasy

Cuatro identidades, cada una respondiendo distinto a la regla que más presiona el juego —los
enemigos pegajosos (pilar 4)— y a la pregunta que abre la partida —qué relación tenés con tu
propia Locura (pilar 1).

| Personaje (codename, no final) | Skills | Fantasía | Problema que resuelve |
|---|---|---|---|
| **Bulwark** | `Brawling` + `Toughness` | Plantarse en la sala llena y aguantarla | Ocupar un espacio saturado sin acelerar la muerte propia |
| **Hunter** | `Marksman` + `Swiftness` | Nunca estar donde pega el enemigo | Resolver una sala sin pagar el costo de entrar a ella |
| **Wraith** | `Stealth` + `Insight` | Entrar, soltar perseguidores, salir con más recursos que con los que entró | Cruzar el mapa sin acumular el problema que arrastra "te siguen" |
| **Ram** | `Brawling` + `Insight` | Lo mismo que Bulwark, sin la red de seguridad | El personaje que se ofrece a quemar cuando hace falta daño cuerpo a cuerpo, no a distancia |

**Codenames, no nombres finales.** Son etiquetas funcionales en inglés para no chocar con
[D-14](../06-decisiones/registro.md#d-14--la-temática-es-biopunk-provisionalmente) —la
temática sigue provisional— ni con la regla de `vocabulario.md` de que un ID nuevo se agrega
ahí antes de usarse en otro lado. Si esta propuesta se aprueba, dar de alta los 4 IDs en
`vocabulario.md` es un paso separado, no incluido acá.

### Por qué cada uno existe como personaje separado, y no como skill de otro

**Bulwark.** `Toughness` reduce la Locura que gana el personaje. Ningún otro rol de esta
propuesta puede compartir esa pieza sin contradecirse: un personaje que también quiere
quemarse rápido (Hunter, Ram) no puede llevar la skill que frena exactamente eso. Y como
`Ratchet` [nunca baja y ningún nivel de skill se puede bajar](../02-personaje/trinquete.md)
—"querés bajar un nivel comprado con umbral: imposible"—, la decisión "¿mitigo o no mitigo mi
propia progresión?" tiene que resolverse **al elegir personaje**, no a mitad de partida. Eso
es lo que hace de Bulwark un personaje y no un ítem.

**Hunter.** Es la otra mitad de una oposición que el propio `skills.md` declara explícita:
`Brawling` y `Marksman` son "las dos respuestas opuestas al mismo problema" (los enemigos
pegajosos). Ponerlas en el mismo personaje anula esa oposición. Hunter tiene que ser distinto
de Bulwark porque estructuralmente responde lo contrario a la misma pregunta: Bulwark entra y
aguanta, Hunter evita entrar.

**Wraith.** Es el único personaje de los cuatro que produce daño **sin tirar un dado de
ataque** (`Stealth` L3, ver [§4](#4-formulas)) y que puede convertir una tirada contra sí
mismo en ganancia de `Reserve` (`Insight`, permitido explícitamente en tiradas propias). Esa
combinación construye una fantasía de "nunca peleo de frente" que un personaje con cualquier
bonus de dados de ataque (`Brawling` o `Marksman`) dejaría de encarnar: la mitad de sus turnos
buenos dejarían de usar la mecánica de esquive porque atacar sería mejor.

**Ram.** Es Bulwark sin la red de seguridad, a propósito: comparte `Brawling` con Bulwark y
reemplaza `Toughness` por `Insight`. Es el contraste explícito, no un descuido — la matriz de
cobertura (§3.3) depende de que este par exista para que la pregunta "¿quién quemo?" tenga una
respuesta con mejor output cuerpo a cuerpo que Bulwark, en vez de que Bulwark sea
estrictamente superior por hacer las dos cosas.

## 3. Detailed Design

### 3.1 Decisión previa: 2 comunes, no firma + 1 común

**Postura: 2 comunes.** No es la opción "seatisfactoria genérica" — es la única que no exige
inventar mecánica nueva, y esta tarea tiene la restricción explícita de no hacerlo.

El propio `skills.md` es terminante sobre esto en sus casos borde: *"Las skills de firma. No
están en el manual transcripto (...) las firmas son contenido a diseñar de cero."* No hay
ni un efecto de firma escrito en ningún lado del proyecto — ni transcripto, ni bosquejado. Usar
firma + 1 común en esta propuesta significaría diseñar **4 habilidades nuevas de 3 niveles
cada una** (12 celdas más), exactamente el tipo de contenido que D-28 ya cortó por volumen:
*"24 celdas de skill (...) es la pieza de contenido más grande del proyecto y la que menos
aporta a demostrar los temas del temario."* Proponer firma acá reabriría con la mano izquierda
lo que D-28 cerró con la derecha.

**Lo que se pierde, dicho en limpio:** una firma es identidad garantizada por construcción —
nadie más la tiene, nunca hay solape. Con 2 comunes, dos personajes *pueden* compartir una
skill (de hecho comparten, ver §3.3), y el riesgo que `abiertas.md` señala es real: *"si se cae
la de firma, los 4 personajes quedan mecánicamente intercambiables."*

**Por qué se acepta igual:** intercambiable significa "mismas dos skills, ambos". Esta
propuesta lo evita por diseño — ningún par de personajes comparte las dos (§3.3) — así que el
riesgo que señala `abiertas.md` queda mitigado sin firma, con la asignación, no con contenido
nuevo. Si en playtest la identidad se siente débil igual, la salida ya está documentada en
D-28 y no cuesta rediseño: agregar 1 firma por personaje reemplazando 1 de las 2 comunes,
manteniendo niveles en 3.

### 3.2 Los 4 personajes, sus 2 skills y sus 3 niveles

Los niveles no se inventan acá — son los que ya especifica `skills.md` (F0-F9) para las 6
skills comunes. Lo que aporta esta sección es **qué par**, y qué significa ese par junto,
personaje por personaje. Todo enlace a la fórmula o línea exacta de `skills.md`.

#### Bulwark — `Brawling` + `Toughness`

| Nivel | `Brawling` | `Toughness` |
|---|---|---|
| 1 | +1 dado atacando en tu espacio ([F1](skills.md#f1--tamaño-de-la-tirada)) | 1 reroll gratis si te atacan o tirás por `Hazard` ([F5](trinquete.md#4-pérdida-efectiva-con-reducción)) |
| 2 | Reparte `S` éxitos entre objetivos elegidos antes de tirar ([F3](skills.md#f3--heridas-repartidas-con-brawling)) | **Reemplaza** L1: reduce 1 `Wound` y/o 1 `Ratchet` por fuente, solo ataque enemigo o `Hazard` |
| 3 | Igual reparto que L2 (el salto a "cada objetivo recibe el total" es L4, fuera de alcance) | Misma reducción, **ahora de cualquier fuente**, no solo ataque/`Hazard` |

Orden recomendado de subida, y por qué importa: `Toughness` a L2 **apaga** el reroll gratis de
L1 (F0, reemplazo). Un jugador que arranca la partida entrando a salas llenas quiere esa
reducción cuanto antes; uno que todavía no se metió en líos puede quedarse en `Brawling` un
umbral más sin perder nada, porque `Brawling` nunca reemplaza — solo suma.

#### Hunter — `Marksman` + `Swiftness`

| Nivel | `Marksman` | `Swiftness` |
|---|---|---|
| 1 | Alcance `d_max = 1` ([F7](skills.md#f7--alcance-y-ataques-por-turno-de-marksman)) | `M = 4` espacios por acción `Move` ([F6](skills.md#f6--economía-de-acciones-y-movimiento-con-swiftness)) |
| 2 | +2 dados atacando fuera de tu espacio (F1). Sigue en `d_max = 1` | `Move` gratis además de las 3 acciones — techo 16 espacios/turno |
| 3 | `d_max = 2` | No suma movimiento — habilita **llevarte a otro personaje** en el `Move` (casos borde de `skills.md`) |

**Todo el par es sumativo** (F0: `Marksman` y `Swiftness` son "A" en las tres filas). No hay
ningún nivel que apague al anterior. Es el único personaje de los cuatro donde la "decisión de
orden" que D-28 señala como debilitada es **literalmente cosmética** en cuanto a poder final:
subir cualquiera de las dos primero no cierra ninguna puerta. La decisión que queda es táctica
—¿qué necesito *este* turno, alcance o movilidad?— no de identidad. Se acepta como el personaje
más simple del roster a propósito: no todos necesitan la misma profundidad, y este absorbe la
simplicidad que D-28 ya aceptó como costo del recorte.

#### Wraith — `Stealth` + `Insight`

| Nivel | `Stealth` | `Insight` |
|---|---|---|
| 1 | 1 esquive por acción `Move` ([F8](skills.md#f8--esquives-de-stealth)) | Convierte 1 `Wild` en éxito, tope 1 por tirada ([F2](skills.md#f2--éxitos-efectivos-con-insight)) |
| 2 | **Reemplaza** L1: 3 esquives por `Move` | **Reemplaza** L1: convierte `Wild` sin tope de cantidad |
| 3 | Suma: cada esquive gastado hace **1 herida sin tirar dado** al enemigo esquivado | Suma: cada `Wild` contada como éxito además **recupera 1 `Reserve`** (tope 4) |

El build final (ambas en L3, garantizado por F4/D-28) hace que Wraith pueda tirar contra sí
mismo a propósito —el manual lo permite explícitamente— y salir de esa tirada con más
`Reserve` del que tenía, al costo de las heridas que esa `Wild` contada como éxito le aplica.
Es el mismo canje que `skills.md` describe en sus casos borde para `Insight` L3, pero acá es
la firma de todo el personaje, no un detalle de una skill.

#### Ram — `Brawling` + `Insight`

| Nivel | `Brawling` | `Insight` |
|---|---|---|
| 1-3 | Igual tabla que Bulwark arriba | Igual tabla que Wraith arriba |

Ram usa las mismas dos tablas que ya están escritas para Bulwark y Wraith — no hay nada nuevo
que definir. Lo que define a Ram es la combinación: entra a la sala llena como Bulwark, pero
sin `Toughness` que le frene la Locura, y compensa con `Insight` sobre sus propias tiradas de
`Hazard` o auto-dirigidas, no sobre las que recibe de un enemigo (`Insight` opera sobre tiradas
que **el personaje mismo** hace, no sobre la tirada de ataque del enemigo).

### 3.3 Matriz de cobertura

| Skill | Eje que cubre | Bulwark | Hunter | Wraith | Ram |
|---|---|---|---|---|---|
| `Insight` | Economía de dados / `Reserve` | | | ✅ | ✅ |
| `Brawling` | Combate cuerpo a cuerpo | ✅ | | | ✅ |
| `Marksman` | Combate a distancia | | ✅ | | |
| `Stealth` | Movilidad / evasión | | | ✅ | |
| `Swiftness` | Economía de acciones | | ✅ | | |
| `Toughness` | Defensa / freno de `Ratchet` | ✅ | | | |

**Cobertura completa:** las 6 skills del pool aparecen al menos una vez. Ningún skill queda
sin dueño.

**Dónde se pisan, dicho explícitamente:**

- `Brawling` (Bulwark, Ram): **mismo verbo de combate, distinta relación con `Ratchet`.**
  Bulwark lo frena, Ram lo acelera. No es el mismo personaje con otro nombre — es el contraste
  declarado en §2. Si en balance `Toughness` resultara demasiado débil (perillas-y-constantes.md
  lo deja en rango 0-1 para `rt`), Bulwark se acercaría a Ram en output sin perder su freno de
  `Ratchet`, y ahí sí valdría revisar si el contraste sigue funcionando. Se anota como riesgo a
  vigilar, no como problema actual.
- `Insight` (Wraith, Ram): **mismo motor de economía de dados, ejes de combate opuestos**
  (evasión pura vs. cuerpo a cuerpo sin mitigación). El riesgo real acá no es el solape entre
  ellos: es el que ya señala `perillas-y-constantes.md` sobre `Insight` en general — "un
  personaje quemado con `Insight` alto es una combinación bastante más fuerte de lo que las dos
  piezas sugieren por separado" — y acá hay **dos** personajes expuestos a esa combinación, no
  uno. Candidato directo para `/balance-check combat` antes de cerrar contenido.
- **Ningún par de personajes comparte las dos skills.** Es la condición que evita la
  intercambiabilidad que `abiertas.md` advierte, y es la que reemplaza a la firma como
  garantía de identidad (§3.1).

**Ejes que quedan en manos de un solo personaje, sin backup:** combate a distancia (solo
Hunter), evasión (solo Wraith), freno de `Ratchet` (solo Bulwark), economía de acciones (solo
Hunter — mismo personaje que combate a distancia, ver abajo). Si Hunter se pierde temprano, el
roster se queda sin nadie que resuelva una sala sin entrar a ella. Es una consecuencia directa
de tener 4 personajes y 6 skills: con 8 casillas de skill y 6 opciones, algo tiene que quedar
concentrado. Se acepta porque Hunter concentrando "a distancia" y "movilidad" es la lectura
más consistente con la fantasía de "nunca estar donde pega el enemigo" — separarlos forzaría
un quinto personaje que no hay presupuesto de contenido para construir.

### 3.4 Alternativas de asignación consideradas, y por qué se descartan

**Alternativa 1 — Repartir `Marksman` y `Stealth` juntos (Hunter = distancia + evasión),
dejar `Swiftness` para otro personaje.** Gana: Hunter deja de depender solo de `Swiftness`
para el eje de movilidad, y gana una herramienta real contra "te siguen" en vez de solo
esquivarlo corriendo más lejos (ver edge case en §5). Cuesta: dos personajes (Hunter, Wraith)
quedarían con evasión, y ninguno con economía de acciones pura — `Swiftness` terminaría
huérfano o forzado en Bulwark/Ram, rompiendo su identidad de "plantado". Se descarta porque
deja **0 skills** cubriendo economía de acciones si no se fuerza en otro lado, y forzarla en
Bulwark contradice su fantasía de personaje que no necesita moverse mucho.

**Alternativa 2 — `Toughness` compartida entre dos personajes en vez de `Brawling`.** Gana:
evita el riesgo anotado sobre `Brawling` compartido (Bulwark vs. Ram, arriba). Cuesta: dos
personajes frenando su propio `Ratchet` reduce a **la mitad** los personajes disponibles para
la estrategia de quemado deliberado que el propio `trinquete.md` marca como el motor de
rejugabilidad de la adaptación a single player. Se descarta porque pisa un pilar de diseño más
grande que el riesgo que evita.

**Alternativa 3 — 5.º personaje o pool ampliado a 7-8 skills para eliminar todo solape.** Gana:
cobertura sin ningún par compartido. Cuesta: contenido nuevo (skills o personaje), exactamente
lo que D-28 cortó por volumen y lo que esta tarea prohíbe inventar. Descartada de entrada.

## 4. Formulas

Todas componen fórmulas ya definidas en `skills.md` y `trinquete.md`. No se define aritmética
nueva — se instancia la existente con el par de skills de cada personaje.

### F-Bulwark — Reparto + mitigación en un espacio saturado

```
N_ataque = 3 + 1 (Brawling L1+) + B_ratchet
Σ w_i = S,  T objetivos elegidos antes de tirar         (Brawling L2/L3 — F3 de skills.md)
P_efectiva_por_fuente = max(0, P_bruta − 1)             (Toughness L3, cualquier fuente — F5 de trinquete.md)
W_final_por_ataque    = max(0, W_bruta − 1)             (Toughness L3, solo ataque/Hazard hasta L2, cualquier fuente en L3)
```
Rango: `S` sale de `N_ataque` tirado (3-9 dados según bonus de `Ratchet` acumulados); `T` va de
1 a la cantidad de enemigos en el espacio.

**Ejemplo.** Bulwark en un espacio con 3 enemigos que lo atacan, cada uno con 1 `Toll` en su
tirada. Bulwark ataca con `S = 4` éxitos y reparte `2/1/1` entre tres objetivos de 2/2/3 vida:
mata a los dos primeros, deja al tercero con 1 de vida — 1 punto de daño se pierde por objetivo
(F3, comportamiento L2/L3, no el reparto total de L4). Cada uno de los 3 ataques recibidos
pierde 1 puntos de `Ratchet` por la mitigación: Bulwark termina el intercambio con **0** puntos
de `Ratchet` ganados en vez de 3, al costo de construir su build más lento que cualquiera de los
otros tres.

### F-Hunter — Alcance + techo de movimiento

```
d_max = 2                                                (Marksman L3 — F7)
Espacios máx./turno = (A + 1[Swiftness ≥ 2]) × M = (3+1) × 4 = 16   (Swiftness L2/L3 — F6)
N_ataque_a_distancia = 3 + 2 (Marksman L2+) + B_ratchet
```
Rango de `N`: 5-9 según `B_ratchet` (0-4).

**Ejemplo.** Con 3 dados bonus de `Ratchet` cruzados y las 3 acciones del turno gastadas en 2
ataques a distancia (sin el ataque gratis de L4, fuera de alcance): cada tirada es
`N = 3 + 2 + 3 = 8` dados, de los cuales solo los 3 estándar por tirada pueden sacar `Toll` — 6
de 16 dados en el turno. Es el mismo patrón que el ejemplo "pico de output" de `skills.md` F7,
sin el multiplicador de L4.

### F-Wraith — Esquive sin dado + economía de `Reserve`

```
s(L3) = 3 esquives por Move                              (Stealth L2/L3 — F8)
Heridas sin dado = 1 × esquives gastados, solo si L ≥ 3
S = H + 1 × W  (m(L3) sin tope)                           (Insight L3 — F2)
Reserve recuperada = min(W, 4)
```

**Ejemplo.** Sale de un espacio con 4 enemigos, gasta los 3 esquives: queda **1** siguiendo,
aplica **3 heridas sin tirar dado**. En la misma secuencia, tira contra sí mismo por `Hazard`
sacando 2 `Hit` y 2 `Wild`: éxitos totales `2 + 2 = 4` (contra sí mismo, así que le pega a
Wraith), y recupera `min(2, 4) = 2` de `Reserve`. El canje es literal: 2 heridas propias a
cambio de 2 `Reserve`.

### F-Ram — Mismo reparto que Bulwark, sin mitigación

```
Σ w_i = S,  T objetivos                                   (Brawling L3 — F3, igual que Bulwark)
P_efectiva = P_bruta                                       (sin Toughness: no hay reducción)
S = H + 1 × W  (Insight L3, aplicado a tiradas propias, no a las del enemigo)
```

**Ejemplo.** Mismo intercambio que el de Bulwark —3 enemigos, 1 `Toll` cada uno— pero sin
mitigación: Ram gana **3** puntos de `Ratchet` completos por el mismo combate donde Bulwark
ganó 0. A cambio, en su propia tirada de `Hazard` de fin de turno puede usar `Insight` para
convertir `Wild` en éxitos y recuperar `Reserve`, algo que Bulwark también tiene disponible si
compartiera la skill, pero que en este roster es exclusivo de Ram y Wraith.

## 5. Edge Cases

| Caso | Qué pasa |
|---|---|
| Se decide quemar a Bulwark de todos modos | Nada lo impide, pero cruza umbrales más lento que los otros 3 por la mitigación de `Toughness` (F5 de `trinquete.md`) — es una elección ineficiente, no ilegal. Sirve como personaje "ancla" que no hace falta sacrificar. |
| Hunter con `Swiftness` alto en el mapa actual | El prototipo en motor hoy tiene 9 espacios de conectividad despareja. El techo de 16 espacios/turno de `Swiftness` L2/L3 (F6) probablemente excede el diámetro real de ese grafo, igual que `skills.md` ya advierte en general ("hay que chequearlo contra el diámetro del grafo del mapa"). Acá el riesgo es concreto porque Hunter es el único que lleva `Swiftness`: si se vuelve irrelevante posicionarlo, ese personaje pierde media identidad. No se propone ningún ajuste numérico acá — es un ítem para `/balance-check` una vez que el mapa final (o el prototipo) tenga diámetro medido. |
| Hunter corre lejos sin `Stealth` | La regla de "te siguen" (`enemigos.md`) no distingue por skill: los enemigos lo acompañan a donde vaya. `Swiftness` no lo libera de perseguidores, solo le permite arrastrarlos más lejos antes de que el turno termine — el mismo patrón de riesgo que el ejemplo de F5 en `enemigos.md` (6 enemigos acumulados en un `Move` de 3 pasos sin `Stealth`). Hunter sin escolta de otro personaje puede terminar cargando un grupo grande sin forma de soltarlo. |
| `Insight` de Wraith y Ram junto con dados bonus de `Ratchet` | Ambos están expuestos al riesgo que ya anota `perillas-y-constantes.md`: la combinación pega más fuerte de lo que sugieren las dos piezas por separado. Con dos personajes así en vez de uno, el riesgo se duplica en superficie de playtest. No se ajusta ningún número acá — es candidato a `/balance-check combat`. |
| `Toughness` de Bulwark sube de rango en un balance futuro | Si `rt` (perillas-y-constantes.md, rango 0-1) se tocara, Bulwark se volvería más parecido a Ram en supervivencia sin perder su freno de progresión. Vigilar si en ese caso el contraste Bulwark/Ram sigue leyéndose como dos personajes distintos o como uno mejor que el otro. |
| Hunter no tiene ningún skill con comportamiento "Reemplaza" (F0) | Es la única excepción a que "el orden de subida importe" en los 4 personajes. Se acepta como el personaje más simple del roster a propósito (ver §3.2) — no es un error, es dónde se puso la simplicidad que D-28 ya aceptó como costo. |
| Un `Ally` da un skill que ningún personaje del roster tiene en ese momento | Sigue la regla general de F9 de `skills.md` sin cambios: lo gana en nivel 1 y pasa a tener 3 skills mientras lo conserve. Esta propuesta no toca esa mecánica. |

## 6. Dependencies

**De qué depende esta propuesta (upstream, ya cerrado, no se toca acá):**

- [`skills.md`](skills.md) — pool de 6 skills, sus 3 niveles y las fórmulas F0-F9. Esta
  propuesta no agrega ni modifica ninguna.
- [`trinquete.md`](trinquete.md) — cadencia de 6 umbrales (4/8/12/15/18/19), la estrategia de
  quemado deliberado y el candado de manifestación del adversario.
- [`../07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md) — 2 skills
  por personaje, 3 niveles por skill, ya fijados por D-28.
- [`../01-fundamentos/mapa-y-espacios.md`](../01-fundamentos/mapa-y-espacios.md) — tamaño y
  grado del grafo, relevante para el edge case de `Swiftness`/Hunter.
- [`../04-oposicion/enemigos.md`](../04-oposicion/enemigos.md) — la regla de "te siguen", que
  es la que le da sentido a `Stealth` y a la oposición `Brawling`/`Marksman`.

**Qué depende de esta propuesta, si se aprueba (downstream):**

- `DT_Skills` — 4 filas de asignación personaje→skill.
- [`../05-partida/contenido-de-una-mision.md`](../05-partida/contenido-de-una-mision.md) —
  inventario de producción por personaje (nombres finales, arte, animaciones).
- [`../06-decisiones/abiertas.md`](../06-decisiones/abiertas.md) y
  [`registro.md`](../06-decisiones/registro.md) — cerrar A-08, con este documento como
  referencia.
- `vocabulario.md` — alta de los 4 codenames como IDs, si se conservan.

**Bidireccionalidad pendiente:** hoy `skills.md`, `high-concept.md` y `alcance.md` linkean a
A-08 como pendiente. Si esta propuesta se aprueba, esos tres documentos deberían actualizar esa
referencia para apuntar acá. Esta propuesta no los edita.

## 7. Tuning Knobs

| Perilla | Default (esta propuesta) | Rango seguro | Qué afecta | Dueño |
|---|---|---|---|---|
| Asignación skill→personaje | Ver tabla de §3.3 | Discreta: cualquier reasignación que cubra las 6 skills y no repita el mismo par en dos personajes | Identidad e intercambiabilidad de los 4 personajes | Este documento |
| Firma vs. 2 comunes | 2 comunes | Firma exige diseñar contenido nuevo, fuera de esta propuesta | Costo de contenido vs. fuerza de identidad garantizada | D-28 + este documento |
| Skills por personaje (heredado, no se toca) | 2 | 2-4 | Ver `perillas-y-constantes.md` | `perillas-y-constantes.md` |
| Niveles por skill (heredado, no se toca) | 3 | 3-5 | Ver `perillas-y-constantes.md` | `perillas-y-constantes.md` |

**Nada de esto toca código.** Como ya señala D-28, la asignación completa son filas de
`DT_Skills`; cambiar de idea sobre qué par lleva cada personaje no requiere retrabajo de
sistemas.

## 8. Acceptance Criteria

1. Cada uno de los 4 personajes arranca con exactamente 2 skills en nivel 1, tomadas del pool
   de 6 sin repetir dentro del mismo personaje, según la tabla de §3.2: Bulwark
   (`Brawling`+`Toughness`), Hunter (`Marksman`+`Swiftness`), Wraith (`Stealth`+`Insight`), Ram
   (`Brawling`+`Insight`).
2. Ningún par de personajes comparte las dos skills. Verificable comparando los 4 conjuntos de
   §3.2: los 4 son distintos entre sí.
3. Las 6 skills del pool aparecen en al menos 1 personaje. Verificable contra la fila "Skill"
   de la matriz de §3.3: ninguna fila queda en blanco.
4. Al menos 3 de los 4 personajes tienen, en su par, alguna skill con comportamiento "Reemplaza"
   en algún nivel (F0 de `skills.md`). Hunter es la excepción documentada y aceptada en §3.2 y
   §5.
5. Con las 4 subidas de nivel disponibles por el recorte de alcance (F4 de `skills.md`), los 4
   personajes llegan a nivel 3 en sus 2 skills al final de una partida completa — consistente
   con el techo `(3,3)` que fija D-28, sin excepción por personaje.
6. `Toughness` es la única skill del roster que reduce la ganancia de `Ratchet` de quien la
   lleva (Bulwark). Ningún otro de los 3 personajes tiene una forma de frenar su propia
   progresión.
7. `Marksman` y `Brawling` nunca aparecen juntos en el mismo personaje del roster propuesto.

## 9. Lo que no se decidió acá, y por qué

- **Nombres y arte finales de los 4 personajes.** Los codenames (`Bulwark`, `Hunter`, `Wraith`,
  `Ram`) son etiquetas de diseño, no nombres de producto. Dependen de la temática, que sigue
  provisional ([D-14](../06-decisiones/registro.md#d-14--la-temática-es-biopunk-provisionalmente)).
- **Contenido de una eventual skill de firma.** Esta propuesta cierra "no usar firma para esta
  versión", pero no diseña ninguna — sigue siendo, tal como dice `skills.md`, contenido a
  construir de cero si algún día se decide usar.
- **Cuáles 4 de los 6 umbrales de `Ratchet` dan dado bonus permanente.** Sigue abierto en
  `perillas-y-constantes.md`, y no se asume nada acá: las fórmulas de §4 usan `B_ratchet` como
  variable, no como valor fijo.
- **El diámetro real del grafo del mapa.** El prototipo actual en motor tiene 9 espacios; el
  target final documentado es 12-32. El edge case de `Swiftness`/Hunter en §5 queda marcado
  como pendiente de medir, no resuelto con un número inventado acá.
- **Qué `Ratchet Card` del mazo de 8 le toca a cada personaje.** Es aleatorio por diseño
  (`trinquete.md`) y no depende de qué skills lleve el personaje.
- **Si el balance de `Insight` compartido (Wraith, Ram) necesita ajuste.** Se deja marcado como
  candidato a `/balance-check combat`, no se toca ningún número en este documento.
