# Export del GDD para ClickUp

Generado por `design/gdd/tools/export-clickup.py`. **No editar a mano**: se
regenera. La fuente de verdad es `design/gdd/`.

19 paginas. Cada bloque separado por una linea de guiones es una pagina;
el titulo del bloque es el nombre exacto que va en ClickUp.


==============================================================================
PAGINA 01 -- Indice
fuente: design/gdd/README.md
==============================================================================

# 📖 GDD — Project_C

Documento de diseño del juego, partido por tema. **Esta carpeta es la fuente de verdad.**

Videojuego en Unreal Engine 5.8, con arquitectura híbrida C++ + Blueprint. Single player, por
turnos, con
presentación 3D. Reimplementa las mecánicas del juego de mesa *Cthulhu: Death May Die*
(Season 3) cambiando la temática.

> **La temática todavía no está decidida.** Todo el vocabulario de estos documentos es
> neutro a propósito: son IDs, no texto de jugador. Hay dos variantes temáticas completas
> y verificadas en `docs/rulebook/`, y el core no debe depender de ninguna.

## Índice

### 🎯 00 — Visión

| Archivo | Qué contiene |
|---|---|
| 🎯 **00-vision/concepto.md** | Qué es el juego y cuál es su decisión central |
| 📐 **00-vision/alcance.md** | 1 misión + 1 adversario, y qué implica ese recorte |

### 🧱 01 — Fundamentos

| Archivo | Qué contiene |
|---|---|
| 🔤 **01-fundamentos/vocabulario.md** | Los IDs neutros y por qué existen |
| 🗺️ **01-fundamentos/mapa-y-espacios.md** | El mapa como grafo, salas, puertas, distancias |

### 🧍 02 — El personaje

| Archivo | Qué contiene |
|---|---|
| 📊 **02-personaje/barras-y-recursos.md** | Wounds, Reserve y Ratchet |
| ⚙️ **02-personaje/trinquete.md** | **La mecánica central del juego** |
| 🎓 **02-personaje/skills.md** | 6 skills × 4 niveles |

### 🎲 03 — Resolución

| Archivo | Qué contiene |
|---|---|
| 🎲 **03-resolucion/dados.md** | Las 4 caras, rerolls y orden de resolución |
| 🔄 **03-resolucion/turno.md** | La secuencia de turno completa |
| 🧩 **03-resolucion/vocabulario-de-efectos.md** | Las piezas componibles de las que están hechas las cartas |

### ⚔️ 04 — La oposición

| Archivo | Qué contiene |
|---|---|
| ⚔️ **04-oposicion/enemigos.md** | Taxonomía, comportamiento y spawn |
| ⏳ **04-oposicion/reloj-y-avance.md** | El Doom Track y por qué es impredecible |
| 👑 **04-oposicion/objetivo-y-adversario.md** | Las dos mitades de la partida |

### 🏁 05 — La partida

| Archivo | Qué contiene |
|---|---|
| 🏁 **05-partida/victoria-y-derrota.md** | Condiciones de fin y las dos fases de riesgo |
| 📦 **05-partida/contenido-de-una-mision.md** | Inventario de contenido a producir |

### 🎛️ 07 — Balance

| Archivo | Qué contiene |
|---|---|
| 🎛️ **07-balance/perillas-y-constantes.md** | **Dueño único de todos los números**: constantes, perillas con rango seguro, la distribución provisional de dados, y lo que el manual no contiene |

### 🧭 06 — Decisiones

| Archivo | Qué contiene |
|---|---|
| 📌 **06-decisiones/registro.md** | Decisiones tomadas, con su razón y su fecha |
| ❓ **06-decisiones/abiertas.md** | Lo que falta decidir y qué está en juego |

## Cómo leer esto

Si venís de cero, el orden es el del índice. Si venís a implementar algo puntual, andá
derecho al archivo del sistema: cada uno se sostiene solo y linkea a los que necesita.

Las **reglas exactas y los números** salen de la transcripción del manual original en
`docs/rulebook/rulebook.md`. Este GDD las reformula y las explica; no las reemplaza. Ante
una diferencia entre los dos, **gana el manual** y hay que corregir el GDD.

## Documentos relacionados

| Ruta | Qué es |
|---|---|
| `docs/rulebook/rulebook.md` | Transcripción forense 1:1 del manual original, 28 páginas |
| `docs/rulebook/glossary-biopunk.md` | Variante temática A, 66 términos |
| `docs/rulebook/glossary-dnd.md` | Variante temática B, 67 términos |
| `design/course-alignment.md` | Matriz de cobertura del temario de la materia y plan de dos fases |
| **../architecture/README.md** | **Cómo se implementa en UE5**: framework, servicios, comunicación y mapa de clases |

## Mantenimiento

**Existe una copia legible para el equipo en ClickUp** (doc `GDD — Project_C`,
`2ky4j7vt-8099`) y tiene que estar **1:1** con esta carpeta: mismas secciones, mismas
reglas, mismos números. La de ClickUp puede estar mejor redactada; no puede decir algo
distinto. Si se cambia una, se cambia la otra.

Reglas para editar:

- **Un tema, un archivo.** Si un archivo empieza a cubrir dos cosas, se parte.
- **Los números viven en un solo lugar**, y ese lugar es
  **07-balance/perillas-y-constantes.md**. Ningún otro
  archivo declara un default ni un rango seguro: linkean a ese. Esta regla se agregó porque el
  problema ya pasó — la misma perilla estaba definida en tres archivos con rangos distintos.
- **Nada de vocabulario temático.** Ni Cthulhu, ni Elder One, ni Mythos, ni nombres de
  ninguna de las dos variantes. Si necesitás nombrar algo nuevo, agregalo al vocabulario.
- **Las decisiones se registran.** Toda decisión de diseño va a `06-decisiones/registro.md`
  con su razón. El "por qué" es lo que se pierde primero.


==============================================================================
PAGINA 02 -- Concepto
fuente: design/gdd/00-vision/concepto.md
==============================================================================

# 🎯 Concepto

## Qué es

Táctico por turnos, single player. Controlás **4 personajes** que bajan a un mapa a romper
un objetivo antes de que un **adversario** termine de manifestarse. Cuando el objetivo se
rompe, el adversario se vuelve vulnerable y empieza la pelea final.

El juego tiene un reloj que corre solo y no se puede detener. Todo lo que hacés lo acelera.

## La decisión central

**Tus personajes se vuelven más fuertes solo dañándose en una barra que nunca se cura.**

El poder y la muerte salen del mismo lugar. No hay una ruta segura que dé progresión: el
único camino para subir skills y ganar dados permanentes es gastar la barra que, al
llenarse, te saca el personaje del juego.

Eso invierte el instinto de casi cualquier otro juego. El jugador **quiere** recibir daño,
en una barra específica, y en la cantidad justa. El detalle está en
**02-personaje/trinquete.md**, que es el documento más
importante de este GDD.

## Los cinco sistemas que cargan el peso

Si algo se recorta, no puede ser ninguno de estos. Son la estructura del juego.

**1. El trinquete.** Tres barras, dos se curan y una nunca. La que no se cura es la que da
poder. → **02-personaje/trinquete.md**

**2. El reloj de ritmo variable.** El adversario avanza cuando se acumulan símbolos en el
descarte, y ahí el mazo se remezcla. Nunca sabés si el próximo avance viene en 3 turnos o
en 6. → **04-oposicion/reloj-y-avance.md**

**3. El referéndum de fin de turno.** Todo turno termina en premio o en paliza, según si
limpiaste tu espacio. Una sola regla que es un loop completo.
→ **03-resolucion/turno.md**

**4. Enemigos pegajosos.** Al salir de un espacio, todos los enemigos que había te siguen.
No hay desengage, y eso convierte el movimiento en la decisión más cara del juego.
→ **04-oposicion/enemigos.md**

**5. El adversario en dos fases que se fortalece muriendo.** No se lo puede tocar hasta
romper el objetivo, y sus stages acumulan dados: matarle uno lo hace pegar más fuerte.
→ **04-oposicion/objetivo-y-adversario.md**

## Qué tipo de experiencia busca

**Desafío y maestría.** No es un juego de narrativa ni de expresión. Lo que el jugador
aprende partida a partida es a leer probabilidad, a administrar una pérdida que no se
puede revertir, y a decidir cuándo conviene empujar y cuándo retirarse.

Con una sola misión, lo que cambia entre partidas es **qué skills subiste y en qué orden**.
El trinquete no es solo la mecánica central: es también el motor de rejugabilidad.

## Qué lo hace distinto

Hay muchos tácticos por turnos y muchos juegos de dados. Lo raro acá es la combinación:

- **El recurso de progresión es también el reloj de muerte del personaje**, y no se puede
  devolver. La mayoría de los juegos con progresión te dejan volver atrás o farmear; acá
  cada punto que ganás es definitivo en las dos direcciones.
- **Los dados cobran siempre.** Una de las cuatro caras te cuesta trinquete aunque estés
  atacando y ganando. Actuar es intrínsecamente caro, y eso hace que "no hacer nada" sea
  una jugada real.
- **Controlás a los cuatro.** Eso habilita una estrategia que el juego de mesa suprime:
  quemar deliberadamente a un personaje para convertirlo en el más fuerte, y usarlo como el
  que entra a la carnicería. Ningún jugador de mesa se ofrece de voluntario para eso.

## Fuera de alcance

Nada de esto está en el juego, y no es un olvido:

- Multijugador y cooperativo. Es single player.
- Combate en tiempo real. Es por turnos.
- Narrativa ramificada, diálogo, elecciones morales.
- Progresión entre partidas que dé poder permanente. La dificultad se sube con reglas
  opcionales, no bajando el desafío con grind.


==============================================================================
PAGINA 03 -- Alcance
fuente: design/gdd/00-vision/alcance.md
==============================================================================

# 📐 Alcance

## La decisión

**1 misión y 1 adversario.**

Eso no es un recorte: es **la unidad atómica del juego original**. El juego de mesa se arma
combinando una caja de escenario con una caja de jefe, una de cada una. Con una misión y un
adversario tenés una partida completa, ganable y perdible, no una porción de algo.

## Qué entra

| Sistema | Estado |
|---|---|
| Los cinco sistemas que cargan el peso | Completos, ninguno se recorta |
| Mapa | 1 layout, con superficie y subterráneo |
| Personajes | 4, controlados todos por el jugador |
| Adversario | 1, con sus 4 stages |
| Objetivo | 1, más las 2 acciones propias de la misión |
| Skills | Los 6, con sus 4 niveles cada uno |

El inventario contado de contenido está en
**05-partida/contenido-de-una-mision.md**.

## Qué cambia respecto del original

**Se pierde la rejugabilidad por combinación.** El juego de mesa rejuega porque son 6
escenarios × 2 jefes = 12 configuraciones distintas. Acá hay una sola.

Lo que queda para que valga volver a jugar:

- El orden de los mazos, que cambia en cada partida
- Qué personajes elegís
- Qué carta de trinquete le toca a cada uno
- **Qué skills subís y en qué orden** ← es la que pesa
- Las reglas opcionales de dificultad

De esa lista, la única que produce partidas realmente distintas es la de los skills. Por eso
**el trinquete** carga con doble función: es la tensión del
juego y es el motor de rejugabilidad.

## La consecuencia de forma

Con una sola misión, el juego no es una campaña. Es **un escenario que se repite, en el que
mejorás y después subís la dificultad**. Esa es la silueta de un roguelike de escenario
único, más cerca de una isla de *Into the Breach* que de una campaña.

No hace falta declararlo como género todavía, pero conviene tenerlo presente al diseñar: si
la misión solo se juega una vez, la mitad del valor del diseño se desperdicia.

## El adversario carga la identidad

Con uno solo, **no hay contraste**. En el juego original los dos jefes son deliberadamente
distintos de textura: uno es un depredador físico, el otro es una anomalía. Elegir uno solo
significa que ese define el tono, el tipo de presión y el recurso especial del juego entero.

Cuál se elige sigue abierto → **06-decisiones/abiertas.md**.

## Alcance vs. la materia

Este proyecto también tiene que cumplir el temario de una cursada, con entregas en fechas
fijas. **El alcance del juego excede el de la materia**: la cursada termina en diciembre y
el juego es multi-año.

La materia es un hito adentro del juego, no el juego. El plan de dos fases atado a las
fechas de los parciales está en **course-alignment.md**, junto
con la matriz que verifica que ningún tema del temario quede sin lugar.


==============================================================================
PAGINA 04 -- Vocabulario neutro
fuente: design/gdd/01-fundamentos/vocabulario.md
==============================================================================

# 🔤 Vocabulario neutro

## Por qué existe

La temática del juego **no está decidida**. Hay dos variantes completas y verificadas en
`docs/rulebook/`, y puede aparecer una tercera.

Si el core del juego usara los nombres de una de ellas, cambiar de temática dejaría de ser
una decisión de arte y pasaría a ser una refactorización. Por eso **el core habla en IDs
neutros y la temática es data**.

Esto ya está probado, no es una hipótesis de diseño. Los dos glosarios temáticos demuestran
que el mismo documento mecánico soporta vocabularios completamente distintos **sin cambiar
una sola regla**, y la equivalencia se verificó línea por línea.

## Cómo se usa

Los nombres de abajo son **identificadores**, no texto de jugador. El texto que ve el
jugador sale de un theme pack: una tabla que mapea cada ID a su nombre y su descripción en
la temática elegida. Los glosarios de `docs/rulebook/` son exactamente la especificación de
ese theme pack.

En código, en Data Tables y en estos documentos se usa siempre el ID.

## La tabla

| Concepto | ID neutro | En el manual original |
|---|---|---|
| Personaje jugable | `Character` | Investigator |
| El jefe | `Adversary` | Elder One |
| Enemigo humano, ligado al objetivo | `Servant` | Cultist |
| Todo otro enemigo | `Creature` | Monster |
| Barra que no se cura y da poder | `Ratchet` | Sanity |
| Moneda de reroll | `Reserve` | Stress |
| Daño | `Wounds` | Wounds |
| Nodo del mapa | `Space` | Space |
| Punto de spawn de enemigos | `Gate` | Gate |
| Reloj de fatalidad | `Doom Track` | Summoning track |
| Símbolo que hace avanzar el reloj | `Doom Symbol` | Elder One Summoning |
| Mazo de amenazas | `Pressure Deck` | Mythos deck |
| Mazo de recompensas | `Reward Deck` | Discovery deck |
| Carta que dispara en cada umbral | `Ratchet Card` | Insanity card |
| Lo que hay que romper | `Objective` | Ritual |
| Cara de éxito | `Hit` | Success |
| Cara inerte, sirve solo con skill | `Wild` | Elder Sign |
| Cara que cuesta `Ratchet` | `Toll` | Tentacle |
| Token que daña a fin de turno | `Hazard` | Fire |
| Espacio sin enemigos | `Clear` | Safe space |
| Acción de curarse | `Recover` | Rest |
| Objeto que acompaña y absorbe daño | `Ally` | Companion |
| Carta negativa persistente | `Burden` | Condition |
| Objeto con habilidad propia | `Trinket` | Relic |

## Los que se dejaron quietos

`Wounds`, `Space`, `Gate` y `Skill` no se renombraron porque ya son genéricos: no arrastran
temática de ningún lado y cualquier reemplazo sería peor.

## Reglas de nomenclatura

- **Un ID nuevo se agrega acá primero**, y recién después se usa en otro documento o en
  código. Si aparece un nombre que no está en esta tabla, es un bug de proceso.
- **Nada de nombres temáticos en el core.** Ni de las variantes existentes ni inventados.
- **Los IDs no se traducen.** Van en inglés incluso en documentos en español, porque son
  los mismos nombres que van a aparecer en Blueprints, Data Tables y Gameplay Tags.
- **El nombre describe la función, no la ficción.** `Ratchet` se llama así porque es un
  trinquete: solo gira para un lado. Ese nombre sigue siendo correcto sea cual sea la
  temática que se elija.

## Las variantes temáticas

Existen y están terminadas, pero **ninguna está elegida**:

| Variante | Glosario | Términos |
|---|---|---|
| A | `docs/rulebook/glossary-biopunk.md` | 66 |
| B | `docs/rulebook/glossary-dnd.md` | 67 |

Las dos vienen con su manual completo aplicado y verificado. Cuando se decida la temática,
el trabajo es mapear esta tabla contra el glosario elegido — no rediseñar nada.


==============================================================================
PAGINA 05 -- Mapa y espacios
fuente: design/gdd/01-fundamentos/mapa-y-espacios.md
==============================================================================

# 🗺️ El mapa y los espacios

## El mapa es un grafo, no una grilla

Esta es la decisión de implementación más importante del mapa, y equivocarla es caro.

