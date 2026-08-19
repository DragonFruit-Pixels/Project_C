# GDD — Project_C

Documento de diseño, mecánicas principales. **Temática sin declarar**: todo el vocabulario
de este documento es neutro y funciona igual para cualquiera de las variantes.

Base mecánica: *Cthulhu: Death May Die* Season 3. Transcripción 1:1 en `docs/rulebook/`.

---

## 1. Qué es el juego

Táctico por turnos, single player. Controlás **4 personajes** que bajan a un mapa a romper
un objetivo antes de que un **adversario** termine de manifestarse. Cuando el objetivo se
rompe, el adversario se vuelve vulnerable y empieza la pelea final.

El juego tiene un reloj que corre solo y no se puede detener. Todo lo que hacés lo acelera.

**La decisión central:** tus personajes se vuelven más fuertes **solo** dañándose en una
barra que nunca se cura. El poder y la muerte salen del mismo lugar.

**Alcance:** 1 misión, 1 adversario. Es la unidad completa del juego, no un recorte.

---

## 2. Vocabulario neutro

Los nombres de este documento son IDs, no texto de jugador. El texto de jugador sale de un
theme pack; los glosarios de `docs/rulebook/` ya probaron que el mismo documento mecánico
soporta vocabularios totalmente distintos sin tocar una regla.

| Concepto | ID neutro | Original DMD |
|---|---|---|
| Personaje jugable | `Character` | Investigator |
| El jefe | `Adversary` | Elder One |
| Enemigo humano, ligado al objetivo | `Servant` | Cultist |
| Todo otro enemigo | `Creature` | Monster |
| Barra que no se cura y da poder | `Ratchet` | Sanity |
| Moneda de reroll | `Reserve` | Stress |
| Daño | `Wounds` | Wounds |
| Nodo del mapa | `Space` | Space |
| Punto de spawn | `Gate` | Gate |
| Reloj de fatalidad | `Doom Track` | Summoning track |
| Símbolo que lo hace avanzar | `Doom Symbol` | Elder One Summoning |
| Mazo de amenazas | `Pressure Deck` | Mythos deck |
| Mazo de recompensas | `Reward Deck` | Discovery deck |
| Carta que dispara en cada umbral | `Ratchet Card` | Insanity card |
| Lo que hay que romper | `Objective` | Ritual |
| Cara de éxito | `Hit` | Success |
| Cara inerte, sirve solo con skill | `Wild` | Elder Sign |
| Cara que cuesta Ratchet | `Toll` | Tentacle |
| Token que daña a fin de turno | `Hazard` | Fire |
| Espacio sin enemigos | `Clear` | Safe space |

---

## 3. El mapa es un grafo

**No es una grilla.** Es un grafo de `Space`. Dos espacios son adyacentes si se puede mover
entre ellos; las paredes son ausencia de arista.

- Las **salas** contienen 1, 2 o 3 espacios. Son la unidad de autoría y de streaming.
- **Escaleras y túneles** son aristas extra entre espacios lejanos.
- **Paredes destructibles**: romper una agrega una arista permanente y cambia todas las
  distancias del mapa.
- **Distancia = camino más corto** (BFS). Todas las reglas de "el más cercano" y "movete N
  hacia X" son consultas sobre el grafo. Los pasajes bloqueados se respetan.
- Sin límite de figuras por espacio.
- El mapa tiene **3 `Gate`** donde spawnean enemigos.
- Un espacio está **`Clear`** cuando no hay enemigos en él.

El mapa tiene una **sección de superficie** y una **subterránea**.

---

## 4. El personaje

Tres barras y tres skills.

| Barra | Se cura | Función |
|---|---|---|
| `Wounds` | sí, con `Recover` | Al final = muerte |
| `Reserve` | sí, con `Recover` | Se gasta para rerollear. Máximo 4 |
| `Ratchet` | **nunca** | **Al final = muerte. Y es la progresión** |

