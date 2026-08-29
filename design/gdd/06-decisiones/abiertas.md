# ❓ Decisiones abiertas

← [Índice](../README.md)

Lo que falta decidir, con qué está en juego en cada una. Ordenadas por cuánto arrastran al
resto del proyecto.

Cuando una se cierra, se mueve a [`registro.md`](registro.md) con su razón.

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

## A-08 — Qué 4 personajes entran, y qué 2 skills lleva cada uno

**Arrastra:** bloquea `DT_Skills` y las fichas de personaje. No bloquea nada de código.

El manual da 3 skills por personaje —1 de firma y 2 de un pool común de 6— con 4 niveles cada
una. Para esta versión se baja a **2 skills por personaje**. La asignación concreta la hace el
diseñador a mano, personaje por personaje: no hay regla que la derive.

**Lo que queda dentro de esta misma decisión:** si esas 2 son *firma + 1 común* o *2 comunes*.
No es cosmético — si se cae la de firma, los 4 personajes quedan mecánicamente
intercambiables, y con ellos se cae la jugada de quemar deliberadamente a uno, que solo duele
si ese personaje hacía algo que los otros no.

**Y cuántos niveles quedan, que es lo que puede romper el reloj de progresión.** Con 6
umbrales de `Ratchet` que dan 1 nivel cada uno:

| Forma | Subidas disponibles | Umbrales que pierden su subida |
|---|---|---|
| 3 skills × 4 niveles (manual) | 9 | 0, y sobran 3 subidas sin usar |
| **2 skills × 4 niveles** | **6** | **0, encaje exacto** |
| 2 skills × 3 niveles | 4 | **2** |

Con 3 niveles, dos umbrales de cada partida pierden su subida de skill. **No quedan en nada** —
siguen disparando la `Ratchet Card` y el dado bonus donde corresponda— pero el propio
[`skills.md`](../02-personaje/skills.md) trata ese caso como imposible ("solo puede pasar si el
track tiene más de 9 umbrales") y pasaría en todas las partidas.

→ [`../02-personaje/skills.md`](../02-personaje/skills.md),
[`../02-personaje/trinquete.md`](../02-personaje/trinquete.md)

---