El manual original define los espacios **por paredes y pasajes**, y dice explícitamente que
un espacio es adyacente a otro si es posible moverse entre ellos. No hay coordenadas, no hay
casillas cuadradas, no hay vecindad implícita por posición.

Entonces el mapa es un **grafo de nodos `Space`**:

- Un **nodo** es un `Space`.
- Una **arista** es un pasaje transitable entre dos `Space`.
- Una **pared** es la ausencia de arista. No se modela como objeto; simplemente no hay
  conexión.

Una grilla obligaría a inventar reglas para todo lo que sigue. Un grafo lo resuelve gratis.

## Salas

Las **salas** son la unidad de autoría y de streaming, no la unidad de juego. Una sala
contiene **1, 2 o 3 espacios** y expone puntos de conexión hacia otras salas.

En la implementación, cada sala es un **SubLevel**. Eso da tres cosas a la vez: composición
del mapa por armado, streaming para performance, y la posibilidad de que varias personas
trabajen en salas distintas sin pisarse los archivos.

## Adyacencia y distancia

**Adyacente** = existe una arista directa entre dos espacios.

**Distancia** = cantidad de aristas del **camino más corto**. Es un BFS sobre el grafo, y
lo usan un montón de reglas: "el enemigo más cercano", "movete N espacios hacia X",
"el objeto más lejano".

Reglas de distancia que salen del manual y hay que respetar:

- La distancia de un espacio a sí mismo es **0**. Si algo está en tu espacio, es lo más
  cercano que existe.
- Los **pasajes bloqueados** cuentan: el camino no puede atravesarlos.
- **Excepción:** si la figura que se va a mover puede ignorar un bloqueo, entonces ese
  bloqueo tampoco cuenta al calcular quién es "el más cercano" para ella.
- Si hay **empate** entre varios candidatos a la misma distancia, elige el jugador.

> Ese último punto aparece decenas de veces en el manual y es un costo real de la
> adaptación: cada empate es o un prompt al jugador (tedioso) o una regla automática
> (pierde agencia). Está anotado como decisión abierta.

## Aristas especiales

**Escaleras y túneles.** Conectan dos espacios lejanos y los vuelven adyacentes **para
todo efecto**, incluido el cálculo de distancias. En el grafo son simplemente una arista
más. Si el token que las genera se remueve, la arista desaparece.

**Paredes destructibles.** Romper una **agrega una arista permanente** al grafo. Eso cambia
de golpe todas las distancias, las rutas de escape y quién es el enemigo más cercano.

Esto convierte la destrucción en **una mecánica, no en un efecto visual**. Quién puede
romper qué sigue abierto: candidatos son el adversario al cambiar de stage, una criatura
pesada, o una de las 2 acciones propias de la misión.

## Ocupación

**No hay límite de figuras por espacio.** Un espacio puede tener los 4 personajes, seis
enemigos y el adversario a la vez. Eso es deliberado: es lo que hace posible que un espacio
se vuelva una trampa mortal.

## Estado `Clear`

Un espacio está **`Clear`** cuando no hay enemigos en él.

Es un estado calculado, no un flag: se evalúa en el momento en que una regla lo pregunta.
Dos acciones del juego dependen de él —`Recover` y el referéndum de fin de turno— así que
se consulta muchas veces por turno.

## Puertas

El mapa tiene **3 `Gate`**, cada una identificada por color. Son los puntos donde las cartas
de presión invocan enemigos.

Su ubicación es fija en el mapa y conocida por el jugador. Eso es información con la que se
juega: se sabe de dónde va a venir el problema, aunque no cuándo.

## Superficie y subterráneo

El mapa tiene **dos secciones**: una exterior donde arranca la partida y una subterránea
donde está el objetivo.

Es una división de producción además de una de ficción: la superficie se construye con
Landscape y el subterráneo con meshes, y la iluminación de las dos cambia a medida que el
adversario avanza en su reloj.

Las dos temáticas candidatas son sobre descender, así que la estructura funciona para
cualquiera de las dos.

## Resumen para implementar

| Concepto | Estructura |
|---|---|
| `Space` | Nodo del grafo |
| Pasaje | Arista bidireccional |
| Pared | Ausencia de arista |
| Sala | Subgrafo + SubLevel |
| Escalera / túnel | Arista extra, removible |
| Pared destructible | Arista que se agrega en runtime |
| Distancia | BFS respetando bloqueos |
| `Clear` | Consulta, no estado guardado |
| `Gate` | 3 nodos marcados, posición fija |

---

## 📐 Fórmulas

El mapa no tiene aritmética de balanceo: tiene **teoría de grafos**. Todo lo de abajo es
determinista y testeable sin tirar un dado.

Notación común a todas las fórmulas:

| Símbolo | Qué es | Rango |
|---|---|---|
| `V` | Conjunto de `Space` del mapa | 16–48 nodos (16 tiles del manual × 1–3 espacios por tile, p. 9). El total real de la misión **no está en el manual**: lo define la carta de episodio |
| `E_p` | Aristas de pasaje (autoría, estáticas) | del orden de 2.5 × cantidad de nodos |
| `E_t` | Aristas por par de tokens (escalera / túnel) | 0–2 (el manual trae 2 tokens de escalera y 2 de túnel = como máximo 2 pares, p. 2) |
| `E_r` | Aristas creadas en runtime (paredes destructibles) | **0 en el manual** — la mecánica no existe ahí, es propuesta del GDD (A-07) |
| `E_c` | Adyacencias condicionales de un `Trinket` | 0..n, lo define la carta |
| `B` | Pasajes bloqueados en este instante | 0 hasta la cantidad total de aristas; lo define la misión |
| `f` | La figura para la que se evalúa | — |
| `M` | Presupuesto de pasos de una acción `Move` | 3 (manual p. 13), 4 con `Swiftness` nivel 1+ (p. 25) |

---

### F1 — Transitabilidad estructural de un pasaje

```
Transitable(a, b) = Flecha(a → b) AND Flecha(b → a)
```

Booleana. El manual (p. 9) es explícito: si hay flecha de un solo lado, o el pasaje sale del
tablero, **no se puede pasar**. Es un AND, no un OR, y por eso la arista es bidireccional o
no existe.

**Ejemplo.** `S1` tiene flecha hacia el borde norte; el tile de arriba no tiene flecha de
vuelta → `Transitable = false` → no se agrega arista → `S1` y el espacio de arriba **no son
adyacentes** ni para moverse ni para contar distancia.

---

### F2 — Conjunto de aristas efectivo y adyacencia

```
E_mov(f)  = (E_p ∪ E_t ∪ E_r ∪ E_c) menos { e ∈ B : NO IgnoraBloqueo(f, e) }
E_dist(f) = (E_p ∪ E_t ∪ E_r)       menos { e ∈ B : NO IgnoraBloqueo(f, e) }

Adyacente(a, b, f) = (a, b) ∈ E_mov(f)
```

**Son dos grafos distintos, y esa es la trampa.** `E_c` —la adyacencia que un `Trinket`
otorga solo en cierta circunstancia— **habilita el movimiento pero se ignora al calcular el
camino más corto** (manual p. 20, literal). Las aristas de token (`E_t`) sí cuentan para
todo, porque el manual dice "adyacentes **para todo efecto**" (p. 9).

**Ejemplo.** Un `Trinket` vuelve adyacentes `S1` y `S7`. Con él, mover de `S1` a `S7` cuesta
1 paso. Pero si una carta pregunta "el enemigo más cercano", un enemigo en `S7` sigue
midiendo `d = 2`: la arista del `Trinket` no está en `E_dist`.

---

### F3 — Distancia: BFS sobre `E_dist(f)`

```
d_f(a, b) = mínima cantidad de aristas de E_dist(f) entre a y b
d_f(a, a) = 0
d_f(a, b) = infinito  si no hay camino
```

Rango: `0` hasta `cantidad de nodos − 1`, o infinito. Con 48 nodos como techo y grado
promedio 2.5, un BFS completo desde un nodo visita 48 nodos y 120 aristas: es costo
despreciable, se puede correr por consulta sin cachear nada. **No hace falta Dijkstra**:
todas las aristas pesan 1.

Algoritmo, en el orden exacto en que hay que evaluarlo:

1. `dist[a] = 0`, cola = `[a]`, el resto en infinito.
2. Saco `u` de la cola.
3. Para cada `v` con `(u,v) ∈ E_dist(f)` y `dist[v]` en infinito: `dist[v] = dist[u] + 1`,
   encolo `v`.
4. Repito hasta cola vacía.
5. Los que quedaron en infinito son inalcanzables **para esa figura**, en este instante.

**Grafo de ejemplo** (se usa también en F4, F5 y F6):

```
S1 — S2 — S3 — S4 — S5          escalera:  S1 <-> S4
 |                              bloqueo:   pasaje S1 — S2
S6 — S7
```

BFS desde `S1`, para una figura que **respeta** el bloqueo:

| Nodo | `S1` | `S2` | `S3` | `S4` | `S5` | `S6` | `S7` |
|---|---|---|---|---|---|---|---|
| `d` | 0 | **3** | 2 | 1 | 2 | 1 | 2 |

`S2` está a 3 porque hay que dar la vuelta: `S1 → S4` por la escalera, `S4 → S3`, `S3 → S2`.

BFS desde `S1`, para una figura que **ignora** el bloqueo:

| Nodo | `S1` | `S2` | `S3` | `S4` | `S5` | `S6` | `S7` |
|---|---|---|---|---|---|---|---|
| `d` | 0 | **1** | 2 | 1 | 2 | 1 | 2 |

Mismo mapa, mismo instante, **dos tablas de distancia distintas**. Eso no es un bug: es la
excepción del manual (p. 20) y hay que implementarla como parámetro de la consulta, no como
estado del mapa.

---

### F4 — Espacios alcanzables con una acción `Move`

```
R(a, f, M) = { s ∈ V : d_f(a, s) <= M }
```

Con `M = 3`, desde `S1` y respetando el bloqueo, `R` incluye los 7 espacios del ejemplo,
porque incluso `S2` está a 3.

**Pero el movimiento se ejecuta paso a paso, no por salto al destino.** El camino recorrido
**no tiene que ser simple**: el manual habilita explícitamente volver al espacio de origen y
volver a salir (p. 24, notas de `Stealth`). Entonces:

```
Camino válido = (s0, s1, ... sk),  con k <= M  y  (s_i, s_i+1) ∈ E_mov(f) para todo i
```

Los nodos pueden repetirse. `R` sirve para pintar el highlight de destinos posibles; **no
sirve para resolver el movimiento**, porque cada salida de un espacio vuelve a disparar "te
siguen" y `Hazard`.

**Ejemplo.** `S1 → S6 → S1 → S6` es un `Move` legal de 3 pasos que termina donde ya estuviste,
y saliste de `S1` dos veces: los enemigos de `S1` te siguen en el paso 1, y los que hayan
quedado o llegado a `S1` te siguen otra vez en el paso 3.

---

### F5 — El más cercano y el más lejano, con desempate

```
Cercanos(a, T, f) = los t ∈ T con d_f(a,t) mínima, excluyendo los de d = infinito
Lejanos(a, T, f)  = los t ∈ T con d_f(a,t) máxima, excluyendo los de d = infinito

si hay 1 candidato  → automático
si hay 2 o más      → elige el jugador activo
```

`T` es el conjunto candidato (espacios, figuras o tokens del tipo que pida el efecto). Los
inalcanzables **se excluyen del conjunto**: un espacio al que no se puede llegar no es "el
más lejano", es nada.

**Ejemplo con el grafo de F3.** Personaje en `S1`. Enemigo `E1` en `S2`, enemigo `E2` en `S3`.
La carta dice "mové el enemigo más cercano".

- Si `E1` y `E2` **no** pueden cruzar el bloqueo: `d(E1) = 3`, `d(E2) = 2` → gana `E2`.
- Si **sí** pueden cruzarlo: `d(E1) = 1`, `d(E2) = 2` → gana `E1`.

**La misma carta, el mismo tablero, distinto enemigo elegido.** La distancia se mide con los
permisos de la figura que se va a mover, no con los del personaje.

Si en cambio `E2` estuviera en `S5` (`d = 2`) y `E3` en `S3` (`d = 2`): empate a 2, y el juego
**tiene que preguntar**, no autoseleccionar (manual p. 20).

---

### F6 — Empuje de un enemigo hacia un espacio

```
P = un camino más corto de origen(e) al destino, sobre E_dist(e)   (empate: elige el jugador)
pos_final(e) = P[ min(N, d_e(origen, destino)) ]
```

`N` = pasos que ordena la carta. Los ejemplos del manual usan 1 y 2 pasos (p. 15, p. 16); la
**tabla completa de cartas no está en el manual transcripto**, así que el rango real de `N`
sale del diseño del `Pressure Deck`. El enemigo se mueve **todo lo que pueda** y **para al
llegar**: nunca sobrepasa el destino (manual p. 15).

**Ejemplo A.** `E2` en `S3`, destino `S1`, `d = 2`, la carta dice `N = 2`.
`P = (S3, S4, S1)` — sí, por la escalera. `min(2, 2) = 2` → `pos_final = S1`. Llegó.

**Ejemplo B.** `E1` en `S2`, destino `S1`, `d = 3` (respeta el bloqueo), `N = 2`.
`P = (S2, S3, S4, S1)`. `min(2, 3) = 2` → `pos_final = S4`. Queda a distancia 1.

**Ejemplo C.** El mismo `E1`, pero que ignora el bloqueo: `d = 1`, `N = 2`.
`min(2, 1) = 1` → `pos_final = S1`. **Le sobra un paso y no lo usa.**

**Ejemplo D.** `d = infinito` (el enemigo está encerrado detrás de bloqueos que no puede
cruzar). No hay `P`. El paso no se puede ejecutar → **se saltea** y la carta sigue
(manual p. 15: si un paso no se puede hacer, se saltea).

---

### F7 — El estado `Clear`

```
Clear(s) = cantidad de enemigos en s == 0

EsEnemigo(e) = e es Servant  o  e es Creature
               o  (e es Adversary Y el Adversary está manifestado en el mapa)
```

Booleana, **calculada al momento de la consulta**, nunca guardada. El `Adversary` mientras
está en el `Doom Track` y no en el mapa no cuenta: un espacio puede estar `Clear` con el
`Adversary` a mitad del reloj.

**Ejemplo.** `S4` tiene 1 `Servant` con 2 heridas encima. `Clear(S4) = false` → no podés
`Recover`. El personaje lo mata: la figura se remueve, el conteo pasa a 0 → `Clear(S4) = true`
**en la siguiente consulta**, sin recargar nada. Si en la fase 3 el espacio se limpia a mitad
de camino, los enemigos dejan de atacar pero **no** podés robar recompensa: esa consulta se
hizo al empezar la fase (manual p. 16).

---

### F8 — Invariantes de validación del mapa (se corren al cargar)

Esto no es una fórmula de gameplay, es el linter del grafo. Si alguna falla, el mapa está mal
armado y hay que fallar ruidoso en editor, no en runtime.

```
1. Simetría:      (a,b) es arista  <=>  (b,a) es arista
2. Sin bucles:    (a,a) nunca es arista
3. Grado:         1 <= grado(s) <= 6  para todo espacio s
4. Salas:         cantidad de espacios por sala ∈ {1, 2, 3}          (manual p. 9)
5. Puertas:       exactamente 3 Gate, en 3 nodos distintos           (manual p. 9)
6. Conectividad:  d(inicio, s) finita para todo s, sin bloqueos puestos
7. Objetivo:      d(inicio, espacio del Objective) finita para TODA combinación de
                  bloqueos que la misión pueda activar
8. Tokens:        cada token de escalera o túnel tiene exactamente 1 par
```

**Ejemplo de la 7.** El mapa tiene superficie y subterráneo unidos por 2 pasajes, y la misión
puede bloquear pasajes. Si los 2 son bloqueables, existe un estado donde
`d(inicio, Objective) = infinito` y **la partida se vuelve inganable sin que nada avise**. El
invariante lo detecta en editor: la respuesta es dejar al menos un pasaje no bloqueable, o un
par de tokens que cruce las secciones.

