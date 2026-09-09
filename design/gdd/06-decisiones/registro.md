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

**Estado:** implementada el mismo día. Toolchain instalado (MSVC 14.44.35207, Windows SDK
10.0.26100.0), módulo `ProjectC` creado con las 5 clases base del framework, `URatchetComponent` y
`FRatchetRules`. `Project_CEditor` compila y linkea, y los 2 tests de automatización del track de
`Ratchet` pasan — verificados por mutación, no solo por estar verdes.

**Descarta:** "todo en C++", que en Unreal es un anti-patrón conocido —deja al contenido afuera y
obliga a recompilar para cada ajuste de balance. También descarta **GAS**: está construido
alrededor de predicción de cliente y tiempo real, y este juego es por turnos, determinista y
single player. Y descarta StateTree y MassEntity por ahora.

→ [`../../architecture/06-limite-cpp-blueprint.md`](../../architecture/06-limite-cpp-blueprint.md)

---

## D-14 — La temática es **biopunk**, provisionalmente

**2026-08-26**

Cierra [A-01](abiertas.md). Se elige la variante A —subciudad química, sumidero, el organismo
que colonizó el desecho— sobre la variante D&D. **Provisional** quiere decir que la decisión se
puede revisar sin costo, no que esté a medias.

**Por qué se puede revisar sin costo:** por [D-02](#d-02--la-temática-queda-sin-declarar-y-el-core-es-neutro).
El core habla en IDs neutros y la temática es data, así que elegir biopunk **no cambia una sola
línea de código**: cambia qué theme pack se carga. Auditado el 2026-08-26 — cero términos
temáticos en `Source/` y en `design/architecture/`.

**Descarta:** nada, todavía. La variante D&D queda entera en `docs/rulebook/glossary-dnd.md` y
sigue siendo aplicable mientras el core siga neutro.

---

## D-15 — Un adversario genérico: las stats son el diseño, la piel viene después

**2026-08-26**

Cierra [A-02](abiertas.md). Hay **1 adversario**, con stats y efectos **genéricos**. Qué criatura
es, cómo se ve y cómo se llama se decide con el arte, no ahora.

Su forma es data, cuatro veces:

```
DA_AdversaryStage  ×4
  ├─ Health          vida propia del stage
  ├─ BonusDice       se suman a TODOS sus ataques — acumulativo entre stages
  ├─ RevealEffect    se ejecuta una vez al descubrirse
  └─ PermanentMods   queda hasta el final de la partida — se CONSULTA, no se ejecuta
```

**Por qué:** el adversario nunca fue una decisión de código. Con el vocabulario neutro y los
stages como Data Asset, elegir criatura es autoría de contenido. Postergarlo no bloquea nada y
decidirlo temprano ata el arte a un diseño que todavía se está balanceando.

**Lo que sí destraba, y es lo único técnico acá:** `PermanentMods` **no existe**. El vocabulario
de efectos sabe ejecutar, no sabe responder. Hace falta un set chico y cerrado de modificadores
de regla —`+N dados a`, `−N al umbral del reloj`, `+N coste a una acción`, `prohibir acción`,
`+N spawn`— y con eso **cualquier boss futuro es data**. Es el agujero de escalabilidad de
bosses, cerrado.

**Descarta:** el contraste entre dos adversarios de textura distinta, que el original usa para
dar identidad. Ya estaba descartado por [el alcance](../00-vision/alcance.md).

---

## D-16 — El título no bloquea, y sale de la lista de decisiones de diseño

**2026-08-26**

Cierra [A-08](abiertas.md). `THE SUMP` sigue siendo provisional y el título se decide cuando
haya con qué — no es una dependencia de nada del desarrollo.

**Por qué:** no toca código, ni datos, ni arquitectura. Estaba en la lista de decisiones abiertas
como si arrastrara algo, y no arrastra nada.

---

## D-17 — La física del dado contesta un valor; nunca es dueña del estado

**2026-08-26**

Cierra [A-09](abiertas.md), y no por elegir entre las tres opciones sino porque una regla más
general la vuelve innecesaria.

Una tirada física **no es una animación: es una fuente que contesta un valor.** Es el mismo caso
que "esperar una decisión del jugador" — la pila de resolución se estaciona esperando una
respuesta— con otro respondedor:

```
la pila necesita un valor de dado
  ├── dados físicos: se tiran, se detecta "asentado", contestan la cara
  ├── si no se asienta en N segundos: contesta URandomSubsystem
  └── modo rápido: contesta URandomSubsystem directo
```

**Por qué:** sale de [`08-presentacion-y-reglas.md`](../../architecture/08-presentacion-y-reglas.md).
Si las reglas resuelven instantáneamente y la presentación reproduce, la física no puede ser
autoridad de nada — solo puede contestar. Un `UDiceResolver` en C++ sigue siendo el único que
**declara** el resultado; quién se lo sopló es indistinto para todo lo que venga después.

**Descarta:** que el actor del dado declare el resultado, que era lo que complicaba el save a
mitad de tirada y la coreografía de rerolls.

---

## D-18 — La forma: por turnos, con figuras 3D que caminan y cámara tipo XCOM

**2026-08-26**

Cierra A-10, que era la última decisión grande abierta y **no estaba registrada en ningún lado**
— vivía sólo en el estado de sesión.

- **Por turnos**, confirmado. No hay reinterpretación en tiempo real.
- **Movimiento:** el personaje **camina** con rig animado de sala en sala. No teletransporta.
- **Cámara:** controlada, con movimiento acotado, **al estilo XCOM** — no orbital libre ni fija.

**Por qué los turnos no eran realmente una elección:** la base mecánica *es* un juego de mesa
—dados, cartas, descarte visible, `Doom Track`—; pasarla a tiempo real no la adapta, la borra. Y
la regla que define el juego —el `Toll` cobra siempre, así que **no hacer nada es una jugada
legítima**— no existe sin turnos. Toda la arquitectura escrita ya lo asumía.

**Lo que confirma:** `AProjectCCharacter` sigue heredando de `ACharacter` y no de `APawn`. Había
una duda razonable —`CharacterMovementComponent` es mucha maquinaria para un tablero— y la
caminata animada la justifica. Alimenta el Blendspace y la State Machine de la clase 6.

**Lo que abre, y es nuevo:** el personaje camina **la arista del grafo**, y eso obliga a decidir
qué ruta física recorre → A-15.

**Descarta:** la representación de ficha que teletransporta de `Space` a `Space`, y la cámara
orbital libre.

---

## D-19 — Los modificadores de regla son un tag + una operación, no un tipo por perilla

**2026-08-26**

Cierra A-11. Un efecto permanente **no se ejecuta: se consulta.** El vocabulario de efectos
sabe ejecutar; esto es el mecanismo que le faltaba.

```
FRuleMod
  ├─ Rule    FGameplayTag   qué perilla toca
  ├─ Op      Add | Override | Forbid
  └─ Value   int
```

El namespace `Rule.*` arranca con:

| Tag | Default | Qué mueve |
|---|---|---|
| `Rule.Dice.Bonus.EnemyAttack` | 0 | dados que suman los ataques de enemigos |
| `Rule.Dice.Bonus.AdversaryAttack` | 0 | ídem para el adversario — es el `BonusDice` acumulado de los stages |
| `Rule.Dice.Bonus.PlayerAttack` | 0 | dados que suma el personaje activo |
| `Rule.Clock.Threshold` | 3 | `Doom Symbol` necesarios para que avance el reloj |
| `Rule.Move.Budget` | 3 | pasos de una acción `Move` |
| `Rule.Recover.Amount` | 3 | puntos que reparte `Recover` |
| `Rule.Action.Cost.*` | 1 | costo en acciones de una acción concreta |
| `Rule.Action.Forbidden.*` | — | inhabilita una acción |
| `Rule.Spawn.ExtraPerTurn` | 0 | invocaciones extra por turno |

**Por qué un tag y no un tipo de struct por perilla:** agregar una perilla nueva pasa a ser
**agregar un tag y un sitio de consulta**, no declarar un tipo y tocar a todos los llamadores. Es
el punto donde los Gameplay Tags nativos dejan de ser una buena idea abstracta y empiezan a
pagar.

**Dónde viven los mods activos:** en `AMissionGameState`. Son estado de la misión que todos
leen — el HUD tiene que poder mostrar "el reloj avanza más rápido".

**La disciplina que lo hace funcionar, y sin la cual esto no sirve:** *todo* sitio que use uno de
esos valores lo pide con `QueryMod(Rule.X, default)`. Un solo lugar que lea la constante directo
es un modificador que silenciosamente no hace nada — y ese bug no tiene síntoma, sólo se nota
jugando.

**Consecuencia:** cualquier boss futuro es data. Era el agujero de escalabilidad de bosses.

---

## D-20 — La pila de resolución serializa el **resultado**, no la semilla

**2026-08-26**

Cierra A-12. Cada `FResolutionStep` guarda lo que pasó —las caras que salieron, qué se
eligió— y no la semilla que lo produjo.

**Por qué:** con la semilla, deshacer una tirada y rehacerla da un resultado distinto. Eso es
*save scumming*, y desarma la tensión del `Toll`, que es la regla que define el juego. Con el
resultado guardado, deshacer y rehacer da lo mismo y el undo sólo corrige un click.

**No decide si hay undo** — decide que va a ser posible. Al revés no: con la semilla guardada, el
undo queda descartado para siempre y no se retrofitea.

**La semilla igual se guarda, pero de la misión, no del paso.** Sirve para reproducir una partida
entera y perseguir un bug; no para re-tirar un dado suelto.

---

## D-21 — Un nivel por misión, con el grafo **sellado** antes de la primera consulta

**2026-08-26**

Cierra A-13. Todo el mapa carga al inicio. Los sublevels por sala siguen permitidos **como
unidad de autoría** —que es lo que deja a dos personas editando salas distintas sin pisarse—
pero se cargan todos al arranque, no bajo demanda.

`UGraphSubsystem` **rechaza consultas hasta recibir el sellado**, que ocurre cuando terminaron de
registrarse todos los `ASpace` y pasaron los 8 invariantes de F8.

**Por qué:** `ASpace` se registra en `BeginPlay`. Con streaming, una sala que llega tarde deja el
grafo incompleto, y un BFS sobre un grafo incompleto **no crashea: contesta mal** — devuelve "no
hay camino" donde había uno. En un juego donde el camino más corto decide a dónde se mueven los
enemigos, mentir es peor que crashear.

Y para un tablero el streaming no compra nada: el mapa entero está presente desde el turno 1 por
diseño.

**Descarta:** el streaming de salas bajo demanda como técnica de performance. El tema sigue
demostrable con la carga inicial de sublevels.

---

## D-22 — La caminata sigue **waypoints por arista**, no NavMesh

**2026-08-26**

Cierra A-15, que abrió [D-18](#d-18--la-forma-por-turnos-con-figuras-3d-que-caminan-y-cámara-tipo-xcom).
Cada arista lleva una lista ordenada de waypoints, autorada con la sala. Sin waypoints, el
recorrido es la recta entre los dos `Space` — que alcanza para las aristas internas de una sala.

**Por qué no NavMesh:** una pared es la **ausencia de arista**, y puede no existir como
geometría. Dos `Space` sin arista pueden estar visualmente abiertos, así que `MoveTo` puede
rodear por una ruta que las reglas prohíben — y la figura camina un camino que el juego no
permitía. Es la regla de [`08-presentacion-y-reglas.md`](../../architecture/08-presentacion-y-reglas.md)
aplicada al movimiento: **la presentación no puede desmentir a las reglas.**

**Costo:** con ~20 nodos y grado promedio 2.5 son unas 50 aristas. Con 1 a 3 `Space` por sala, la
mayoría es "centro → puerta → centro": dos o tres puntos, no una spline elaborada.

**El NavMesh se sigue necesitando** para la clase 9, pero para que el Behaviour Tree **decida** —
no para que la figura **se mueva**.

---

## D-23 — `APlayerState` se adopta con las estadísticas de la partida; GAS se documenta, no se usa

**2026-08-26**

Cierra A-14. Los dos son temas de la cursada sin encaje obvio en un single player por turnos, y
la respuesta correcta es distinta para cada uno.

### `APlayerState` — se adopta

Tenía un lugar real y estaba vacío porque nadie lo buscó. Lo que lo define es que **sobrevive a
la muerte del Pawn**, y acá eso describe algo concreto: las estadísticas de la partida siguen
vivas cuando una figura se pierde en el track.

`AMissionPlayerState` guarda `TollTaken`, `RerollsSpent`, `EnemiesKilled`, `CharactersLost` y
`SpacesMoved`.

**Es el hogar correcto por descarte, no por relleno:** no son reglas de la misión (`GameMode`),
ni estado del tablero que todos leen (`GameState`), ni cruzan de misión (`GameInstance`). Son
del jugador y duran lo que dura la partida.

Y hacen falta igual: **la pantalla de fin de misión no tiene de dónde sacar esos números.** Son
cinco contadores que se iban a escribir de todos modos; lo único que se decidió es dónde viven.

### GAS — sigue descartado, pero con el mapeo escrito

No usarlo no significa no conocerlo. El mapeo concepto por concepto quedó en
[`06-limite-cpp-blueprint.md`](../../architecture/06-limite-cpp-blueprint.md): `UAttributeSet` →
los componentes de barras, `GameplayEffect` infinito → **`FRuleMod`** ([D-19](#d-19--los-modificadores-de-regla-son-un-tag--una-operación-no-un-tipo-por-perilla)),
`GameplayAbility` → `UMissionAction`, `AbilityTask` → la pila de resolución. Los Gameplay Tags sí
se adoptan y ya estaban adoptados.

**Por qué no se fuerza:** el modelo de ejecución de GAS —instant / duration / periodic sobre
atributos— no expresa "una carta que se resuelve paso a paso donde un paso que no se puede
ejecutar se saltea", que es la semántica que vale para todo el juego. Y la mitad del peso de GAS
es predicción de cliente y replicación, maquinaria para un problema que este juego no tiene.

Saber **por qué no**, con el mapeo delante, es más sólido que bolteárselo encima a un juego que
no lo pide.

**Queda por consultar en la cursada:** GAS aparece en la slide de contenidos generales de la
clase 1, pero **no figura en ninguna de las 19 clases del cronograma**. Es una inconsistencia del
material, no del diseño.

## D-24 — La selección es **un estado**, no cuatro eventos

**2026-08-27**

`ISelectable` expone `SetHighlight(ESelectionHighlight)` —`None`, `Legal`, `Hovered`,
`Selected`— en vez de `OnHoverBegin` / `OnHoverEnd` / `OnSelected` / `OnDeselected`. Quién decide
el estado es `AMissionPlayerController`, con precedencia `Selected` > `Hovered` > `Legal`.

**Por qué:** con cuatro eventos, cada implementador tiene que reconstruir el estado a partir de
la secuencia de avisos, y el primer caso raro lo rompe: hoverear algo que ya está seleccionado
manda `OnHoverBegin`, y al salir el objeto no sabe si volver a "normal" o a "seleccionado". Con
un estado, el único que conoce las tres cosas a la vez decide una vez y el Blueprint sólo dibuja.

**Descarta:** reaccionar a la *transición* (un sonido sólo al empezar el hover) sin comparar
contra el estado anterior. Es barato de agregar si hace falta; el bug del resaltado huérfano no
lo era.

---

## D-25 — Dos excepciones a "no hay Tick", las dos en presentación

**2026-08-27**

`BP_CameraPawn` tickea para interpolar pan, zoom y orbit. `AMissionPlayerController::PlayerTick`
traza bajo el cursor una vez por frame para el hover. Nada más del proyecto tickea.

**Por qué:** la regla de `01-por-donde-se-empieza.md` es sobre las **reglas** — que ningún estado
de juego dependa del paso del tiempo. Estas dos son presentación pura y no tocan estado. Sin
interpolación la cámara salta; trazando sólo cuando el mouse se mueve, mover la cámara con el
mouse quieto deja el resaltado pegado en el espacio equivocado.

**Descarta:** poder decir "el proyecto no usa Tick" sin matices. A cambio, la frase que queda es
verificable: *ninguna clase de reglas tickea*.

---

## D-26 — La figura lleva un volumen de selección aparte de su cápsula

**2026-08-27**

`AProjectCCharacter` suma un `USphereComponent SelectionBounds` con perfil `Figure`. La cápsula
conserva su perfil de movimiento.

**Por qué:** el perfil `Figure` es `QueryOnly` e ignora todo salvo el canal `Selectable`.
Pisárselo a la cápsula deja al personaje sin piso, y el síntoma aparecería recién con la
locomoción de la clase 6 — muy lejos de la causa. Es además el patrón que `ASpace` ya usa con su
`Bounds`: un componente cuyo único trabajo es ser trazable.

**Descarta:** un componente menos por figura. Se paga barato y compra que colisión y selección no
compartan destino.

---

## D-27 — Los placeholders salen de `/Engine/BasicShapes`, no de Meshy

**2026-08-27**

Un `Space` es un Cube escalado y una figura es un Cylinder, los dos del contenido del motor. El
mannequin (`SKM_Manny`) **no** viene con el engine sino con el content pack Third Person, y esa
decisión se pospone a la clase 6, cuando el Animation Blueprint lo necesite de verdad.

**Por qué:** las formas básicas no pesan en el repo, no gastan créditos y no hay nada que
esperar. Y sobre todo: con un `Space` que se ve lindo se deja de mirar si el grafo anda. Las
reglas se prueban antes que el arte.

**Descarta:** que la demo se vea presentable. Es a propósito.

---

## D-28 — 2 skills por personaje y 3 niveles, provisional

**2026-08-27**

El manual da 3 skills por personaje (1 de firma + 2 de un pool común de 6) con 4 niveles cada
una: 24 celdas. Esta versión baja a **2 skills** y **3 niveles**, y construye 18 celdas. El
nivel 4 no se borra de la documentación: se marca `fuera de alcance`.

Qué 4 personajes entran y qué 2 skills lleva cada uno lo decide el diseñador a mano, y sigue
abierto → [A-08](abiertas.md).

**Por qué:** volumen de contenido para la entrega de la cursada. 24 celdas de skill, cada una
con su efecto, su balance y su fila de Data Table, es la pieza de contenido más grande del
proyecto y la que menos aporta a demostrar los temas del temario.

**Descarta, y hay que saberlo:** la elección de build. Con 6 umbrales de `Ratchet` que dan 1
nivel cada uno y sólo 4 subidas disponibles, la única repartición legal es `(3,3)` — **toda
partida de todo personaje termina con sus 2 skills al máximo**, y dos umbrales pierden su
subida. La decisión pasa a ser de orden y se disuelve al 4.º umbral.

Eso contradice lo que el [high concept](../00-vision/high-concept.md) declara como motor de
rejugabilidad ("qué subís y en qué orden") y neutraliza el argumento 1 de
[`reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md). Se acepta a sabiendas: es contenido
contra variedad, y hoy pesa más entregar.

**Cómo se revierte, si en playtest molesta:** volver a 4 niveles, o dejar 3 niveles y subir a 3
skills. Las dos dan techo 6 y encajan exacto con los 6 umbrales. Ninguna toca código: son
filas de `DT_Skills`.

---

## D-29 — El código se escribe en inglés; el diseño, en castellano

**2026-08-29**

Todo lo que vive bajo `Source/` —comentarios, mensajes de `UE_LOG`, nombres y aserciones de
test— va en inglés y en ASCII puro: sin acentos y sin rayas largas. Los 41 documentos de
`design/` y `docs/` siguen en castellano. **La frontera es el borde de `Source/`.**

**Por qué:** el vocabulario del dominio ya estaba en inglés por regla propia
([vocabulario.md](../01-fundamentos/vocabulario.md): "los IDs no se traducen"). El código
quedaba entonces partido de la peor manera —identificadores en inglés explicados en
castellano— y esa costura se abre de nuevo en cada archivo nuevo. Los documentos de diseño son
lo contrario: se entregan y se defienden en una materia que se cursa en castellano.

El ASCII no es una preferencia estética. "Cero bytes fuera de ASCII" se verifica con un `grep`
y no se afloja con el tiempo; "no hay castellano" es una lectura humana que sí. De paso evita
los avisos `C4819` de MSVC en archivos sin BOM.

**Descarta:** que el profesor lea las explicaciones del código en su idioma. Se acepta porque
lo que se evalúa como texto son los documentos, y esos no se tocaron.

**Dónde se aplica sola:** `.claude/rules/unreal-code.md`, que ya está acotado a `Source/**`.

---

## D-30 — La cámara vive entera en Blueprint

**2026-08-30**

`BP_CameraPawn` dejó de heredar de `ACameraPawn` y pasó a heredar de `APawn`. Los componentes,
las 15 variables, `BeginPlay`, `Tick` y los tres eventos de Enhanced Input están en el grafo, y
`CameraPawn.h/.cpp` se borraron junto con los tres handlers que tenía `AMissionPlayerController`.
**No quedó C++ que calcule nada del encuadre.**

**Por qué:** C++ es la clase 14. Las clases 1 a 13 se ensenñan en Blueprint, así que una cámara
resuelta en C++ está resuelta donde nadie la va a buscar — lo había marcado
[`course-alignment.md`](../../course-alignment.md) con esas palabras. Posesión es tema de la
clase 4 y el input también, y ahora los dos se leen en el mismo grafo: el pawn que el jugador
posee es el que escucha las teclas, sin controller en el medio y sin interfaz.

De paso arregló un bug que el C++ arrastraba: leía el yaw de vuelta del `SpringArm`, y una
rotación relativa vuelve normalizada a [-180, 180] mientras el yaw objetivo crece sin límite.
Orbitando más de media vuelta la cámara barría por el lado largo. Ahora `CurrentYaw` espeja lo
último que se escribió en vez de leerlo del componente.

**Descarta:** el tipado fuerte y el `Cast<ACameraPawn>` que daba autocompletado desde el
controller. A cambio, las 8 perillas de `Camera|Tuning` son `Instance Editable` y se tocan en el
panel de detalles **durante el PIE**, que es exactamente lo que un Blueprint compra y lo que la
materia quiere ver.

**No contradice [D-13](#d-13):** la arquitectura híbrida sigue en pie. Las reglas —`FGraphMath`,
`FRatchetRules`, `UGraphSubsystem`— siguen en C++ con sus tests. Lo que se movió es
presentación, que es dónde dice [`08-presentacion-y-reglas.md`](../08-presentacion-y-reglas.md)
que tiene que estar.

---

---

## Plantilla para agregar

```markdown
## D-NN — Título corto

**AAAA-MM-DD**

Qué se decidió, en una o dos frases.

**Por qué:** la razón real, no la racionalización.

**Descarta:** qué queda afuera por esta decisión.
```