**Skills:** 3 por personaje. La primera es única del personaje; las otras 2 salen de un pool
común de 6. Cada skill tiene **4 niveles**.

---

## 5. El trinquete — la mecánica central

`Ratchet` solo sube. No hay forma de bajarlo. Descansar no lo toca.

El track tiene **umbrales**. Cada vez que cruzás uno:

1. Se ignora el exceso de pérdida (parás justo en el umbral)
2. Se dispara tu `Ratchet Card` — un efecto propio de ese personaje
3. **Subís 1 nivel de un skill a elección**
4. En **4 de los umbrales**, ganás además **+1 dado bonus permanente** para todas tus
   tiradas, salvo las tiradas contra vos mismo

Al final del track, el personaje se pierde y sale del juego.

**Consecuencias de diseño:**

- El jugador **quiere** recibir daño, en esta barra y solo en esta.
- Con 4 personajes en tus manos, podés **quemar a uno a propósito**: acumularle skills y
  dados y usarlo como el que entra a la carnicería mientras los otros quedan limpios. En el
  juego de mesa nadie juega así porque cada jugador defiende a su personaje.
- Esa estrategia tiene un candado temporal, ver §10: perder a alguien antes de que el
  adversario esté en el tablero pierde la partida entera.
- Es también el **motor de rejugabilidad**: con una sola misión, lo que cambia entre
  partidas es qué skills subiste y en qué orden.

---

## 6. Dados

Toda tirada es **3 dados estándar + los bonus que apliquen**. Cuatro caras:

| Cara | Efecto |
|---|---|
| `Hit` | Éxito. Atacando = 1 herida al objetivo |
| `Wild` | Nada, salvo que un skill o carta lo lea |
| `Toll` | **-1 `Ratchet`**. Siempre, en toda tirada |
| `Blank` | Nada |

**`Toll` cuesta en TODA tirada** — atacando, siendo atacado, o tirando por cualquier motivo.
Actuar es intrínsecamente caro. Los dados bonus **no tienen cara `Toll`**.

**Rerolls:** gastás 1 `Reserve` para rerollear 1 dado, las veces que quieras hasta llegar al
máximo. También cuando los enemigos te atacan: **el jugador tira los dados del enemigo**.

**Orden de resolución:** 1) aplicar `Hit` · 2) resolver efectos disparados · 3) aplicar `Toll`.

Los dados se tiran con **física real**.

---

## 7. Secuencia de turno

Un turno por personaje. Los 4 personajes = 4 turnos por ronda.

### 1. Tres acciones

Se pueden repetir. Cuatro base más 2 propias de la misión.

- **`Move`** — hasta 3 espacios. **Todos los enemigos del espacio que dejás te siguen.** Si
  el espacio tenía `Hazard`, te llevás uno por cada token.
- **`Attack`** — 1 enemigo en tu espacio. Tirada. Cada `Hit` = 1 herida.
- **`Recover`** — solo en espacio `Clear`. Curás 3 puntos repartidos entre `Wounds` y
  `Reserve`. **`Ratchet` no se cura nunca.**
- **`Trade`** — intercambio con personajes en tu espacio.

### 2. Carta de presión

Se roba 1 `Pressure Card` y se resuelve de arriba a abajo. Estructura fija de 3 partes:

1. **`Doom Symbol`** — no hace nada ahora, se chequea a fin de turno
2. **Efecto especial** — mover enemigos, dañar, forzar tiradas
3. **Spawn** — invocar enemigos en `Gate` indicadas

**Un paso que no se puede ejecutar se saltea, no falla.**

### 3. Referéndum

- Espacio **`Clear`** → robás una `Reward Card`
- Espacio **con enemigos** → **todos te atacan**, uno por uno

Sin punto medio. Cada turno termina en premio o paliza según si limpiaste tu espacio.

### 4. Fin de turno

1. Efectos de fin de turno
2. **`Hazard`**: 1 dado por token. Cada `Hit` = 1 herida, cada `Toll` = -1 `Ratchet`. Se
   descartan los tokens