---

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| Pasaje con flecha en un solo lado | No existe arista. El destino no aparece como movible y `d` se calcula como si hubiera una pared (manual p. 9) |
| Pasaje que apunta fuera del mapa | Igual que el anterior: no hay arista. No existe "salir del tablero" |
| Distancia de un espacio a sí mismo | `d = 0`. Si hay un candidato en tu propio espacio **gana siempre** el "más cercano", y no hay nada que preguntar (manual p. 20) |
| Empate de distancia entre candidatos | El juego **pregunta al jugador activo**. No autoselecciona ni usa el orden de spawn como criterio de desempate (manual p. 20) |
| Candidato inalcanzable (`d` infinita) | Sale del conjunto candidato: no es "el más cercano" ni "el más lejano". Y como objetivo de ataque a distancia es inválido — el manual dice que un espacio inaccesible por cualquier razón no se puede targetear (p. 24) |
| Pasaje bloqueado en el camino | El camino más corto **no puede atravesarlo**. Excepción: si la figura que se mueve puede ignorarlo, tampoco cuenta al medir quién es el más cercano **para ella** (manual p. 20) |
| Se remueve uno de los dos tokens de un par | La arista desaparece y los espacios **dejan de ser adyacentes** de inmediato (manual p. 9). Un token suelto no conecta nada |
| Se remueve un token con una figura a mitad de la animación de movimiento | El manual no lo cubre, porque en la mesa el movimiento es instantáneo. **Hay que decidir.** Regla propuesta: la validación ocurre al confirmar el paso, la animación es cosmética, y una arista que desaparece después no revierte el paso confirmado |
| Adyacencia condicional de un `Trinket` | Se puede **mover** por ella, pero se **ignora** al calcular caminos y distancias (manual p. 20). Consecuencia concreta: podés estar a 1 paso de un enemigo que el juego reporta a distancia 4 |
| Empuje de `N` pasos con el destino a menos de `N` | El enemigo para al llegar y **descarta los pasos sobrantes**. No rebota, no sigue de largo, no queda "cargado" (manual p. 15) |
| Empuje cuando no hay ningún camino | El paso se saltea y la carta sigue resolviéndose. No es un error (manual p. 15) |
| Varios caminos más cortos de la misma longitud | Todos son válidos y el jugador activo elige por cuál (manual p. 15). Importa de verdad: el camino define de qué espacios sale el enemigo, y por lo tanto a quién deja de acompañar |
| 4 `Character` + 6 `Creature` + el `Adversary` en un mismo `Space` | Legal. **No hay límite de ocupación** (manual p. 9). Ninguna figura se rechaza; el problema es de cámara y de layout de figuras, no de reglas |
| Un `Move` que vuelve al espacio de origen | Legal, y consume pasos igual. Cada salida vuelve a disparar "te siguen" y `Hazard`. El manual lo usa como ejemplo al explicar `Stealth` (p. 24) |
| Un efecto que **coloca** al personaje en otro espacio | No es movimiento: no arrastra enemigos y no agarra `Hazard`. Es la única excepción de la nota del manual (p. 13). En el grafo es un teleport que ignora aristas |
| Pared destructible | **No existe en el manual.** Es propuesta del GDD y sigue abierta (A-07). Definido acá: romperla agrega una arista permanente e invalida todo cache de distancia del frame |
| Se rompe una pared a mitad de una acción `Move` | El manual no lo cubre porque la mecánica no existe ahí. **Regla propuesta:** los pasos ya confirmados quedan como están y los pasos restantes se recalculan sobre el grafo nuevo |
| Spawn en una `Gate` que ya tiene 5 enemigos | Se apila sin problema. La única razón para que un spawn falle es que no haya figura en el pool → **04-oposicion/enemigos.md** |
| El espacio se limpia a mitad de la fase 3 | Los enemigos dejan de atacar, pero **no** se roba recompensa: lo que decide es el estado al **empezar** la fase (manual p. 16) |
| `Adversary` todavía en el `Doom Track`, no manifestado | No cuenta como enemigo → el espacio puede estar `Clear` (manual p. 7) |
| Dos secciones del mapa unidas por un único pasaje bloqueable | Existe un estado con el objetivo a distancia infinita. El invariante F8.7 lo tiene que rechazar **en editor**. El manual no lo cubre: es un problema que aparece porque nosotros elegimos armar el mapa por salas |

---

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.

| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Espacios por sala | 1–3 (manual p. 9) | 1–4 | La granularidad del movimiento. Salas de 1 espacio hacen que 3 pasos cubran 3 salas y el mapa se sienta chico; salas de 3 hacen que cada sala sea un problema en sí misma |
| Total de `Space` del mapa | **No está en el manual** (depende del episodio; el techo físico son 16 tiles). Propuesta: 20 | 12–32 | El diámetro del mapa, y por lo tanto cuánto tarda en llegarte un enemigo empujado. Abajo de 12 no hay a dónde escapar y "te siguen" deja de amenazar, porque ya estás rodeado siempre |
| Grado promedio del grafo | **No está en el manual.** Propuesta: 2.5 | 2.0–3.5 | Cuántas rutas de escape hay. Arriba de 3.5 el mapa es una malla y arrastrar enemigos deja de doler; abajo de 2.0 es un pasillo y cualquier bloqueo parte el mapa en dos |
| Cantidad de `Gate` | 3 (manual p. 9) | 2–4 | Cuán repartida y cuán predecible es la presión. **Tocarla rompe el formato de las cartas de presión**, que nombran una `Gate` por color → **04-oposicion/enemigos.md** |
| Distancia mínima `Gate` ↔ espacio inicial | **No está en el manual.** Propuesta: 3 | 2–5 | Cuántos turnos de gracia hay al arranque. A 1, el primer spawn está encima antes de que exista una sola skill |
| Presupuesto de pasos de `Move` | 3 (manual p. 13) | 2–4 | Todo. A 4 el kiting se vuelve viable, los enemigos no alcanzan nunca y el tope blando de dificultad por pools deja de morder; a 2 no llegás al objetivo antes de que termine el reloj |
| Pares de aristas por token (escalera / túnel) | Hasta 2 (manual p. 2: 2 tokens de cada tipo) | 0–3 | El diámetro efectivo del mapa. **Cada par colapsa distancias de forma global**, no local: agregar uno recalcula quién es "el más cercano" en medio mapa |
| Bloqueos simultáneos activos | **No está en el manual** (lo define la misión) | 0–3 | Cuánto se puede canalizar el movimiento. Arriba de 3, el riesgo de violar el invariante F8.7 —objetivo inalcanzable— se vuelve alto |
| Paredes destructibles por partida | **0: no existen en el manual** (A-07) | 0–3 | Si la destrucción es mecánica o decorado. Cada una invalida caches de distancia y cambia rutas de escape a mitad de partida. Arriba de 3, el mapa deja de ser memorizable y planificar pierde valor |
| Límite de figuras por `Space` | Sin límite (manual p. 9) | Sin límite, o 8 como mínimo si se pone uno | Si un espacio puede volverse una trampa mortal. **Poner un límite rompe dos sistemas**: "te siguen" no tendría a dónde meter a los enemigos, y el spawn necesitaría una regla de overflow que el manual no tiene |
| Secciones del mapa | 2, superficie y subterráneo (decisión del GDD, no del manual) | 2–3 | Producción antes que juego: cada sección es un set de iluminación y de meshes. Como juego, define cuántos cuellos de botella obligatorios hay en la ruta al objetivo |

---

## ✅ Criterios de aceptación

1. Un pasaje con flecha en un solo lado **no** genera arista: el destino no se ofrece como
   movible y el grafo serializado del mapa tiene 0 aristas cruzando ese lado.
2. `d(s, s) = 0` para todos los `Space` del mapa, y un candidato que está en el espacio del
   personaje gana el "más cercano" sin abrir prompt de desempate.
3. Con el grafo de F3 y el pasaje `S1–S2` bloqueado, la consulta de distancias desde `S1`
   devuelve exactamente `S1:0, S2:3, S3:2, S4:1, S5:2, S6:1, S7:2`.
4. La misma consulta, para una figura marcada como "ignora bloqueos", devuelve
   `S1:0, S2:1, S3:2, S4:1, S5:2, S6:1, S7:2`, sin recargar el nivel y sin mutar el grafo.
5. Removido uno de los dos tokens del par escalera `S1 / S4`, la siguiente consulta devuelve
   `d(S1, S4) = 3` y `S4` deja de aparecer como destino a 1 paso de `S1`.
6. "Mover el enemigo más cercano 2 espacios hacia mí", con un enemigo a `d = 4`, lo deja a
   `d = 2` sobre un camino de longitud 4 válido.
7. El mismo efecto con el enemigo a `d = 1` lo deja **en el espacio del personaje** y no
   consume el segundo paso: la posición final es el destino, nunca un espacio más allá.
8. Con dos enemigos a `d = 2`, el juego abre selección y **no** mueve ninguno hasta que el
   jugador elija; con un solo enemigo a `d = 2` no abre nada.
9. Un `Trinket` que declara adyacencia condicional entre `S1` y `S7` permite un `Move` de 1
   paso entre ellos, y en la misma partida la consulta "enemigo más cercano" devuelve el
   mismo enemigo con el `Trinket` activo y sin él.
10. Con 4 `Character`, 6 `Creature` y el `Adversary` manifestado en un mismo `Space`: ninguna
    colocación se rechaza, `Clear = false`, y la fase 3 encola 7 ataques contra el personaje
    activo.
11. En un `Space` con 0 enemigos, `Recover` está habilitado; al entrar 1 enemigo queda
    deshabilitado en la siguiente consulta, sin recargar el nivel ni el mapa.
12. El mapa cargado pasa los 8 invariantes de F8 —3 `Gate` en nodos distintos, toda sala con
    1 a 3 espacios, todos los tokens en pares, y `d(inicio, s)` finita para todo `Space`— y
    el chequeo falla ruidoso en editor si se rompe alguno.


==============================================================================
PAGINA 06 -- Barras y recursos
fuente: design/gdd/02-personaje/barras-y-recursos.md
==============================================================================

# 📊 Barras y recursos

Cada `Character` tiene **tres barras** y **tres skills**. Las skills están en
**skills.md**; acá van las barras.

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
**05-partida/victoria-y-derrota.md**, y son más graves
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
→ **trinquete.md**

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
| Se acaban los tokens de `Hazard` del tablero | **No se emula el límite: no hay tope.** El manual limita los del tablero al stock físico (12 en esta caja) y ya no limita los que están sobre el personaje. Es una restricción de cartón, no de diseño, y replicarla en digital sería copiar un defecto. Consecuencia aceptada y anotada: **el juego queda más duro que la mesa** y esa dureza se compensa con las perillas de **07-balance**, no reponiendo el tope. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
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


==============================================================================
PAGINA 07 -- Trinquete
fuente: design/gdd/02-personaje/trinquete.md
==============================================================================

# ⚙️ El trinquete (`Ratchet`)

> **Este es el documento más importante del GDD.** Si algo del juego se rediseña, esta
> mecánica es la que no se toca: es lo que lo hace distinto de cualquier otro táctico por
> turnos.

## La regla

`Ratchet` **solo sube**. No hay forma de bajarla. Descansar no la toca, curarse no la toca,
ningún objeto la toca.

Se llama trinquete porque es exactamente eso: un mecanismo que solo gira para un lado.

## Cómo sube

- **1 por cada cara `Toll`** que salga en cualquier tirada. Cualquiera: atacando, siendo
  atacado, o tirando por un efecto. Ver **03-resolucion/dados.md**.
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
> **03-resolucion/vocabulario-de-efectos.md**.

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
→ **05-partida/victoria-y-derrota.md**

## El motor de rejugabilidad

Con **una sola misión**, lo que cambia entre partidas es **qué skills
subiste y en qué orden**. Cada umbral es una decisión de build, y las decisiones tempranas
condicionan a las tardías porque algunos niveles de skill reemplazan al anterior en vez de
sumarse.

El track tiene **6 umbrales**, en las casillas 4, 8, 12, 15, 18 y 19 de 20 — verificado, ver
**07-balance**. Con 4 personajes eso son **hasta 24
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

No está decidido. → **06-decisiones/abiertas.md**

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
**07-balance**: `p(Toll) = 2/6`. Entonces

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
| El personaje cruza umbral y todas sus skills están en nivel 4 | **No cubierto por el manual.** Decisión: el paso 3 se pierde sin sustituto, y los pasos 2 y 4 (la `Ratchet Card` y el dado bonus) se resuelven igual. |
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
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
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
| Niveles de skill por umbral | 1 (manual, pág. 8) | 1; 2 solo como modo de dificultad baja | Velocidad a la que se arma la build. Ponerlo en 2 hace que las 24 celdas de skill se agoten antes de la mitad de la partida y la decisión de umbral deje de doler. |
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


==============================================================================
PAGINA 08 -- Skills
fuente: design/gdd/02-personaje/skills.md
==============================================================================

# 🎓 Skills

## Estructura

Cada `Character` tiene **3 skills**:

- La primera es su **skill de firma**, única de ese personaje.
- Las otras **2 salen de un pool común de 6**, así que dos personajes pueden compartirlas.

Cada skill tiene **4 niveles**. Todos arrancan en nivel 1.

**Se sube exactamente de una forma: cruzando un umbral de
**Ratchet**.** Cada umbral da 1 nivel, a elección del jugador.

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

Es la misma fórmula de **../03-resolucion/dados.md**, acotada al
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
  **barras-y-recursos.md**. El manual no da el largo de esa barra.

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
(**../01-fundamentos/mapa-y-espacios.md**): si el mapa
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
**../01-fundamentos/vocabulario.md**. Acá se la nombra en
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
| Dos efectos quieren convertir la misma cara | Prohibido. Una cara convertida deja de contar como la original y no se puede convertir dos veces (**../03-resolucion/dados.md**). |
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
| `Swiftness` 3: te llevás un personaje y ese personaje deja enemigos atrás | **No cubierto por el manual.** Decisión: aplica la regla general de **../03-resolucion/turno.md** — al personaje llevado los enemigos **lo siguen** y **agarra `Hazard`** del espacio que deja, porque no está haciendo su propia acción `Move` pero tampoco está siendo *colocado*. |
| `Swiftness` 3 con L4: la decisión de llevar o no | Es **independiente en cada salida** de la misma acción `Move` (p.25, con ejemplo). |
| `Swiftness` 2: cuándo se puede usar el `Move` gratis | Solo en la fase de acciones, antes o después de las otras (p.13, free actions). No se puede guardar para otra fase. |
| `Toughness` 2/3 y 3 enemigos atacándote | Son **3 fuentes distintas**: la reducción aplica **a cada ataque**, no una vez por turno. La tirada completa de `Hazard`, en cambio, cuenta como **una** sola fuente. |
| `Toughness` reduce un `Toll` que iba a cruzar un umbral | El `Toll` reducido **nunca se aplica**, así que el umbral no se cruza. Primero se reduce, después se aplica, y recién ahí el marcador para en el umbral. La reducción y el descarte del exceso del umbral **no se acumulan**. |
| La reducción supera lo recibido | `max(0, …)`: nunca es negativa y **no cura nada**. |
| Las skills de firma | **No están en el manual transcripto.** Cada personaje tiene 1 firma más 2 comunes, y las firmas están impresas en los tableros de personaje, que la transcripción no incluye. Todo lo de arriba cubre las 6 comunes; las firmas son contenido a diseñar de cero. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
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
| Acciones extra de `Swiftness` L4 | 1 | 0-1 | El output total del turno, ~+33%. **Rompe el reloj**: el `Doom Track` avanza por turno, no por acción, así que más acciones por turno es más juego por unidad de reloj. Ver **../04-oposicion/reloj-y-avance.md**. |
| Reducción de `Wounds` de `Toughness` L4 | 2 | 1-3 | Cuánto castigo aguanta un personaje parado en un espacio lleno. En 3, la mayoría de los ataques del manual hacen 0. |
| Reducción de `Ratchet` de `Toughness` L2-L4 | 1 | 0-1 | **La perilla más delicada del set.** En 0, `Toughness` deja de frenar la progresión y deja de ser una decisión interesante. En 2, un personaje con `Toughness` 4 casi no sube `Ratchet`, no cruza umbrales y nunca construye build: rompe **trinquete.md** de raíz. |
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


==============================================================================
PAGINA 09 -- Dados
fuente: design/gdd/03-resolucion/dados.md
==============================================================================

# 🎲 Dados

## La tirada base

Toda tirada es **3 dados estándar + los dados bonus que apliquen**.

El jugador **no puede elegir tirar menos** dados de los que le corresponden, salvo que el
efecto sea explícitamente opcional. No se puede jugar a lo seguro tirando menos.

## Las cuatro caras

| Cara | Efecto |
|---|---|
| `Hit` | Éxito. Atacando, es 1 herida al objetivo |
| `Wild` | Nada, salvo que una skill o carta la lea |
| `Toll` | **+1 `Ratchet`**. Siempre, en toda tirada |
| `Blank` | Nada |

Algunas caras traen **dos resultados a la vez** — por ejemplo `Hit` y `Toll` juntos. Se
aplican los dos.

## La regla que define el juego

**`Toll` cobra en TODAS las tiradas.** Atacando, defendiéndote, o tirando por cualquier
motivo.

