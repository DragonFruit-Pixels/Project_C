# 📌 Registro de decisiones

← [Índice](../README.md)

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

→ [`../01-fundamentos/vocabulario.md`](../01-fundamentos/vocabulario.md)

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

→ [`../02-personaje/trinquete.md`](../02-personaje/trinquete.md)

---

## D-05 — Alcance: 1 misión y 1 adversario

**2026-08-19**

**Por qué:** es la unidad atómica del juego original — una caja de escenario más una de
jefe. Es una partida completa, no un recorte.

**Descarta:** la rejugabilidad por combinación (12 configuraciones en el original). La
rejugabilidad pasa a depender del trinquete y de las decisiones de build.

→ [`../00-vision/alcance.md`](../00-vision/alcance.md)

---

## D-06 — El mapa es un grafo, no una grilla

**2026-08-19**

**Por qué:** el manual define adyacencia por pasajes, no por posición. Las escaleras y
túneles vuelven adyacentes espacios lejanos, y todas las reglas de distancia son camino más
corto con aristas bloqueables. Una grilla obligaría a inventar reglas para cada uno de esos
casos; un grafo los resuelve nativamente.

**Beneficio lateral:** sirve igual para turnos que para zonas en tiempo real, así que no
compromete la forma final.

→ [`../01-fundamentos/mapa-y-espacios.md`](../01-fundamentos/mapa-y-espacios.md)

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

→ [`../../course-alignment.md`](../../course-alignment.md) · [D-13](#d-13--arquitectura-híbrida-c--blueprint)

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
procedencia en [`../07-balance/perillas-y-constantes.md`](../07-balance/perillas-y-constantes.md).

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

→ [`../../architecture/06-limite-cpp-blueprint.md`](../../architecture/06-limite-cpp-blueprint.md)

---

## Plantilla para agregar

```markdown
## D-NN — Título corto

**AAAA-MM-DD**

Qué se decidió, en una o dos frases.

**Por qué:** la razón real, no la racionalización.

**Descarta:** qué queda afuera por esta decisión.
```