3. **Si hay 3+ `Doom Symbol` en el descarte → el adversario avanza**, y el mazo de presión
   se remezcla entero
4. Chequear manifestación del adversario
5. Efectos de fin de turno del adversario

---

## 8. El reloj

El `Doom Track` tiene 8 espacios. El adversario avanza cuando hay **3 `Doom Symbol`** en el
descarte, y ahí **el mazo se remezcla**.

Eso hace que el ritmo sea **variable e impredecible**: nunca sabés si el próximo avance
llega en 3 turnos o en 6. Y como se roba una carta **por turno de personaje**, con 4
personajes el reloj corre al doble de velocidad que la configuración solitaria original.
**Ese es el primer número a rebalancear.**

Si el marcador llega al octavo espacio, se pierde la partida.

---

## 9. Objetivo y adversario

El juego tiene dos mitades.

**Mitad 1 — el objetivo.** El adversario **no puede ser atacado ni dañado**, aunque ya esté
en el mapa. Hay que completar la tarea de la misión. Mientras tanto él sí te puede atacar.

**Mitad 2 — la pelea.** El adversario se manifiesta cuando llega al primer espacio rojo del
track **o** cuando rompés el objetivo, lo que pase primero. Entonces se puede matar.

Tiene **4 stages**. Cada uno tiene vida propia, un efecto al revelarse, un efecto permanente
y **dados que suma a sus ataques**.

- Los stages se matan de a uno; el daño sobrante **no** pasa al siguiente
- **Los efectos y los dados de los stages anteriores quedan en juego** — matarle un stage lo
  hace pegar más fuerte
- Cada reveal es un momento de cámara

---

## 10. Victoria y derrota

**Ganás** matando el stage final.

**Perdés** si:
- Un personaje muere **antes** de que el adversario se manifieste
- **Todos** los personajes mueren después de que se manifestó
- El marcador llega al final del `Doom Track`

Esa primera condición crea dos fases de riesgo: **"no puedo perder a nadie"** antes de la
manifestación, **"bajas aceptables"** después. El sacrificio del §5 solo se habilita en la
segunda.

---

## 11. Skills

6 skills comunes, 4 niveles cada uno. **24 celdas: es la superficie de maestría del juego.**
Ya están especificadas en `docs/rulebook/rulebook.md`, páginas 24-25.

| ID neutro | Original | Qué hace |
|---|---|---|
| `Insight` | Arcane Mastery | Convierte caras `Wild` en éxitos |
| `Brawling` | Brawling | Pega mejor y a más enemigos en tu espacio |
| `Marksman` | Marksman | Ataca a distancia |
| `Stealth` | Stealth | Evita que los enemigos te sigan al moverte |
| `Swiftness` | Swiftness | Más movimiento y más acciones |
| `Toughness` | Toughness | Reduce daño y pérdida de `Ratchet` |

---

## 12. Contenido de una misión

| Elemento | Cantidad |
|---|---|
| Cartas de presión | 16 (8 de la misión + 8 del adversario) |
| Cartas de botín | 15 |
| Cartas de trinquete | 8, se reparte 1 por personaje |
| Stages del adversario | 4 |
| Acciones propias de la misión | 2 |
| Puertas de spawn | 3 |
| `Servant` máximo simultáneo | 10 |

---

## 13. Decisiones abiertas

- **La temática.** Hay dos variantes completas y verificadas en `docs/rulebook/`.
- **Cuál de los dos adversarios.** Con uno solo, ese adversario es la identidad del juego.
- **El rebalanceo del reloj** para 4 personajes.
- **Qué reemplaza los límites de componentes** del juego físico, que funcionaban como tope
  blando de dificultad.
- **Quién puede romper paredes.**
- **Cuántas de las decisiones de "el jugador activo elige" se automatizan** y cuántas se le
  preguntan al jugador. Es el impuesto que hace que los juegos de mesa digitales se sientan
  peores que el cartón.