Esto es lo que hace que actuar sea intrínsecamente caro. No existe una acción gratis: cada
vez que agarrás los dados estás aceptando la posibilidad de acercarte al final del track. Y
como el track también es tu progresión, la pregunta nunca es solo "¿me sale?".

Consecuencia directa: **no hacer nada es una jugada legítima**. Un turno donde te movés a un
espacio limpio y descansás es un turno donde no tiraste dados, y eso vale.

## Dados bonus

Vienen de tres lados: skills, cartas, y los umbrales de
**Ratchet** que dan uno permanente.

**Los dados bonus no tienen cara `Toll`.** Son puro beneficio.

Eso hace que el poder que ganás con el trinquete sea doblemente bueno: no solo tirás más
dados, tirás dados que no te pueden cobrar. Un personaje muy avanzado en el track tiene una
proporción de dados seguros mucho mejor que uno limpio.

**No hay límite** de dados en una tirada.

**Excepción importante:** los dados bonus permanentes del trinquete **no aplican a las
tiradas contra vos mismo**. Ver abajo.

## Tiradas contra vos mismo

Algunas tiradas son **en tu contra**: cada `Hit` te perjudica en vez de ayudarte. Los dos
casos son el ataque de un enemigo y la resolución de `Hazard`.

En esas tiradas los dados bonus permanentes **no se suman**, porque agregarlos sería
agregarte más chances de que te peguen. Los efectos y skills sí se aplican, salvo que digan
lo contrario.

Los dados de los enemigos **los tira el jugador**. No hay un lado de la mesa que tire por
la oposición: todo pasa por tus manos, incluido lo que te lastima.

## Rerolls

Gastás **1 `Reserve` para rerollear 1 dado**, ignorando su resultado original. Se puede
repetir todas las veces que quieras mientras te quede `Reserve`.

Funciona en **cualquier** tirada, incluidas las de los enemigos atacándote.

Algunas skills dan **rerolls gratis**, que no cuestan `Reserve`.

Los rerolls son la única herramienta del jugador contra la varianza, y son limitados. La
tensión real del sistema es que `Reserve` se gasta en el momento pero se recupera solo
descansando, y descansar exige un espacio limpio.

## Orden de resolución

Cuando terminaste de rerollear, se cuentan los resultados y se aplican **en este orden**:

**1. `Hit`.** Atacando, son heridas al objetivo. Si un efecto te pide "hacé una tirada",
seguí lo que diga la carta: a veces hay que igualar o superar un número, a veces cada `Hit`
te deja hacer algo.

**2. Efectos disparados.** Todo lo que dependa del resultado: "si sobrevive al ataque…",
"cuando el enemigo muere…".

**3. `Toll`.** Recién ahora sube el `Ratchet`, 1 por cada `Toll`. Si cruzás un umbral, el
marcador para ahí, se ignora el resto, y se resuelve el umbral completo.

El orden importa: **matás primero y pagás después**. Un personaje puede matar a un enemigo
y cruzar un umbral con la misma tirada, y el enemigo muere igual.

## Conversión de caras

Algunas skills convierten una cara en otra — `Insight` cuenta `Wild` como `Hit`.

Regla dura: **una cara convertida deja de contar como la original**, salvo que el efecto
diga explícitamente lo contrario. Y **no se puede convertir dos veces**: dos habilidades no
pueden encadenarse sobre la misma cara.

## Implementación

Los dados se tiran con **física real**: colisión, rebote y lectura de la cara que quedó
arriba. No es una animación sobre un resultado ya calculado.

Eso vale la pena por dos razones. Es la mecánica más presente del juego —se tira en casi
todas las acciones— y merece peso. Y como los dados son físicos, el jugador puede
seleccionarlos individualmente con el mouse para elegir cuál rerollear.

## 📐 Fórmulas

**Aviso previo, obligatorio.** El manual **no publica la distribución de caras del dado**.
Dice que cada dado tiene 4 resultados posibles y que algunas caras traen dos resultados a la
vez, pero nunca dice cuántas de las 6 caras físicas corresponden a cada resultado. Todo lo
que sigue está escrito **paramétrico**, y los ejemplos usan una distribución **provisional,
declarada acá**, que es una perilla de balanceo y no un dato del manual.

Distribución provisional — **NO sale del manual**:

| Dado | `Hit` | `Hit`+`Toll` | `Toll` | `Wild` | `Blank` | p(`Hit`) | p(`Toll`) | p(`Wild`) |
|---|---|---|---|---|---|---|---|---|
| estándar | 2 | 1 | 1 | 1 | 1 | 3/6 = 0.500 | 2/6 ≈ 0.333 | 1/6 ≈ 0.167 |
| bonus | 3 | — | — | 1 | 2 | 3/6 = 0.500 | **0** (esto sí sale del manual) | 1/6 ≈ 0.167 |

La suma de las probabilidades del dado estándar pasa de 1 y no es un error: no son eventos
mutuamente excluyentes, porque existe la cara doble `Hit`+`Toll`. Corolario práctico: `Hit` y
`Toll` **están correlacionados dentro del mismo dado**, así que no se pueden simular como dos
tiradas independientes.

### 1. Tamaño de la tirada

```
N = 3 + B_skill + B_carta + (B_ratchet si la tirada NO es contra vos mismo)
```

| Variable | Qué es | Rango esperado |
|---|---|---|
| `3` | dados estándar, fijos en toda tirada del personaje (manual, pág. 11) | constante |
| `B_skill` | dados bonus de skills activas (`Brawling` nivel 1 da 1, `Marksman` nivel 2 da 2) | 0–3 |
| `B_carta` | dados bonus de cartas y efectos puntuales | 0–2 |
| `B_ratchet` | dados bonus permanentes de umbrales; **4 umbrales los dan** (manual, pág. 8) | 0–4 |

Las tiradas de enemigo **no** se calculan con esta fórmula: salen de la ficha del enemigo,
como `s` dados estándar + `b` dados bonus. El único dato de ficha que aparece en el manual es
un ejemplo suelto: 1 estándar + 2 bonus (pág. 17). **Las fichas concretas de este proyecto no
están definidas todavía y el manual no las da.**

**Ejemplo:** personaje con 2 umbrales-con-dado cruzados y `Brawling` nivel 1, atacando en su
espacio: `N = 3 + 1 + 0 + 2 = 6` dados (3 estándar + 3 bonus). El mismo personaje siendo
atacado por un enemigo de 1+2 tira **3 dados** (los de la ficha), y sus 2 bonus permanentes
**no** se suman.

### 2. `Hit` esperados

```
E[Hit] = n_std × p_H_std + n_bon × p_H_bon
```

- `n_std`, `n_bon` — cantidad de dados estándar y bonus de la tirada. Rango 3–4 y 0–6.
- `p_H_std`, `p_H_bon` — probabilidad de `Hit` por dado. Con la tabla provisional, 0.5 los dos.
- Rango práctico del resultado: **1.5** (tirada pelada) a **~4.0** (personaje cargado).

**Ejemplo:** ataque con 3 estándar + 2 bonus → `E[Hit] = 3(0.5) + 2(0.5) = 2.5` heridas
esperadas. Contra un enemigo de 3 de vida, eso es "casi lo mata de una".

### 3. `Toll` esperados — el número que gobierna el juego

```
E[Toll] = n_std × p_T_std          ← los dados bonus no participan, nunca
```

Rango: **1.0** siempre, en toda tirada de personaje, sin importar cuántos bonus tenga.
Solo cambia si se toca la cantidad de dados estándar (perilla) o la distribución de caras.

**Ejemplo:** un turno de tres acciones de ataque son `3 × 1.0 = 3.0` puntos de `Ratchet`
esperados, aunque el personaje tire 7 dados por acción. Ese desacople es todo el diseño del
trinquete en una línea.

### 4. Eficiencia de la tirada

```
Eficiencia = E[Hit] / E[Toll]        "cuántas heridas compro por cada punto de Ratchet"
```

**Ejemplo:** personaje limpio → `1.5 / 1.0 = 1.5`. Personaje con los 4 dados bonus
permanentes → `3.5 / 1.0 = 3.5`. El poder que se compra con el trinquete no es un +33% de
daño: es **2.33× de eficiencia**. Esta es la métrica que hay que mirar al balancear la curva
de poder, no el daño bruto.

### 5. Distribución del `Toll` en la tirada base

```
P(k Toll) = C(3,k) × p_T^k × (1 − p_T)^(3−k)      con p_T = 1/3
```

| k | Probabilidad | % |
|---|---|---|
| 0 | 8/27 | 29.6% |
| 1 | 12/27 | 44.4% |
| 2 | 6/27 | 22.2% |
| 3 | 1/27 | 3.7% |

Verificación: `0(0.296) + 1(0.444) + 2(0.222) + 3(0.037) = 1.00`, que coincide con la
fórmula 3. Lectura de diseño: **7 de cada 10 tiradas cobran algo** y 1 de cada 4 cobra 2 o
más. No existe la tirada gratis.

### 6. Tiradas esperadas por umbral

```
N_tiradas ≈ d / E[Toll]
```

- `d` — puntos de `Ratchet` que faltan hasta el próximo umbral. **El manual no dice cuántos
  umbrales tiene el track ni a qué distancia están entre sí**; solo dice que 4 de ellos dan
  dado bonus (pág. 8).
- Rango de `d`: sin dato del manual. Provisionalmente 2–4.

**Ejemplo:** con `d = 3` y tiradas base, `3 / 1.0 = 3 tiradas`, o sea **un turno de puro
ataque por umbral**. Ojo con el desperdicio: como el exceso se descarta, si el umbral está a
1 y la tirada saca 3 `Toll`, se pagan 3 y avanza 1. Las tiradas con muchos `Toll` de golpe
son *menos* eficientes como progresión, no más.

### 7. Conversión de caras (`Insight`)

```
Hit_efectivos = H + c × min(W, w_max)
```

| Variable | Qué es | Valores |
|---|---|---|
| `H` | `Hit` realmente tirados | 0…N |
| `W` | `Wild` tirados | 0…N |
| `c` | multiplicador de la conversión (manual, pág. 24) | 1 en niveles 1–3, 2 en nivel 4 |
| `w_max` | cuántos `Wild` se pueden convertir | 1 en nivel 1, ilimitado en nivel 2+ |

En esperanza, con `w_max` ilimitado: `E[Hit_ef] = N × (p_H + c × p_W)`.

**Ejemplo:** 3 dados estándar. Sin `Insight`, `E = 1.5`. Con `Insight` nivel 2,
`3 × (0.5 + 0.167) = 2.0`. Con nivel 4, `3 × (0.5 + 0.333) = 2.5`. O sea que `Insight` nivel 4
vale en esperanza **lo mismo que 2 dados bonus**, y a diferencia de los bonus también rinde en
las tiradas contra vos mismo cuando el efecto lo permite.

### 8. Valor marginal de un reroll

Rerollear el dado `i` mueve la esperanza así:

```
ΔE[Hit]  = p_H − h_i
ΔE[Toll] = p_T − t_i
```

`h_i` y `t_i` valen 1 si la cara actual de ese dado aporta ese resultado y 0 si no. Costo
fijo: 1 `Reserve` (manual, pág. 11).

| Cara que rerolleás (dado estándar) | ΔE[Hit] | ΔE[Toll] | Veredicto |
|---|---|---|---|
| `Toll` solo | +0.50 | −0.67 | el mejor reroll del juego: mejora las dos barras |
| `Blank` | +0.50 | +0.33 | compra daño aceptando riesgo |
| `Wild` sin `Insight` | +0.50 | +0.33 | idéntico a `Blank` |
| `Wild` con `Insight` nivel 2+ | −0.17 | +0.33 | **no rerollear nunca** |
| `Hit` solo | −0.50 | +0.33 | solo tiene sentido en tiradas contra vos mismo |
| `Hit`+`Toll` | −0.50 | −0.67 | depende de qué te esté matando primero |

**Ejemplo resuelto:** tirada de 3 estándar que sale `Hit`, `Toll`, `Blank`. Sin rerolls, el
resultado firme es 1 herida y +1 `Ratchet`. Gastando 2 `Reserve` para rerollear el `Toll` y el
`Blank`, la tirada pasa a `E[Hit] = 1 + 0.5 + 0.5 = 2.0` y `E[Toll] = 0 + 0.33 + 0.33 = 0.67`.
Dos `Reserve` compraron +1.0 de daño esperado y −0.33 de `Ratchet` esperado.

### 9. Procedimiento de resolución (esto no es aritmética, es orden)

Entradas: el multiconjunto de caras después de los rerolls, el contexto de la tirada (ataque,
defensa o efecto) y el objetivo. Salidas: heridas aplicadas, efectos disparados y delta de
`Ratchet`.

```
1. Cerrar rerolls. A partir de acá no se toca nada.
2. Aplicar conversiones de cara. Una sola por cara; la cara convertida deja de ser la original.
3. Contar Hit, Wild, Toll, Blank.
4. Aplicar los Hit  → heridas al objetivo, o el requisito que pida la carta.
5. Resolver muertes y efectos disparados por el resultado.
6. Aplicar los Toll → +1 Ratchet cada uno, de a uno.
7. Si el marcador toca un umbral: PARAR ahí, descartar los Toll restantes y resolver el
   umbral completo (Ratchet Card → +1 nivel de skill → dado bonus si ese umbral lo da).
```

**Ejemplo recorrido.** Personaje con `Insight` nivel 1 y `Ratchet` a 1 punto de un umbral,
atacando a un `Servant` de 2 de vida con 3 estándar + 1 bonus. Sale: `Hit`+`Toll`, `Wild`,
`Blank`, y `Hit` en el bonus.

1. No rerollea.
2. `Insight` nivel 1 convierte el único `Wild` en `Hit`. Ese dado **ya no cuenta como `Wild`**.
3. Conteo: 3 `Hit`, 1 `Toll`, 1 `Blank`.
4. 3 heridas al `Servant`, que tiene 2 de vida → **muere**. La herida sobrante se pierde.
5. Se resuelven los efectos de muerte del `Servant`, si tiene.
6. 1 `Toll` → +1 `Ratchet`.
7. El marcador **toca el umbral**: para ahí, dispara la `Ratchet Card`, sube 1 nivel de skill
   y, si ese umbral trae dado, suma +1 bonus permanente por el resto de la partida.

El enemigo murió **antes** de que el personaje pagara el umbral. Ese es exactamente el punto
del orden.

## ⚠️ Casos borde

