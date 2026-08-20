# 🗺️ El mapa y los espacios

← [Índice](../README.md)

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
| Spawn en una `Gate` que ya tiene 5 enemigos | Se apila sin problema. La única razón para que un spawn falle es que no haya figura en el pool → [`04-oposicion/enemigos.md`](../04-oposicion/enemigos.md) |
| El espacio se limpia a mitad de la fase 3 | Los enemigos dejan de atacar, pero **no** se roba recompensa: lo que decide es el estado al **empezar** la fase (manual p. 16) |
| `Adversary` todavía en el `Doom Track`, no manifestado | No cuenta como enemigo → el espacio puede estar `Clear` (manual p. 7) |
| Dos secciones del mapa unidas por un único pasaje bloqueable | Existe un estado con el objetivo a distancia infinita. El invariante F8.7 lo tiene que rechazar **en editor**. El manual no lo cubre: es un problema que aparece porque nosotros elegimos armar el mapa por salas |

---

## 🎛️ Perillas de balanceo

> **El dueño de los defaults y los rangos seguros es**
> [`07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md). Si algo de esta tabla difiere de ahí,
> gana ese archivo. Las probabilidades usan la **distribución provisional** declarada
> ahí mismo: `p(Hit) = 0,50`, `p(Toll) = 0,33`.


| Perilla | Default | Rango seguro | Qué afecta |
|---|---|---|---|
| Espacios por sala | 1–3 (manual p. 9) | 1–4 | La granularidad del movimiento. Salas de 1 espacio hacen que 3 pasos cubran 3 salas y el mapa se sienta chico; salas de 3 hacen que cada sala sea un problema en sí misma |
| Total de `Space` del mapa | **No está en el manual** (depende del episodio; el techo físico son 16 tiles). Propuesta: 20 | 12–32 | El diámetro del mapa, y por lo tanto cuánto tarda en llegarte un enemigo empujado. Abajo de 12 no hay a dónde escapar y "te siguen" deja de amenazar, porque ya estás rodeado siempre |
| Grado promedio del grafo | **No está en el manual.** Propuesta: 2.5 | 2.0–3.5 | Cuántas rutas de escape hay. Arriba de 3.5 el mapa es una malla y arrastrar enemigos deja de doler; abajo de 2.0 es un pasillo y cualquier bloqueo parte el mapa en dos |
| Cantidad de `Gate` | 3 (manual p. 9) | 2–4 | Cuán repartida y cuán predecible es la presión. **Tocarla rompe el formato de las cartas de presión**, que nombran una `Gate` por color → [`04-oposicion/enemigos.md`](../04-oposicion/enemigos.md) |
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
