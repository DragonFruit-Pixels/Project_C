# Decisiones abiertas

← [Índice](../README.md)

Lo que falta decidir, con qué está en juego en cada una. Ordenadas por cuánto arrastran al
resto del proyecto.

Cuando una se cierra, se mueve a [`registro.md`](registro.md) con su razón.

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

Con [uno solo](../00-vision/alcance.md), **el adversario es la identidad del juego**. No hay
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

→ [`../04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)

**Es la más urgente de las tres primeras**, porque no se puede playtestear nada sin resolverla.

---

## A-04 — Qué reemplaza los límites de componentes

**Arrastra:** la dificultad general.

En el juego físico los componentes son finitos, y eso funciona como **tope blando de
dificultad**: si no quedan figuras, el spawn se saltea.

En digital ese límite deja de ser físico. Se puede replicar tal cual, subir, o sacar.
**Sacarlo endurece el juego** y obliga a re-balancear todo lo demás.

→ [`../04-oposicion/enemigos.md`](../04-oposicion/enemigos.md)

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

→ [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md)

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

→ [`../01-fundamentos/mapa-y-espacios.md`](../01-fundamentos/mapa-y-espacios.md)

---

## A-08 — El título

`Project_C` es el nombre del repositorio, no del juego.

No se decide hasta tener el concepto cerrado y la temática elegida. Los dos glosarios traen
un título provisional cada uno, y **ninguno de los dos es una decisión** — están ahí solo
para que el documento tenga algo que imprimir en la primera página.