| Caso | Qué pasa |
|---|---|
| Cara doble `Hit`+`Toll` | Se aplican los dos resultados del mismo dado: 1 herida **y** +1 `Ratchet`. El `Hit` se resuelve en el paso 4 y el `Toll` en el 6, así que la herida existe incluso si ese `Toll` te hace cruzar un umbral (manual, pág. 12). |
| 3 `Toll` con el umbral a 1 de distancia | El marcador avanza 1, **para en el umbral**, y los 2 `Toll` restantes se **descartan**. No se acarrean al umbral siguiente ni a nada (manual, pág. 8). |
| Una tirada que atravesaría **dos** umbrales | Imposible por construcción: el marcador para en el primero y el exceso muere ahí. Máximo **un umbral por tirada**. |
| Matar al enemigo y cruzar un umbral con la misma tirada | El enemigo muere igual. Los `Hit` van en el paso 4 y el `Toll` en el 6 (manual, pág. 12). |
| Tirada contra vos mismo teniendo dados bonus permanentes | Los permanentes del trinquete **no se suman** (manual, pág. 11). Los bonus de skills y cartas **sí** se aplican, salvo que el efecto diga lo contrario. |
| Enemigo cuya ficha trae dados bonus | Se tiran igual, y esos bonus no tienen cara `Toll`: un enemigo de 1 estándar + 2 bonus puede cobrarte **como máximo 1** punto de `Ratchet` (manual, pág. 17). La regla de "no sumar bonus" es sobre los dados del personaje, no sobre la ficha del enemigo. |
| `Reserve` en 0 | No hay más rerolls pagos. Siguen disponibles los **rerolls gratis** de skill (`Toughness` nivel 1 da 1 al ser atacado o en `Hazard`; `Brawling` nivel 3 da 2 atacando en tu espacio). |
| Rerollear el mismo dado dos veces seguidas | Permitido mientras quede `Reserve`. El manual lo dice explícito en su ejemplo de la pág. 14. |
| Reroll gratis y reroll pago en la misma tirada | Se combinan, y **el orden lo elige el jugador**. El ejemplo del manual (pág. 17) usa primero el gratis de `Toughness` y después uno pago. |
| Rerollear un `Hit` que ya salió | Permitido. Es la jugada normal en tiradas contra vos mismo (rerolleás los `Hit` del enemigo) y cuando el enemigo castiga los `Hit`. |
| Dos habilidades queriendo convertir la misma cara | **Prohibido.** Una cara no se convierte dos veces, y una cara convertida deja de contar como la original salvo que el efecto lo aclare (manual, pág. 12). |
| `Insight` nivel 1 con 3 `Wild` en la tirada | Solo **1** cuenta como `Hit`. Los otros 2 quedan inertes y no se pueden convertir con nada más. |
| `Toughness` nivel 2+ contra una tirada con 2 `Toll` | Se cuentan los 2 `Toll` y después se **reduce la pérdida en 1**: el `Ratchet` sube 1, no 2. La reducción no baja de 0: 1 `Toll` reducido en 1 es 0, nunca −1 (manual, pág. 25). |
| Se acaban los dados físicos | En la mesa se anota el resultado y se vuelven a tirar los mismos dados (manual, pág. 11). En digital **no hay límite** y la implementación no debe cappear la cantidad. Lo que sí hay que decidir es el límite de layout de la mesa de dados en pantalla: **eso no está en el manual**. |
| Tirada de 0 dados (un efecto deja la tirada en cero) | **El manual no lo cubre.** Decisión a tomar: 0 dados produce 0 `Hit` y 0 `Toll` y **no cuenta como tirada**, así que no dispara efectos del tipo "cuando hacés una tirada". |
| `Hazard` con 0 tokens a fin de turno | No se tira nada. No es una tirada de 0 dados: el paso entero se saltea. |
| `Hit` de sobra sobre un enemigo ya muerto, sin `Brawling` nivel 2+ | Se pierden. El objetivo se elige **antes** de tirar y todas las heridas van a ese objetivo aunque sobren (manual, pág. 14). |
| El `Toll` que llega al final del track | El personaje **se pierde y sale del juego**. La regla de exceso protege umbrales, no el final del track: el manual no marca el final como umbral, así que ahí no hay freno. |
| Cara `Wild` en la tirada de un enemigo | No hace nada, salvo que el personaje **o el enemigo** tengan un efecto que la lea (manual, pág. 17). Es el único caso en que una cara del enemigo puede beneficiarte. |
| Un efecto pide "igualar o superar X `Hit`" y sale exactamente X | Cuenta como éxito: el manual dice "match or exceed" (pág. 12). Los `Toll` de esa tirada se cobran igual, haya éxito o no. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.

| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Caras `Toll` por dado estándar | **No está en el manual.** Provisional: 2 de 6, o sea `p_T = 1/3` | 1–3 de 6 (`p_T` 0.167–0.5) | La velocidad de la progresión y la del reloj de muerte, al mismo tiempo y en el mismo sentido. Es la perilla más peligrosa del juego: mover esto reescribe la duración de la partida y desincroniza la ventana de sacrificio con la manifestación del adversario → rompe **04-oposicion/reloj-y-avance.md**. |
| Caras `Hit` por dado estándar | **No está en el manual.** Provisional: 3 de 6 | 2–4 de 6 | Cuántas acciones cuesta matar cada enemigo, o sea la densidad de combate del turno. Subirlo hace que el mapa se limpie solo y el referéndum deje de doler. |
| Caras `Wild` por dado estándar | **No está en el manual.** Provisional: 1 de 6 | 1–2 de 6 | El valor de `Insight`, y nada más. Con 0 `Wild` la skill deja de existir; con 2 pasa a ser la mejor del pool común. |
| Dados estándar por tirada | 3 (manual, pág. 11) | 2–4 | Sube el daño y el `Ratchet` **en la misma proporción**, así que no sirve para ajustar letalidad sin tocar progresión. Para eso están las caras. |
| `Toll` en los dados bonus | Ninguno (manual, pág. 11) | No tocar | Si los bonus cobraran `Toll`, el premio de los umbrales dejaría de ser premio y la estrategia de quemar un personaje a propósito se muere. Es un axioma disfrazado de perilla. |
| Costo de reroll | 1 `Reserve` por dado (manual, pág. 11) | 1–2 | Cuánta varianza puede comprar el jugador por turno. A 2, `Reserve` alcanza para dos arreglos y las tiradas malas se vuelven casi definitivas. |
| Máximo de `Reserve` | 4 (manual, pág. 8) | 3–6 | Cuántos rerolls hay entre dos `Recover`. Interactúa con `Recover`: con máximo 6 y curación de 3 puntos, el techo nunca se alcanza de una sola acción. |
| Umbrales que dan dado bonus | 4 (manual, pág. 8) | 2–6 | La pendiente de la curva de poder. Con 6, un personaje quemado llega a eficiencia 4.5 y trivializa el combate tardío. |
| Bonus permanentes en tiradas contra vos mismo | No aplican (manual, pág. 11) | No tocar | Activarlo invierte el signo del trinquete: subir de nivel te haría más frágil al ser atacado y el jugador dejaría de querer `Ratchet`. Rompe **trinquete.md** entero. |
| Multiplicador de conversión de `Insight` | 1 en niveles 1–3, 2 en nivel 4 (manual, pág. 24) | 1–2 | Cuánto vale la cara inerte. En nivel 4 con multiplicador 2, la skill equivale a 2 dados bonus que además funcionan contra vos mismo. |

## ✅ Criterios de aceptación

1. Una tirada sin ningún modificador usa **exactamente 3 dados estándar**, y la UI no ofrece la opción de tirar menos.
2. Un dado que cae en la cara `Hit`+`Toll` produce **1 herida y +1 `Ratchet`** en la misma tirada, y la herida se aplica antes de mover el marcador.
3. Un personaje con 4 dados bonus permanentes y `Brawling` nivel 1, atacando en su espacio, tira **8 dados**, de los cuales **exactamente 3** pueden producir `Toll`.
4. Ese mismo personaje, atacado por un enemigo de ficha 1 estándar + 2 bonus, tira **exactamente 3 dados**: los 4 bonus permanentes no aparecen en esa tirada.
5. Gastar 1 `Reserve` rerollea exactamente 1 dado; con `Reserve` en 0 el reroll pago queda deshabilitado y los rerolls gratis de skill siguen usables.
6. El mismo dado se puede rerollear 2 veces consecutivas gastando 2 `Reserve`.
7. Un `Servant` de 2 de vida que recibe 3 `Hit` desaparece del tablero **antes** de que el marcador de `Ratchet` se mueva por los `Toll` de esa misma tirada.
8. Con el umbral a 1 punto de distancia y 3 `Toll` en la tirada, el marcador queda **EN** el umbral y los 2 `Toll` restantes se descartan: el track no avanza ni un punto más.
9. Ninguna tirada dispara 2 umbrales: con 3 `Toll` y umbrales a distancia 1 y 2, se resuelve solo el primero.
10. `Insight` nivel 1 sobre una tirada con 3 `Wild` cuenta **1** como `Hit`, y los otros 2 no son convertibles por ningún otro efecto de esa tirada.
11. Una cara convertida de `Wild` a `Hit` **no** satisface un efecto que pida `Wild` en la misma tirada.
12. Simulando 10.000 tiradas de 3 dados estándar con la distribución de caras vigente, la media de `Toll` cae en 1.0 ± 0.05 y la de `Hit` en 1.5 ± 0.05.


==============================================================================
PAGINA 10 -- Turno
fuente: design/gdd/03-resolucion/turno.md
==============================================================================

# 🔄 Secuencia de turno

> Este es el documento que más se va a consultar durante la implementación.

**Un turno por personaje.** Con 4 personajes, una ronda son 4 turnos completos, uno tras
otro. No hay fase simultánea.

Las cuatro fases van **siempre en este orden**, y ninguna se puede saltear:

```
1. Tres acciones
2. Carta de presión
3. Referéndum
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
mapa. → **04-oposicion/objetivo-y-adversario.md**

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

## Fase 3 — Referéndum

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

→ **04-oposicion/reloj-y-avance.md**

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
| `acciones_extra` | +1 por `Swiftness` nivel 4 (manual, pág. 25) | 0–1 |
| `acciones_gratis` | 1 `Move` gratis con `Swiftness` nivel 2, 1 ataque a distancia gratis con `Marksman` nivel 4, más lo que den cartas | 0–3 |

Las **acciones libres** de efectos y cartas no entran en ninguna de las dos cuentas: no
consumen presupuesto y solo se pueden usar en `P1`.

**Ejemplo:** personaje con `Swiftness` nivel 4 y `Marksman` nivel 4.
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

### 5. El referéndum en números

```
n_atacantes = enemigos_al_empezar_P3 + entraron_durante_P3 − salieron_durante_P3
E[Wounds]   = Σ E[Hit] de la tirada de cada atacante
E[Ratchet]  = Σ E[Toll] de la tirada de cada atacante
```

Las esperanzas por tirada salen de **dados.md** y dependen de la distribución de
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
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.

| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Acciones por turno | 3 (manual, pág. 13) | 2–4 | La densidad de decisiones del turno y, de rebote, la velocidad del trinquete: más acciones son más tiradas, y más tiradas son más `Toll`. Subirlo a 4 acelera la progresión sin tocar los dados. |
| Espacios por `Move` | 3 (manual, pág. 13) | 2–5 | Qué tan pegajoso se siente el mapa. Con 5, los enemigos que te siguen dejan de ser un problema y `Stealth` pierde razón de existir. |
| Puntos de `Recover` | 3, repartibles (manual, pág. 14) | 2–4 | El ritmo de recuperación, y por lo tanto la letalidad real. Interactúa con el máximo de `Reserve` (4): con 2 puntos, volver a tope cuesta dos acciones. |
| Umbral de `Doom Symbol` en el descarte | 3 (manual, pág. 18) | 2–5 | La velocidad del reloj. Bajarlo a 2 acorta la partida y **adelanta la ventana de sacrificio** del trinquete, porque el adversario se manifiesta antes → toca **04-oposicion/reloj-y-avance.md** y la estrategia central de **trinquete.md**. |
| Fracción de cartas con `Doom Symbol` | **No está en el manual.** Solo dice que el mazo son 16 cartas | 0.3–0.6 | La misma velocidad de reloj que la perilla anterior, pero por la vía de la varianza: con `f` bajo el reloj es lento y errático, con `f` alto es rápido y predecible. |
| Tamaño del `Pressure Deck` | 16 cartas (manual, pág. 10) | 12–20 | Cuánto se repiten los efectos dentro de una partida y qué tan pronto se agota el mazo. Con 12 hay que resolver ya el caso borde del mazo vacío. |
| Cartas de presión por turno | 1 (manual, pág. 15) | 1–2 | La presión total del juego. A 2 se duplican el reloj y los spawns al mismo tiempo: es la perilla más brusca de todas. |
| Cartas de `Reward` por referéndum | 1 (manual, pág. 16) | 1–2 | Cuánto paga limpiar el espacio. Subirlo hace que valga la pena invertir dos acciones en limpiar, y baja la presión de la rama de pelea. |
| Acciones propias de la misión | 2 (manual, pág. 13) | 2–3 | Cuánto del turno se dedica al objetivo en vez de al combate. Con 3 el escenario compite con las acciones base por el presupuesto. |
| Personajes por ronda | 4 (decisión del GDD, ver **00-vision/alcance.md**; **el manual no fija la cantidad**) | 1–4 | Cuántos turnos hay por avance del reloj. Con 2 personajes, cada avance del adversario cae cada media ronda y la partida se acorta a la mitad en tiempo real sin cambiar ningún número. |

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


==============================================================================
PAGINA 11 -- Vocabulario de efectos
fuente: design/gdd/03-resolucion/vocabulario-de-efectos.md
==============================================================================

# 🧩 Vocabulario de efectos

Las cartas del juego —presión, recompensa y trinquete— **no son efectos arbitrarios**. Son
combinaciones de un vocabulario chico y cerrado. Este documento lo cataloga, porque es lo que
después se convierte en Data Tables y Gameplay Tags, y es lo que hace que la misión 2 sea
contenido en vez de código.

**Está derivado de evidencia, no inventado.** Las piezas de abajo salen de cartas reales del
juego original, recuperadas de la wiki y de fuentes de la comunidad. Ver [Procedencia](#procedencia).

---

## El hallazgo que cambia el diseño

Las cartas de trinquete **no son puro castigo**. Cada una es un **condicional de dos ramas**:

```
si <se cumple una condición de estado>  ->  costo
si no                                   ->  recompensa
```

Cuatro ejemplos reales, con la mecánica traducida a nuestro vocabulario:

| Carta original | Qué lee | Rama de costo | Rama de recompensa |
|---|---|---|---|
| *Claustrophobia* | **Cuántos espacios adyacentes** tiene tu espacio | ≤ 2 adyacentes → +2 `Reserve` | si no → cura 1 `Reserve` **por espacio adyacente** |
| *Codependency* | La posición de **otro personaje** designado al inicio | está vivo y no comparte tu espacio → +2 `Reserve`, y después uno de los dos se mueve al espacio del otro **sin que los enemigos sigan** | comparte tu espacio → nada |
| *Hypochondria* | **Cuántos objetos y `Ally`** tenés, contra un contador acumulado **en la propia carta** | menos objetos que tokens → `Reserve` igual a la diferencia | si no → cura **todo** el `Reserve` |
| *Recurring Trauma* | El **descarte de presión**, con una carta guardada al lado | hay carta guardada → se resuelve como si la acabaras de robar | no hay → cura **todo** el `Reserve` |

**Por qué importa.** Yo tenía descrita la carta de trinquete como "un efecto propio del
personaje". Es mucho más que eso: es un **incentivo posicional y de estado que se dispara 6
veces por partida**, y que el jugador puede jugar a favor.

Consecuencias directas:

- El jugador no solo administra *cuándo* cruza un umbral: administra **en qué estado está** cuando
  lo cruza. Cruzarlo parado en el lugar correcto puede ser un beneficio neto.
- Eso le da al trinquete una capa de decisión que no venía de los dados, y **responde en parte a
  la crítica de que "el trinquete te pasa, no lo elegís"** (**A-05**).
- Y hace que las 8 cartas sean **contenido de diseño de primer orden**, no relleno temático.

## El otro hallazgo: la topología del mapa es input de juego

*Claustrophobia* lee **cuántos espacios son adyacentes al tuyo**. O sea que el **grado de los
nodos del grafo** es un valor que las reglas consultan, no solo una propiedad del layout.

Consecuencia para el mapa: **no puede ser una retícula uniforme.** Si todos los espacios tienen
el mismo grado, ese tipo de carta no distingue nada y el efecto se vuelve constante. El mapa
necesita variedad deliberada de grado: pasillos de grado 1-2, cruces de grado 4-5.

Eso convierte una perilla que había puesto como estimación —el grado promedio del grafo— en un
parámetro con consecuencia mecánica real.
→ **../07-balance/perillas-y-constantes.md**

---

## El vocabulario, catalogado

### Selectores — qué apunta el efecto

| Selector | Visto en |
|---|---|
| El personaje activo | todas |
| Otro personaje, designado al inicio de la partida | *Codependency* |
| Los espacios adyacentes al tuyo, y su cantidad | *Claustrophobia* |
| Tu espacio, y qué hay en él | referéndum, `Recover` |
| Tu inventario, y su cantidad | *Hypochondria* |
| El enemigo más cercano de un tipo, por camino más corto | cartas de presión |
| Todos los enemigos de un tipo | cartas de presión |
| Una `Gate` por color | cartas de presión |
| El descarte de presión | *Recurring Trauma* |
| Tokens acumulados **en la propia carta** | *Hypochondria* |

### Condiciones — cuándo se dispara cada rama

Comparación de cantidad (`≤ 2 adyacentes`, `objetos < tokens`), presencia o ausencia
(`hay carta guardada`), estado de otra figura (`está vivo`), y posición relativa
(`no comparte tu espacio`).

### Acciones — qué hace

| Acción | Nota |
|---|---|
| Sumar o curar `Reserve` | incluido **"curá todo el `Reserve`"**, que es una recompensa grande |
| Sumar o curar `Wounds` | |
| Sumar `Ratchet` | |
| Mover una figura N espacios hacia un objetivo | por camino más corto, para al llegar |
| Mover un personaje a otro espacio | *Codependency* usa la excepción de **"colocar"**: los enemigos **no** siguen |
| Invocar un enemigo en una `Gate` | se saltea si no hay figura disponible |
| Colocar un token sobre una carta | estado persistente **entre activaciones** |
| Resolver otra carta como si se acabara de robar | *Recurring Trauma* |
| Forzar una tirada | con umbral de `Hit` a superar, o efecto por cada `Hit` |
| Subir un nivel de skill | |
| Reclamar o descartar una carta | |
| Avanzar el `Doom Track` | |

### Semántica de ejecución

Dos reglas que valen para **todo** el vocabulario, y que se implementan una sola vez:

1. **Un paso que no se puede ejecutar se saltea, no falla.** La carta sigue resolviéndose.
2. **Estado persistente en la carta.** Una carta puede acumular tokens entre activaciones, así
   que el efecto crece con la partida. *Hypochondria* se vuelve peor cada vez que se dispara.

---

## Qué falta y por qué no se cataloga todavía

El catálogo de arriba sale de **4 cartas de trinquete** de las 8 que necesita una misión, y de la
estructura de 3 partes de las cartas de presión. Falta:

- Las cartas de presión reales: solo se conoce su **estructura**, no un set de efectos concretos
- Las cartas de recompensa: objetos, `Ally` y `Burden`, con dos lados cada una
- Los efectos de revelación y permanentes de los 4 stages del adversario

**No hay que buscarlos.** Ese es el contenido que este proyecto diseña, y el vocabulario de arriba
ya alcanza para empezar. La regla sigue siendo la de
**D-09**: se implementan las primeras 5 o 6 cartas a mano, se mira qué
se repite, y el vocabulario definitivo se extrae de esa evidencia — no de esta lista, que es un
punto de partida.

## Procedencia

- [Insanities — Cthulhu Death May Die Wiki (Fandom)](https://cthulhu-death-may-die.fandom.com/wiki/Insanaties) — el texto de las cartas de trinquete
- [Insanity Card: Recurring Trauma — BoardGameGeek](https://boardgamegeek.com/thread/2321338/insanity-card-recurring-trauma) — la interacción con el descarte de presión
- La estructura de 3 partes de las cartas de presión y la semántica de "se saltea" salen del
  manual transcripto, `docs/rulebook/rulebook.md` pág. 15

Los textos de carta son de **fuentes de terceros**. Se usan como evidencia de la *forma* del
vocabulario, no como contenido a copiar: las cartas de este juego se diseñan de cero.


==============================================================================
PAGINA 12 -- Enemigos
fuente: design/gdd/04-oposicion/enemigos.md
==============================================================================

# ⚔️ Enemigos

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
→ **06-decisiones/abiertas.md**

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
**07-balance**, `p(Hit) = 0,50` —**el valor real no
está en el manual**— eso son 3 heridas esperadas por turno de quedarse sucio, y solo 1
`Toll` esperado.

Consecuencia de diseño: **cargar el ataque enemigo en dados bonus pega fuerte y casi no mueve
el trinquete**; cargarlo en dados estándar te empuja al final del track. Son dos perillas
distintas disfrazadas de una →
**03-resolucion/dados.md**

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
nunca se saltea → **../06-decisiones/abiertas.md**

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
| Un enemigo con permiso de cruzar pasajes bloqueados | Su distancia se mide con **sus** permisos, incluso para decidir si es "el más cercano" (manual p. 20) → **../01-fundamentos/mapa-y-espacios.md** |
| El personaje muere con enemigos encima | Las figuras enemigas **se quedan** en el espacio. Si algún efecto necesita la posición del personaje muerto, se usa el espacio donde murió (manual p. 19) |
| Se saca el límite de pool (A-04) | El spawn **nunca** se saltea y `EnemigosMax` deja de existir. El tope blando de dificultad desaparece y hay que reemplazarlo por otro mecanismo —cooldown de `Gate`, cap por espacio, o presupuesto por turno—; el manual no ofrece ninguno porque no le hace falta |

---

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.

| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| `Pool(Servant)` | 10 (manual p. 20) | 6–16, o sin límite | El techo de presión de la mitad de la partida ligada al `Objective`. Sin límite, los spawns nunca se saltean y desaparece el tope blando de dificultad (A-04) |
| `Pool` por tipo de `Creature` | 5 / 2 / 1 según el tipo (18 figuras en 7 tipos, manual p. 3) | 1–6 | Cada cuánto un spawn se saltea. Los tipos de pool 1 son casi únicos: matarlos se siente como un logro y su ausencia se nota el resto de la partida |
| Tipos de `Creature` por misión | Lo define el episodio; las reglas opcionales del manual agregan 2 tipos más (p. 21) | 2–4 | La variedad de amenazas y, directamente, cuántos stat blocks y cuántos modelos hay que producir → **../05-partida/contenido-de-una-mision.md** |
| `H` de un enemigo | `Servant` = 2, un tipo de `Creature` = 3. **El resto no está en la transcripción** | 1–6 | Cuántas acciones cuesta limpiar un espacio. A 4+, un solo enemigo se come un turno entero y `Recover` deja de ser alcanzable, porque nunca llegás a tener el espacio `Clear` |
| Dados de ataque por enemigo | 1 estándar + 2 bonus (el único bloque visible, manual p. 17) | 1–4 dados totales | El costo por turno de quedarse sucio. **Estándar y bonus no son intercambiables**: los bonus no tienen cara `Toll`, así que subirlos aumenta el daño sin acelerar el trinquete → **../02-personaje/trinquete.md** |
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


==============================================================================
PAGINA 13 -- Reloj y avance
fuente: design/gdd/04-oposicion/reloj-y-avance.md
==============================================================================

# ⏳ El reloj (`Doom Track`)

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
→ **06-decisiones/abiertas.md**

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
→ **objetivo-y-adversario.md**

## El reloj como diseño

Vale entender qué logra este sistema, porque condiciona todas las decisiones del jugador:

**Castiga la pasividad sin usar un timer.** No hay reloj real, pero cada turno que jugás
acerca el final. No podés esperar a estar listo.

**Hace que la velocidad sea un recurso.** Ir rápido y sucio te deja peor preparado para la
pelea final; ir lento y prolijo te deja sin tiempo. No hay una respuesta correcta fija.

**Y le pone un techo a la estrategia del sacrificio.** Quemar un personaje para volverlo
fuerte lleva turnos, y cada turno gastado en eso es reloj que corre.
→ **../02-personaje/trinquete.md**

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
   → **../02-personaje/skills.md**
2. **El sacrificio deliberado se queda sin pista.** La estrategia insignia de esta adaptación
   (quemar un personaje a propósito) solo se habilita después de que el adversario se
   manifiesta. Esa ventana se abre siempre a la misma **fracción** de la partida —ver R6— pero
   con `P = 4` lo que queda después son ~4 turnos propios en vez de ~8. Con 4 turnos no se
   quema a nadie a fondo.
   → **../02-personaje/trinquete.md**

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
  → **../03-resolucion/turno.md**

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
| Se pierden todos los personajes después de la manifestación | La partida termina por la otra condición de derrota (→ **../05-partida/victoria-y-derrota.md**). Es el extremo degenerado del lazo de R6: sin nadie vivo no hay robos y el reloj se congela, pero ya no hay partida. |
| La UI esconde la pila de descarte | El mecanismo entero se pierde: el jugador queda dentro de la banda de R8 sin poder leerla. No es un caso borde de reglas, es un requisito: el contador `D` de `T` va siempre a la vista. |

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.

| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Umbral de `Doom Symbol` (`T`) | **3** (manual) | 3-6, tope duro `T ≤ q` | La velocidad del reloj, lineal. Es la compensación más barata para `P = 4`, y su costo es la **predecibilidad**: cuanto más alto, más del mazo ve el jugador antes de cada remezclado. Arriba de `q` el reloj se apaga. |
| Espacios del `Doom Track` (`S`) | **8** (manual) | 8-15 | El largo de la partida, lineal en `S − 1`. Cada espacio agregado necesita un efecto de avance definido en la carta de misión y en las Stage cards; sin eso, se agregan avances vacíos y el reloj deja de escalar en amenaza. |
| Cartas robadas por turno de personaje | **1** (manual) | 1, o 1 cada 2 turnos | El reloj **y** el spawn de enemigos **y** los efectos de presión, todo junto. Bajarlo frena el reloj y al mismo tiempo corta la fuente de `Ratchet`: es la única perilla de esta tabla que empuja los dos problemas de R3 en la misma dirección equivocada. |
| Personajes controlados (`P`) | 4 (decisión del proyecto; el manual recomienda **2**) | 2-4 | Todo. Es la perilla que genera el problema: la velocidad del reloj es lineal en `P` y los turnos propios de cada personaje van con `1/P`. Cambiarla es cambiar el juego, no balancearlo. |
| Cartas con `Doom Symbol` (`q` de `n`) | **no está en el manual** — provisional 12/16 | 8/16 a 14/16 | El ritmo y su varianza a la vez (R2, R8). Bajar `q` frena el reloj **y** ensancha la banda de impredecibilidad, que es la propiedad que el sistema quiere. Es la perilla más elegante de las cinco y la única que mejora dos cosas a la vez. |
| Tamaño del `Pressure Deck` (`n`) | **16** (manual: 8 + 8) | 12-24 | La repetición de contenido por partida y, de rebote, el ritmo: con `q/n` fijo, subir `n` casi no mueve la esperanza pero **ensancha la varianza** y hace que el jugador vea menos veces la misma carta. |
| Espacio de manifestación (`M`) | **no está en el manual** | 3-5 de 8 | Cuánto dura el candado del sacrificio, como fracción de la partida (R7), y por lo tanto cuándo se puede empezar a jugar la estrategia central de **../02-personaje/trinquete.md**. Muy bajo, el candado casi no existe y perder un personaje deja de dar miedo; muy alto, la mitad de la partida se juega con el jefe encima. |
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


==============================================================================
PAGINA 14 -- Objetivo y adversario
fuente: design/gdd/04-oposicion/objetivo-y-adversario.md
==============================================================================

# 👑 Objetivo y adversario

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
→ **../01-fundamentos/mapa-y-espacios.md**

## Ganar

**Se gana matando el stage final.** No hay otra condición de victoria.
→ **../05-partida/victoria-y-derrota.md**

---

## Nota de alcance

Con **un solo adversario**, este documento describe la identidad completa del juego. No hay
contraste ni variedad de jefe: el que se elija define el tono, el tipo de presión y el
recurso especial de todo el proyecto.

Cuál se elige sigue abierto. → **../06-decisiones/abiertas.md**

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
**07-balance** (`p(Hit)` = 0,50, `p(Toll)` = 0,33,
valores de trabajo, **no** del manual), el ataque pega
`E[Wounds]` = 2·0,50 + 3·0,50 = **2,50 heridas por ataque**.
Cobra además `E[Toll]` = 2·0,33 = **0,67 puntos de `Ratchet`** — solo los 2 dados estándar
pueden sacar `Toll`.

Dos cosas que no hay que olvidar al implementar esto: los dados del adversario **los tira el
jugador** y puede rerollearlos gastando `Reserve`, y los dados bonus permanentes del
personaje **no se suman**, porque es una tirada contra sí mismo.
→ **../03-resolucion/dados.md**

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
→ **reloj-y-avance.md**

### F6 — Procedimiento: los dos flags del adversario

No hay un solo estado "el adversario está activo". Hay **dos booleanos independientes**, y
casi todos los malentendidos de esta parte del juego salen de confundirlos.

| Flag | Qué habilita | Cómo se prende |
|---|---|---|
| `manifested` | La figura está en el mapa: sigue a los personajes, ataca en la fase de referéndum, suma dados | El marcador entra al **primer espacio marcado** del `Doom Track` **o** se rompe el `Objective` |
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
| El efecto de revelación de un stage lastima al personaje activo y lo mata | Se aplica igual: primero se resuelve la revelación completa, después la muerte. Si el adversario ya estaba manifestado, la partida sigue mientras quede alguien vivo → **../05-partida/victoria-y-derrota.md** |
| Cae el stage **final** | **Victoria inmediata**, sin importar el excedente ni qué más se disparó en ese instante. La victoria tiene precedencia sobre cualquier condición de derrota simultánea |
| Heridas ya puestas sobre un stage derrotado | Se van con la carta al costado. **No se reciclan, no se transfieren, no se cuentan de nuevo.** Cada Stage card lleva su contador propio desde 0 |
| El adversario está en el mapa y el personaje se va del espacio | El adversario **lo sigue**, como cualquier enemigo, y el personaje agarra el `Hazard` del espacio que deja. No hay desengage salvo con `Stealth` → **enemigos.md** |
| Dos efectos permanentes de stages distintos se contradicen | **Gana el stage de número más alto**, y si eso no alcanza para desempatar, elige el jugador activo. El manual no resuelve conflictos entre stages —solo trae la regla general de que ante varias formas de resolver algo elige el jugador activo— así que esto es una decisión del GDD. Se elige el número más alto porque los stages se revelan en orden creciente y el más nuevo es el que describe el estado actual del adversario |
| Un efecto de carta le pone heridas al adversario sin ser una acción `Attack`, con el `Objective` intacto | **Caso no cubierto por el manual.** La regla dice que no puede ser atacado **ni dañado**, así que la lectura recta es que el daño se descarta. Hay que decidirlo y escribirlo, porque condiciona el diseño de las 15 cartas de recompensa |

---

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.

| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Vida de cada stage atacable | **No está en el manual.** Único dato: 12 para un Stage 2 | 8 – 20 por stage | El largo de la mitad 2 en turnos. Es la perilla que más pelea con el `Doom Track`: subir vida sin alargar el track hace que la pelea no se pueda terminar a tiempo → **reloj-y-avance.md** |
| Dados que suma cada stage | **No está en el manual.** Único dato: 3 bonus + 2 estándar con tres stages revelados | +0 a +2 dados por stage | Cuánto castiga quedarse en el espacio del adversario. Como los dados se acumulan, cada punto acá se paga tres veces: la presión de la última parte de la pelea sube de forma no lineal |
| Cantidad de stages | **4** (3 con vida) | 3 – 5 | La cantidad de picos dramáticos y de reposicionamientos de la figura. Bajar a 3 mata uno de los cuatro momentos de cámara; subir a 5 exige rebalancear vida por stage para no alargar la pelea |
| El excedente pasa al stage siguiente | **OFF** (no pasa) | ON / OFF | Prenderlo premia el burst y acorta la pelea mucho más de lo que parece. Rompe además el ritmo de "cada revelación es un momento": con overflow, dos stages pueden caer en la misma tirada |
| Los efectos y dados previos quedan en juego | **ON** | ON / OFF | Es **la** regla que invierte la curva de tensión del boss fight. Apagarla convierte la pelea en una barra de vida normal y deja sin sentido la decisión de "aguanto lo que viene o no". No es una perilla de tuning: es la identidad del sistema |
| El adversario es invulnerable antes de romper el `Objective` | **ON** | ON / OFF | Es la bisagra entre las dos mitades. Apagarla borra la mitad 1: el jugador puede ignorar el `Objective` y pegarle desde el turno 1. Rompe también las 2 acciones propias de la misión, que existen para servir al objetivo |
| Posición del primer espacio marcado del `Doom Track` | **No está numerada en el manual** (dice solo "primer espacio marcado") | Espacio 3 a 5 de 8 | Cuánto dura el peor escenario: jefe en el mapa y todavía intocable. Más temprano = más turnos comiendo golpes sin poder responder |
| Acciones propias de la misión | **2** | 2 – 3 | Cuánto ayuda el escenario a romper el objetivo. Subirla acelera la mitad 1 y adelanta la manifestación, lo que a su vez adelanta el acceso al sacrificio → **../02-personaje/trinquete.md** |

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


==============================================================================
PAGINA 15 -- Victoria y derrota
fuente: design/gdd/05-partida/victoria-y-derrota.md
==============================================================================

# 🏁 Victoria y derrota

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

Ahí se abre la puerta al **sacrificio deliberado**: empujar a un
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
→ **../04-oposicion/reloj-y-avance.md**

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
El orden de resolución de la tirada es el de **../03-resolucion/dados.md**:
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
| `manifested` | Si el adversario ya está en el mapa → **../04-oposicion/objetivo-y-adversario.md** | true / false |

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
→ **../02-personaje/trinquete.md**

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
→ **../04-oposicion/objetivo-y-adversario.md**

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
→ **../04-oposicion/reloj-y-avance.md**

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
> **07-balance/perillas-y-constantes.md**. Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución verificada** declarada
> ahí mismo: dado estándar `p(Hit) = 0,50` y `p(Toll) = 0,33`; dado bonus
> `p(Hit) = 0,50`, `p(Wild) = 0,33` y **sin cara `Toll`**.

| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Cantidad de personajes (`N`) | **4** en este proyecto; el manual recomienda **2** para un jugador | 2 – 4 | Todo a la vez: cuántas cartas de presión se roban por ronda, cuánta build se construye, y cuán frágil es la mitad 1 (con `N` = 4 hay cuatro maneras distintas de perder de golpe). Es la perilla que obliga a re-tunear el reloj → **../04-oposicion/reloj-y-avance.md** |
| Muertes tolerables antes de la manifestación | **0** | 0 – 1 | Si toda la mitad 1 se juega a cero errores o admite una pérdida. Subirla a 1 habilita el sacrificio temprano y **borra el arco de dificultad**: las dos fases de riesgo dejan de ser dos juegos distintos |
| Espacios del `Doom Track` | **8** | 8 – 12 | El techo duro de largo de partida. Alargarlo es la salida obvia al problema de 4 personajes, y arrastra la vida de los stages del adversario: más track = más turnos = la pelea final puede pedir más vida para seguir siendo tensa |
| `Doom Symbol` necesarios para avanzar | **3** | 3 – 5 | La velocidad del reloj y su varianza. Subirlo alarga la partida sin tocar el track, pero también **aplana la impredecibilidad**, que es el punto del sistema |
| Cartas con `Doom Symbol` en el mazo (`k` de 16) | **No está en el manual** ("la mayoría") | 10 – 14 de 16 | La varianza del reloj más que su promedio. Bajarlo mucho hace que haya rondas enteras sin presión y el jugador puede planificar; subirlo a 16 lo vuelve un contador fijo cada 3 turnos |
| Cartas de presión por turno | **1** | 1 | Es la unidad de medida de todo el reloj. Cambiarla invalida cualquier tuneo previo de las dos perillas anteriores y rompe el autobalanceo de F5 |
| Precedencia de la victoria en empates | **ON** (gana la victoria) | ON / OFF | Casi no mueve la dificultad —los empates exactos son rarísimos— pero define la fantasía entera del final: "salvaste el mundo y morís" contra "morís y no sirvió de nada". Es una decisión de tono, no de balance |
| Largo del track de `Ratchet` | **No está en el manual** | ≥ 4 umbrales + espacio final | Cuántas veces cruza umbrales cada personaje, o sea cuánta build se construye. **Tocarlo rompe el trinquete**: cambia la cantidad de dados bonus permanentes y de niveles de skill de toda la partida → **../02-personaje/trinquete.md** |
| Largo del track de `Wounds` | **No está en el manual** | **5–8** (**07-balance**) | Cuántos golpes aguanta un personaje antes de terminar la partida en la mitad 1. Es la perilla más directa sobre la fragilidad de la primera fase, y la más fácil de romper: bajarla un punto puede volver la mitad 1 injugable con `N` = 4 |

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


==============================================================================
PAGINA 16 -- Contenido de una mision
fuente: design/gdd/05-partida/contenido-de-una-mision.md
==============================================================================

# 📦 Contenido de una misión

Inventario de lo que hay que producir para tener **una partida completa**. Sirve como
checklist de producción y como estimación de esfuerzo.

## Contado

| Elemento | Cantidad | Origen |
|---|---|---|
| Cartas de presión (`Pressure Deck`) | **16** — 8 de la misión + 8 del adversario | Diseño propio |
| Cartas de recompensa (`Reward Deck`) | **15** | Diseño propio |
| Cartas de trinquete (`Ratchet Card`) | **8**, se reparte 1 por personaje | Diseño propio |
| Stages del `Adversary` | **4** | Diseño propio |
| Acciones propias de la misión | **2** | Diseño propio |
| `Gate` en el mapa | **3** | Layout |
| Pool máximo de `Servant` | **10** | Regla |
| Personajes jugables | **4** | Diseño propio |
| Skills comunes | **6**, de 4 niveles cada uno = **24 efectos** | **Ya especificado** |
| Espacios del `Doom Track` | **8** | Regla |

## Lo que NO hay que diseñar

**Las 24 celdas de skills ya están escritas.** Están en `docs/rulebook/rulebook.md`, páginas
24 y 25, transcriptas del manual original. Es el sistema más profundo del juego y viene
resuelto.

Toda la estructura de reglas —secuencia de turno, resolución de dados, distancias,
condiciones de fin— también está transcripta y verificada. Lo que se diseña es **contenido**,
no sistemas.

## Lo que sí hay que diseñar de cero

Ordenado por cuánto define el juego:

### El objetivo de la misión

Qué hay que hacer para romperlo, y cómo se hace. Es lo que da forma a la primera mitad de
toda partida. Junto con las **2 acciones propias**, que existen para servirlo.

Es la pieza más importante y la que menos ayuda tiene: el resto del contenido se puede
derivar del manual, esto no.

### El adversario

Sus 4 stages, con vida, efecto de revelación, efecto permanente y dados. Más su recurso
especial y sus minions.

Con un solo adversario, **esto es la identidad del juego**.
→ **../04-oposicion/objetivo-y-adversario.md**

### Las 16 cartas de presión

Tienen **estructura fija de tres partes**, así que se pueden
componer a partir de un vocabulario chico de efectos reutilizables en vez de escribirse una
por una a mano.

**Esa es la clave de la escalabilidad**: si el vocabulario existe, la carta 17 y la 200
cuestan lo mismo que la 5. Si no, cada carta es código a medida.

El vocabulario **no se diseña de entrada**: se implementan las primeras 5 o 6 cartas a mano,
se mira qué se repite, y se extrae de ahí. Así se paga la abstracción con evidencia en vez
de con adivinanza.

### Las 15 cartas de recompensa

Objetos, `Ally` y `Burden`. Cada una con dos lados, de los que el personaje reclama uno.

### Las 8 cartas de trinquete

Una por personaje, del mazo. Se dispara **cada vez** que ese personaje cruza un umbral, así
que un efecto que parece chico se vuelve grande en un personaje muy avanzado. Diseñarlas
pensando en que se van a disparar 4 a 6 veces.

### Los 4 personajes

Cada uno con su **skill de firma** única, más 2 del pool común. La skill de firma es lo que
los diferencia.

### El mapa

Un layout con sección de superficie y sección subterránea, armado por salas de 1 a 3
espacios, con 3 `Gate` y las conexiones especiales que haga falta.

### Los tipos de enemigo

Vida, dados de ataque, habilidad especial y tamaño de pool para cada uno.

## Nota sobre el orden

El vocabulario neutro de este GDD alcanza para diseñar **todo** lo de arriba sin haber
decidido la temática. Los nombres, el arte y el texto de sabor se aplican encima al final,
a partir del glosario de la variante elegida.

Diseñar el contenido antes de elegir temática no es un problema — es el orden correcto.


==============================================================================
PAGINA 17 -- Perillas y constantes
fuente: design/gdd/07-balance/perillas-y-constantes.md
==============================================================================

# 🎛️ Perillas y constantes

**Este archivo es el dueño único de todos los números del juego.** Si un documento de sistema
dice un default o un rango distinto al de acá, **gana este** y hay que corregir el otro.

Existe porque el problema apareció de verdad: los mismos valores se habían escrito en varios
archivos con rangos que no coincidían. La regla del README —"los números viven en un solo
lugar"— necesitaba un lugar.

---

## Constantes del manual

Salen de la transcripción y **no son perillas**: cambiarlas es cambiar el juego, no
balancearlo. El default es el valor original.

| Constante | Valor | Fuente |
|---|---|---|
| Acciones por turno | 3 | pág. 13 |
| Espacios por `Move` | 3 | pág. 13 |
| Dados estándar por tirada | 3 | pág. 11 |
| Puntos de `Recover` (repartidos entre `Wounds` y `Reserve`) | 3 | pág. 14 |
| Máximo de `Reserve` | 4 | pág. 8 |
| `Doom Symbol` necesarios para avanzar | 3 | pág. 18 |
| Espacios del `Doom Track` | 8 | pág. 20 |
| Umbrales que dan dado bonus permanente | 4 | pág. 8 |
| Stages del `Adversary` | 4 | pág. 19 |
| Skills comunes × niveles | 6 × 4 | pág. 22, 24-25 |
| Cartas de presión | 16 (8 + 8) | pág. 10 |
| Cartas de recompensa | 15 | pág. 6 |
| Cartas de trinquete | 8 | pág. 10 |
| `Gate` por mapa | 3 | pág. 9 |
| Pool máximo de `Servant` | 10 | pág. 20 |
| Espacios por sala | 1 a 3 | pág. 9 |
| Espacios del track de `Ratchet` | **20** | componente, ver abajo |
| Umbrales del track de `Ratchet` | **6**, en las casillas 4, 8, 12, 15, 18 y 19 | componente, ver abajo |
| Capacidad de `Wounds` | **4** — la 5.a es letal | componente, ver abajo |
| Capacidad de `Reserve` | **4** | pág. 8 + componente |

---

## Perillas de balanceo

Un solo default y un solo rango seguro por perilla. La columna **Dueño** dice qué documento
explica el efecto en detalle.

| Perilla | Default | Rango seguro | Qué afecta | Dueño |
|---|---|---|---|---|
| Acciones por turno | 3 | **2–4** | A 2 el juego se vuelve asfixiante y `Swiftness` domina; a 4 se puede atacar y escapar en el mismo turno | **turno** |
| Espacios por `Move` | 3 | **2–4** | A 4 el kiting se vuelve viable y los enemigos pegajosos dejan de ser una amenaza | **mapa** |
| Puntos de `Recover` | 3 | **2–4** | Cuánto castiga entrar a un espacio con enemigos | **barras** |
| Máximo de `Reserve` | 4 | **3–6** | Cuánta varianza puede corregir el jugador por turno | **barras** |
| `Doom Symbol` para avanzar (`T`) | 3 | **3–6** | La velocidad del reloj. **Es la perilla de compensación por jugar con 4 personajes**, y el valor propuesto para eso es 6 | **reloj** |
| Espacios del `Doom Track` | 8 | **8–15** | El largo total de la partida. Alternativa a subir `T` | **reloj** |
| Cartas con `Doom Symbol` (`q` de 16) | sin dato | **8–14 de 16** | La varianza del ritmo del reloj: menos cartas con símbolo = avances más impredecibles | **reloj** |
| Umbrales que dan dado bonus | 4 | **2–5** | Cuánto poder compra el trinquete | **trinquete** |
| Capacidad de `Wounds` | **4** | **3–6** | Cuántos golpes aguanta un personaje. Es la perilla más directa sobre la fragilidad de la mitad 1 | **barras** |
| Espacios del track de `Ratchet` | **20** | **16–24** | Cuánto `Toll` aguanta un personaje antes de perderse. Moverlo sin re-espaciar los umbrales cambia la progresión de rebote | **trinquete** |
| Umbrales del track | **6** (4, 8, 12, 15, 18, 19) | **5–7**, y el **espaciado importa más que la cantidad** | Las decisiones de build por personaje. El espaciado original acelera: 4-4-4-3-3-1 | **trinquete** |
| Pool de `Servant` | 10 | **6–14** | El techo de presión simultánea en el mapa | **enemigos** |
| Espacios totales del mapa | sin dato | **12–32** | Cuánto cuesta cruzar el mapa, y por lo tanto el valor de `Swiftness` | **mapa** |

> **`T` y los espacios del `Doom Track` no se mueven juntas.** Las dos alargan la partida por
> el mismo lado; tocar las dos a la vez duplica el efecto sin que se note de dónde vino.

---

## La distribución de caras del dado

**Verificada.** El manual **no la publica** —ni el de Season 3 ni el core original, comprobado
extrayendo el texto de los dos: listan los cuatro resultados y avisan que algunas caras traen
dos, pero nunca dan el reparto, porque está impreso en los dados y no en el reglamento.

Se recuperó de fuentes de la comunidad, con dos coincidencias independientes, y **cuadra con las
dos pistas que sí están en el manual**: que hay una cara con dos resultados, y el ejemplo de la
pág. 14 donde una tirada saca *elder sign, blank, success + tentacle, tentacle*.

### Dado estándar

| Cara | Cantidad |
|---|---|
| `Hit` | 2 |
| `Hit` + `Toll` | 1 |
| `Toll` | 1 |
| `Wild` | 1 |
| `Blank` | 1 |

    p(Hit)   = 3/6 = 0,50
    p(Toll)  = 2/6 = 0,33
    p(Wild)  = 1/6 = 0,17
    p(Blank) = 1/6 = 0,17

### Dado bonus

| Cara | Cantidad |
|---|---|
| `Hit` | 2 |
| `Wild` + `Hit` | 1 |
| `Wild` | 1 |
| `Blank` | 2 |

    p(Hit)   = 3/6 = 0,50
    p(Wild)  = 2/6 = 0,33
    p(Blank) = 2/6 = 0,33
    p(Toll)  = 0      <- esto sí lo confirma el manual, pág. 11

**El dato que esto revela:** los dos dados tienen **la misma probabilidad de `Hit`**. El dado
bonus no pega más fuerte — pega **igual, sin cobrar `Toll`**, y con el doble de chances de
`Wild`. O sea que el premio del trinquete no es potencia: es **quitarle el costo a la tirada**, y
además le sube el valor a la skill `Insight`, que es la que lee `Wild`.

Eso reordena una prioridad de balanceo: `Insight` escala con la cantidad de dados bonus que
tengas, así que un personaje quemado con `Insight` alto es una combinación bastante más fuerte de
lo que las dos piezas sugieren por separado. Hay que vigilarla.

### El espaciado de los umbrales

Los 6 umbrales están en las casillas **4, 8, 12, 15, 18 y 19** de 20. Los saltos son
**4, 4, 4, 3, 3, 1**.

**El espaciado acelera.** La primera mitad del track cobra 4 puntos por umbral y la última cobra
1. Consecuencias:

- El 5.o y el 6.o umbral llegan **casi juntos**, y el 6.o está a **una sola casilla** de la muerte.
- El último tramo entrega dos niveles de skill casi de golpe y después mata.
- La estrategia de quemar un personaje tiene un **acelerón final**: llegado al 5.o umbral, el 6.o
  sale con el primer `Toll` que aparezca. Pero el paso siguiente es perderlo.

Nada de esto estaba en el GDD antes de verificar el componente, y cambia cómo se lee el
sacrificio deliberado: no es una pendiente pareja, es una rampa que se empina al final.

## Valores que el manual no contiene

El manual es un **reglamento, no una especificación de componentes**: los largos de track están
impresos en los tableros de personaje, las caras en los dados, y las vidas y dados de ataque en
las fichas de enemigo. Comprobado extrayendo el texto del reglamento de Season 3 y del core
original: ninguno de los dos los trae.

**Resueltos** con fuentes de la comunidad, dos coincidencias independientes cada uno:

| Qué faltaba | Valor | Estado |
|---|---|---|
| Distribución de caras de los dos dados | ver arriba | verificado y coherente con el manual |
| Espacios del track de `Ratchet` | 20 | verificado |
| Umbrales y sus posiciones | 6, en 4/8/12/15/18/19 | verificado |
| Capacidad de `Wounds` | 4, la 5.a es letal | verificado |
| Capacidad de `Reserve` | 4 | verificado, coincide con el manual |

**Todavía abiertos:**

| Qué falta | Dónde estaría | Impacto |
|---|---|---|
| Cuáles 4 de los 6 umbrales dan dado bonus | Tablero de personaje | Medio. Cambia cuándo llega el poder, no cuánto |
| Vida y dados de ataque por tipo de enemigo | Fichas de enemigo | Medio. Del manual solo se ven ejemplos sueltos: un `Servant` con 2 de vida, una `Creature` con 3, y una ficha de 1 estándar + 2 bonus |
| Cuántas de las 16 cartas de presión traen `Doom Symbol` | Las cartas | Medio. Define la varianza del reloj |
| Vida y dados por stage del `Adversary` | Stage cards | Bajo, es contenido propio de todos modos |
| Capacidad de heridas de cada `Ally` y precio de cada recompensa | Las cartas | Bajo, es contenido propio |

Los que quedan son de impacto medio o bajo, y los tres primeros son contenido que este proyecto
diseña igual. **Ya no hay nada que bloquee un cálculo.**

## Procedencia

Los valores de componente no salen del reglamento. Se verificaron contra:

- [Into the De(ath)pths, Part II — There Will Be Games](https://www.therewillbe.games/articles-analysis/9443-into-the-de-ath-pths-a-deep-dive-into-cthulhu-death-may-die-part-ii-basics-of-investigation) — caras de los dos dados, capacidad de `Wounds` y `Reserve`
- [Dice Probabilities & Arcane Mastery — BoardGameGeek](https://boardgamegeek.com/thread/3099040/dice-probabilities-and-arcane-mastery) — probabilidades de los dos dados
- [Dized Rules — Sanity](https://rules.dized.com/game/kp6yXekcT6agDFGangIB1Q/XXNB96tBSqWBcLGQXdDfYA/sanity) — mecánica del track y los 4 dados bonus
- [Review — Co-op Board Games](https://coopboardgames.com/cooperative-board-game-reviews/cthulhu-death-may-die-review/) — track de 20 espacios y posiciones de los umbrales
- [Reglamento del core original (PDF)](https://cdn.1j1ju.com/medias/e1/c4/90-cthulhu-death-may-die-rulebook.pdf) — usado para **confirmar la ausencia**: 20 páginas extraídas, ningún largo de track ni cara de dado

**Son fuentes de terceros, no oficiales.** Se marcan como verificadas porque hay dos
coincidencias independientes por dato y porque cuadran con las pistas del manual. Si algún día se
puede leer el componente directo, conviene revalidar.


==============================================================================
PAGINA 18 -- Registro de decisiones
fuente: design/gdd/06-decisiones/registro.md
==============================================================================

# 📌 Registro de decisiones

Decisiones ya tomadas, con su razón. **El "por qué" es lo que se pierde primero**, y sin él
la decisión se vuelve a discutir cada tres meses.

Formato: qué se decidió, por qué, y qué queda descartado.

---

## D-01 — Base mecánica: *Cthulhu: Death May Die* Season 3

**2026-08-18**

Las mecánicas del juego de mesa se reimplementan cambiando la temática.

**Por qué:** es un sistema probado, con las reglas ya escritas y balanceadas. El trabajo de
diseño se concentra en contenido y en adaptación al medio, no en inventar sistemas desde
cero.

El manual está transcripto 1:1 en `docs/rulebook/rulebook.md` — 28 páginas, verificado con
scripts, incluyendo contenido que la capa de texto del PDF no tenía.

---

## D-02 — La temática queda sin declarar, y el core es neutro

**2026-08-19**

El vocabulario del core son IDs funcionales. La temática es data.

**Por qué:** hay dos variantes completas y ninguna elegida. Si el core usara los nombres de
una, cambiar de temática sería una refactorización en vez de una decisión de arte.

**Está probado, no supuesto:** los dos glosarios demuestran que el mismo documento mecánico
soporta vocabularios totalmente distintos sin cambiar una regla, verificado línea por línea.

→ **../01-fundamentos/vocabulario.md**

---

## D-03 — Single player

**2026-08-19**

No hay cooperativo ni multijugador.

**Descarta:** la capa social entera del juego original — la negociación entre jugadores, el
`Trade` como conversación, el orden de turno en sentido del reloj. Todo eso pasa de social
a gestión.

---

## D-04 — El jugador controla los 4 personajes

**2026-08-19**

Se mantiene la cantidad de personajes del original, pero los maneja una sola persona.

**Consecuencias, todas reales:**

- El reloj corre **4× por ronda**. El original recomienda 2 personajes en solitario, así que
  esto duplica la velocidad de la configuración prevista. Hay que re-balancear.
- **Se desbloquea el sacrificio deliberado**, una estrategia que el juego de mesa suprime
  socialmente. La adaptación gana algo en vez de perder.
- Empuja fuerte hacia turnos: 4 personajes en tiempo real exigirían pausa táctica o IA de
  compañeros.

→ **../02-personaje/trinquete.md**

---

## D-05 — Alcance: 1 misión y 1 adversario

**2026-08-19**

**Por qué:** es la unidad atómica del juego original — una caja de escenario más una de
jefe. Es una partida completa, no un recorte.

**Descarta:** la rejugabilidad por combinación (12 configuraciones en el original). La
rejugabilidad pasa a depender del trinquete y de las decisiones de build.

→ **../00-vision/alcance.md**

---

## D-06 — El mapa es un grafo, no una grilla

**2026-08-19**

**Por qué:** el manual define adyacencia por pasajes, no por posición. Las escaleras y
túneles vuelven adyacentes espacios lejanos, y todas las reglas de distancia son camino más
corto con aristas bloqueables. Una grilla obligaría a inventar reglas para cada uno de esos
casos; un grafo los resuelve nativamente.

**Beneficio lateral:** sirve igual para turnos que para zonas en tiempo real, así que no
compromete la forma final.

→ **../01-fundamentos/mapa-y-espacios.md**

---

## D-07 — Blueprint-only, sin módulo C++ · **SUPERADA por [D-13](#d-13--arquitectura-híbrida-c--blueprint)**

**2026-08-19**, revertida el **2026-08-20**

**Por qué:** lo impone el temario de la materia. C++ es la última clase, después del segundo
parcial, así que no puede ser un requisito de las entregas.

**Revierte** una inclinación previa hacia un core de reglas en C++ testeable. La restricción
externa gana.

> **Por qué se cayó, y qué se aprende de esto.** La premisa estaba mal leída: el temario ordena
> *cuándo se enseña* cada tema, no qué está permitido usar. De ahí salió una restricción que no
> existía, y esa restricción descartó explícitamente "un core de reglas en C++ testeable" — que
> era la opción correcta. Queda como recordatorio de que conviene chequear si una restricción
> externa es real antes de dejar que descarte un diseño mejor.

→ **../../course-alignment.md** · [D-13](#d-13--arquitectura-híbrida-c--blueprint)

---

## D-08 — Por turnos, con presentación 3D completa

**2026-08-19**

Personajes que caminan y se animan en un espacio 3D, con decisiones por turno. No es una
ficha que teletransporta de casilla a casilla, y no es tiempo real.

**Por qué:** el temario pide state machines de animación, blendspace, posesión, NavMesh,
Behaviour Trees, percepción y EQS — todos piden agentes moviéndose en 3D. Ninguno pide
combate en tiempo real. Esta forma cubre el 100% del temario con el mínimo riesgo, y es
también la más barata en riesgo de feel.

---

## D-09 — La capa de datos se extrae después, no se diseña de entrada

**2026-08-19**

Fase 1 con las reglas en Blueprint. Fase 2 extrae a Data Tables, Data Assets y Gameplay
Tags.

**Por qué dos motivos que coinciden:** el temario da Data Assets y Gameplay Tags recién en
la clase 12, después del primer parcial. Y diseñar el vocabulario de efectos antes de saber
qué efectos necesita el juego es la forma clásica de construir la abstracción equivocada.

**La regla:** se implementan las primeras 5 o 6 cartas a mano, se mira qué se repite, y el
vocabulario se extrae de esa evidencia.

---

## D-10 — Chaos Destruction y Landscape tienen destino mecánico

**2026-08-19**

Son obligatorios porque la materia evalúa la demostración de los temas, así que se les buscó
un lugar que no fuera decorativo.

- **Chaos Destruction** → paredes destructibles que **agregan aristas al grafo** y cambian
  todas las distancias del mapa.
- **Landscape** → la **sección de superficie** del mapa, antes de descender. Le da además
  trabajo real a Lights.

---

## D-11 — El GDD vive partido en archivos, y ClickUp lo espeja

**2026-08-19**

Esta carpeta es la fuente de verdad. El doc de ClickUp es la copia legible para el equipo, y
tiene que estar 1:1.

**Por qué:** un solo archivo monolítico no se puede editar de a varios ni linkear con
precisión. Y la copia en ClickUp existe porque el equipo no lee el repo.

**Regla:** si cambia uno, cambia el otro en el mismo turno.

---

## D-12 - Los valores de componente se toman de fuentes de la comunidad

**2026-08-19**

El reglamento —de Season 3 y del core original, comprobado extrayendo el texto de los dos—
**no publica los valores impresos en los componentes**: las caras de los dados, el largo del
track de `Ratchet`, la cantidad y posicion de los umbrales, ni la capacidad de `Wounds`.

Se recuperaron de fuentes de terceros, exigiendo **dos coincidencias independientes por dato**
y que cuadraran con las pistas que si estan en el manual. Quedaron registrados con su
procedencia en **../07-balance/perillas-y-constantes.md**.

**Por que:** sin la distribucion de caras no habia forma honesta de calcular una sola
probabilidad, y todas las formulas del GDD quedaban colgadas de una suposicion.

**Lo que se descubrio y no estaba en el diseno:**

- El dado bonus tiene **la misma** probabilidad de `Hit` que el estandar. El premio del
  trinquete no es potencia: es **sacarle el costo a la tirada**.
- El espaciado de los umbrales **acelera** (4-4-4-3-3-1). El ultimo esta a una casilla de la
  muerte.
- Una tirada base cuesta **exactamente 1 punto de `Ratchet`** en promedio. Es la escala de
  todo el juego.

**Riesgo asumido:** no son fuentes oficiales. Si algun dia se puede leer el componente
directo, hay que revalidar.

---

---

## D-13 — Arquitectura híbrida C++ + Blueprint

**2026-08-20**

Cae la restricción **Blueprint-only**. El proyecto pasa a tener un módulo de C++: el
comportamiento y los sistemas van en C++, el contenido y las perillas en Blueprint, con el patrón
**base en C++ / hijo en Blueprint** para todo lo que el contenido toca.

**Por qué:** el usuario pidió explícitamente que la implementación sea la más profesional
posible, sin importar el lenguaje. Y Blueprint-only tenía tres techos concretos, no estéticos:

- **No se pueden crear Subsystems.** Verificado en el engine instalado: las cinco clases base
  son `UCLASS(Abstract)` sin `Blueprintable`. Un proyecto Blueprint-only no tiene servicios de
  primera clase, solo puede hospedarlos en el framework.
- **No se puede testear un grafo.** El GDD ya tiene fórmulas y criterios de aceptación escritos
  —la distribución del dado, el costo esperado de una tirada, los 6 umbrales, el BFS con aristas
  bloqueadas— y ninguno era verificable.
- **Los `.uasset` no mergean.** Era el riesgo número uno registrado para el trabajo grupal.
  Mover el comportamiento a `.cpp` no lo mitiga: lo elimina para todo lo que se mueve, y de paso
  habilita code review.

Y el momento es el mínimo costo posible: `Content/` está vacío, así que no hay ni un Blueprint
que reparentar.

**Descarta:** "todo en C++", que en Unreal es un anti-patrón conocido —deja al contenido afuera y
obliga a recompilar para cada ajuste de balance. También descarta **GAS**: está construido
alrededor de predicción de cliente y tiempo real, y este juego es por turnos, determinista y
single player. Y descarta StateTree y MassEntity por ahora.

→ **../../architecture/06-limite-cpp-blueprint.md**

---

## Plantilla para agregar

```markdown
## D-NN — Título corto

**AAAA-MM-DD**

Qué se decidió, en una o dos frases.

**Por qué:** la razón real, no la racionalización.

**Descarta:** qué queda afuera por esta decisión.
```


==============================================================================
PAGINA 19 -- Decisiones abiertas
fuente: design/gdd/06-decisiones/abiertas.md
==============================================================================

# ❓ Decisiones abiertas

Lo que falta decidir, con qué está en juego en cada una. Ordenadas por cuánto arrastran al
resto del proyecto.

Cuando una se cierra, se mueve a **registro.md** con su razón.

---

## A-01 — La temática

**Arrastra:** arte, audio, narrativa, nombres, marketing. Todo lo que ve el jugador.

Hay dos variantes completas, verificadas y con su manual entero aplicado:

| Variante | Glosario | Términos |
|---|---|---|
| A | `docs/rulebook/glossary-biopunk.md` | 66 |
| B | `docs/rulebook/glossary-dnd.md` | 67 |

**No bloquea nada por ahora.** El core es neutro, así que se puede diseñar todo el contenido
y llegar a jugable sin decidir esto. Pero bloquea la producción de arte, así que no puede
quedar abierta indefinidamente.

**Lo que hay que saber al decidir:** el parecido con el original no vive en el léxico, vive
en la silueta mecánica — un jefe único que despierta por etapas, un reloj de fatalidad,
puntos de spawn, y un recurso de una sola dirección que da poder y mata. Ninguna temática
esconde eso.

---

## A-02 — Cuál adversario

**Arrastra:** el tono del juego, el tipo de presión, el recurso especial, y qué minions
existen.

Con **uno solo**, **el adversario es la identidad del juego**. No hay
contraste que lo suavice.

Los dos del original son deliberadamente distintos de textura: uno es un depredador físico
con un recurso de fatiga y fuego, el otro es una anomalía con un recurso más raro. El primero
es más legible y enseña mejor las mecánicas.

**Está atada a A-01**: la variante temática elegida condiciona qué adversario tiene sentido.

---

## A-03 — El re-balanceo del reloj para 4 personajes

**Arrastra:** el largo de la partida, cuántos umbrales de `Ratchet` alcanza a cruzar cada
personaje, y por lo tanto cuánta build se llega a construir.

El original recomienda **2** personajes en solitario. Con 4, el reloj corre al **doble**.

Opciones: alargar el `Doom Track`, subir el umbral de `Doom Symbol` necesarios, o robar carta
de presión con menos frecuencia que una por turno de personaje.

**Cuidado con romper un balanceo automático que ya existe:** menos personajes vivos = menos
cartas por ronda = reloj más lento. Perder gente ya te da más tiempo, y eso es bueno.

→ **../04-oposicion/reloj-y-avance.md**

**Es la más urgente de las tres primeras**, porque no se puede playtestear nada sin resolverla.

---

## A-04 — Qué reemplaza los límites de componentes

**Arrastra:** la dificultad general.

En el juego físico los componentes son finitos, y eso funciona como **tope blando de
dificultad**: si no quedan figuras, el spawn se saltea.

En digital ese límite deja de ser físico. Se puede replicar tal cual, subir, o sacar.
**Sacarlo endurece el juego** y obliga a re-balancear todo lo demás.

→ **../04-oposicion/enemigos.md**

---

## A-05 — ¿El trinquete se puede gastar?

**Arrastra:** cuánta agencia tiene el jugador sobre la mecánica central.

**El problema:** hoy el `Ratchet` no se elige, te pasa por resultados de dados. En la mesa
eso es drama; en pantalla puede leerse como arbitrario.

**La propuesta:** permitir gastar `Ratchet` además de `Reserve` para rerollear. Eso convierte
lo que te pasaba en algo que decidís pagar, y le da al jugador una palanca sobre su propia
progresión.

**El riesgo:** es la mecánica más importante del juego. Tocarla puede romper el balance
entero, y hace que el sacrificio deliberado sea todavía más fácil de ejecutar.

Es el upgrade más grande que el medio digital habilita, y también el más peligroso.

→ **../02-personaje/trinquete.md**

---

## A-06 — Cuántos "el jugador elige" se automatizan

**Arrastra:** cómo se siente jugar, minuto a minuto.

El manual repite decenas de veces que ante varias formas de resolver algo, decide el jugador
activo: qué enemigo mover cuando hay empate de distancia, en qué orden resolver efectos
simultáneos, en qué orden invocar cuando no alcanzan las figuras.

Cada una es o **un prompt** (tedioso, y son muchos por turno) o **una regla automática**
(pierde agencia, y a veces juega peor de lo que jugaría el jugador).

**Este es el impuesto que hace que los juegos de mesa digitales se sientan peores que el
cartón.** No hay solución general: hay que decidir caso por caso, y es más trabajo de UX que
de sistemas.

---

## A-07 — Quién puede romper paredes

**Arrastra:** poco, pero hay que decidirlo antes de construir el mapa.

Candidatos: el adversario al cambiar de stage, una criatura pesada, o una de las 2 acciones
propias de la misión.

Cada uno da un juego distinto: si rompe el adversario, la destrucción es una amenaza; si
rompe el jugador, es una herramienta.

→ **../01-fundamentos/mapa-y-espacios.md**

---

## A-08 — El título

`Project_C` es el nombre del repositorio, no del juego.

No se decide hasta tener el concepto cerrado y la temática elegida. Los dos glosarios traen
un título provisional cada uno, y **ninguno de los dos es una decisión** — están ahí solo
para que el documento tenga algo que imprimir en la primera página.

---

## A-09 — Quién es la autoridad del resultado del dado

**Arrastra:** el save game, los rerolls por skill, y cuánto le cree el jugador a la tirada.

El temario pide **físicas y colisiones** (clase 3) y los dados son el candidato natural. Pero
eso abre una pregunta que es de diseño antes que de implementación: **¿el resultado sale de la
simulación física, o la física es una animación de un resultado ya decidido?**

| Opción | A favor | En contra |
|---|---|---|
| **Decide la física** | demostración indiscutible; el jugador ve que no hay truco | el dado se cae de la mesa o queda de canto; guardar a mitad de tirada es un caso feo; cada reroll es una coreografía |
| **Decide la lógica, la física presenta** | determinista, save trivial, rerolls limpios | si se nota que está guionado, la tirada pierde credibilidad |
| **Híbrido** — decide la física, con un resolvedor que detecta "asentado" y cae a lógica si no se asienta | conserva la demostración y no se puede colgar | dos caminos que mantener |

**Recomendación: híbrido**, con una condición de arquitectura — que el **resolvedor**, y no el
actor del dado, sea lo único que declara el resultado. Así la física entra como *fuente* de un
valor y no como dueña del estado del juego, y las otras dos opciones siguen disponibles
después.

→ **../03-resolucion/dados.md** ·
**../../architecture/04-mapa-de-clases.md**
