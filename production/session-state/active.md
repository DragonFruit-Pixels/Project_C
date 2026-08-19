# Session State — Project_C

**Updated:** 2026-08-18

## Project
- **Name:** Project_C
- **Division:** Game Dev (Unreal)
- **Stage:** A — Greenfield (proyecto Unreal vacío, sin diseño ni implementación)
- **User role:** Solo dev (diseño + código + arte)

## Stack detected
- **Engine:** Unreal Engine 5.8 (`Project_C.uproject`)
- **Project type:** Blueprint-only (sin `Source/`, sin módulo C++)
- **Plugins enabled:** ModelingToolsEditorMode (Editor)
- **Content:** vacío (0 assets)
- **Config:** DefaultEditor/Engine/Game/Input.ini (defaults de UE)
- **VCS:** ninguno (no es repo git)
- **Arcane:** v2.1.0 — perfiles core + unreal-dev + unity-design + statusline (80 skills, 13 rules, agents: quality, game)

## Existing artifacts
- Ninguno. Sin GDD, sin art bible, sin specs, sin código.

## Session intent
Doble vía en paralelo:
1. **Ideación desde 0** — el usuario tiene una idea semilla pero quiere idearla estructuradamente.
2. **Setup técnico profesional** — dejar el proyecto Unreal con base sólida antes de producir.

## Recommended flow
1. `/brainstorm` — concepto: género, core loop, pilares, fantasía de jugador, referencias → game-concept.md
2. `/ue-project-context` — documento de contexto técnico: módulos, plataformas target, convenciones, estándares
3. `/map-systems` — descomponer el concepto en sistemas, dependencias y orden de diseño

## Ubicación
Mudanza **completada**. El proyecto vive en `C:\Users\sebas\UnrealProjects\Project_C`
(no en `UnrealEngine\`, que fue el destino que se planeó y se descartó).
La ubicación vieja (`Documents\Unreal Projects\Project_C`) ya no existe.
Remote: https://github.com/DragonFruit-Pixels/Project_C — repo **público**.

## Technical setup backlog
- [x] `git init` + `.gitignore` de Unreal + Git LFS para assets binarios
- [x] MCP del editor Unreal (`/install-mcp`) — plugins `ModelContextProtocol` + `AllToolsets`
      habilitados; auto-start en `Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini`;
      `.mcp.json` apunta a `http://127.0.0.1:8000/mcp`. **Falta aprobar el server y abrir el editor.**
- [x] MCP de Meshy (`/meshy-setup`) — registrado en `.mcp.json` vía `npx @meshy-ai/meshy-mcp-server`.
      **Falta la `MESHY_API_KEY` como variable de entorno de usuario** (no alcanza con `.env`:
      la expansión `${MESHY_API_KEY}` lee el entorno del proceso, no el archivo).
- [ ] Módulo C++ (`Source/`) vía `/ue-module-build-system` — decidir C++ vs Blueprint-only
- [ ] Estructura de carpetas de `Content/` + naming conventions (`/ue-naming-conventions`)
- [ ] Test/automation harness (`/test-setup`, `/ue-testing-debugging`) — **bloqueado** hasta
      decidir C++ vs Blueprint-only, que a su vez depende del concepto
- [ ] Plataformas target + performance budgets

## Deuda conocida
- **Arcane degradado por el install de Meshy.** El perfil `meshy` se instaló desde un cache
  viejo (2.1.0), y eso: bajó `arcane_version` 2.5.0 → 2.1.0, borró el bloque `content_hashes`
  del manifest, y **borró los 21 templates de `.claude/skills/_templates/`** (recuperados con
  `git checkout`). Los skills en disco son mezcla de 2.5.0 + meshy. Fix: `arcane update` y
  reinstalar el perfil desde el cache nuevo (`ca092ff3113b`).

## Concepto (decidido)
El juego reimplementa las mecánicas de **Cthulhu: Death May Die** (Season 3) cambiando la
temática lovecraftiana por biopunk. El manual del juego de mesa está en `docs/DMD-S3-WIP-Rulebook.pdf`.

**Tono:** subciudad química vertical. Un cartel refina un mutágeno que da fuerza a cambio del
cuerpo. En el fondo del sumidero un **organismo vivo** colonizó décadas de desecho químico y
lleva generaciones creciendo. El cartel no quiere destruirlo: quiere refinarlo y usarlo,
convencido de que se puede dirigir.

**Por qué biopunk le queda bien (no es un reskin arbitrario):** locura y mutación son
mecánicamente isomorfas — trinquete de una sola dirección, umbrales que transforman, poder a
cambio de humanidad. Pero en un videojuego la mutación es *mejor* que la locura porque **se ve**
en el modelo del personaje, mientras la locura necesita filtros y voces en off.

**Vocabulario:** `docs/rulebook/glossary-biopunk.md`, 66 términos. Los cuatro que arrastran el
resto: Investigator→**Sinker**, Elder One→**the Growth**, Sanity→**Grip** / Insanity Threshold→**Slip**,
Ritual→**the Batch**.

**Restricción de IP:** el registro (subciudad, sumidero, mutágeno) es de dominio común. No se
toca nomenclatura ajena: Zaun, Shimmer, Piltover, chem-baron, Hextech. Ni de DMD: Elder One,
Mythos, Elder Sign, nombres de investigadores.

**Título:** `THE SUMP` es provisional, NO una decisión. Nombrar el juego se decide con el
concepto en mano.

## Variantes temáticas exploradas (2026-08-19)

Se exploraron dos direcciones además de biopunk, con paneles de agentes de 3 lentes cada uno.
**Criterio del usuario: cuantas más palabras en común con DMD y más se parezca, PEOR.**
Medido con `scratchpad/measure_distance.py` — cuántos términos propuestos usan una palabra
que el manual original ya usa:

| Variante | Distancia | Estado |
|---|---|---|
| **biopunk** | 100% | documentos hechos, 9/9 verde |
| **D&D corrupción** | 100% | documentos hechos, 9/9 verde |
| | | *ambas: **0** reutilizaciones sobre el glosario completo* |
| LA DEUDA (oficina de cobro) | 95% | solo panel |
| LA OBRA (gremio de albañiles) | 91% | solo panel |
| Eclesiástica | 82% | solo panel, descartada |
| Folk / Arcana | 73% / 55% | solo panel, descartadas |

**El hallazgo que importa (el techo):** el parecido con DMD no vive en el léxico, vive en la
**silueta mecánica** — un boss único que despierta en 4 etapas, un reloj de fatalidad, puntos
de spawn, y un recurso de una sola dirección que da poder y mata. Mientras se conserven las
mecánicas, eso no se puede esconder con ningún vocabulario. Por eso dark fantasy "pura"
(culto + rito + dios + locura) fracasó: era DMD con otros sustantivos.

**Lección de proceso:** al primer panel de dark fantasy le dije que "dejar términos quietos
cuenta a favor" y eso lo empujó justo contra el criterio del usuario. Los *keeps* son el peor
caso, no una virtud.

## Documentos del manual (hechos, 2026-08-19)
- `docs/rulebook/rulebook.md` — transcripción forense 1:1, 28 páginas, 1367 líneas.
  Incluye contenido que la capa de texto del PDF no tiene: las 6 tarjetas de skill solo
  existen como imagen y se transcribieron leyendo la página a 430 DPI.
- `docs/rulebook/glossary-biopunk.md` — 66 términos + política de casing + "do not rename".
- `docs/rulebook/rulebook-biopunk.md` — vocabulario biopunk, mecánicas intactas.
- `docs/rulebook/glossary-dnd.md` — 67 términos, con procedencia legal criatura por criatura.
- `docs/rulebook/rulebook-dnd.md` — vocabulario D&D dark fantasy, mecánicas intactas.
- Validador: `scratchpad/validate.py <variante>` (V1-V5; biopunk y dnd ambos 9/9 verde),
  `normalize_case.py <variante> [--apply]` y `measure_distance.py`.
  **Ojo: el scratchpad es de sesión y se pierde.** Si hay que re-verificar, hay que reescribirlos.
  Vale considerar moverlos a `docs/rulebook/tools/` para que sobrevivan.

### La variante D&D en detalle
Premisa: bajo una ciudad muerta hay algo **encadenado y filtrando** icor. Los **Binders**
completan **the Working** para atarlo a su voluntad. Los **Sworn** bajan a arruinarlo, y el icor
del piso conviene tomarlo. Trinquete estilo BG3 pero **atado al jefe**: el poder que tomás *es*
el jefe, y al fondo del track no morís, te **perdés en la brood**.

Cinco mecánicas que en DMD son arbitrarias y acá se explican solas:
- Los 4 stages del jefe = **categorías de edad de un dragón** (wyrmling → young → adult → ancient)
- Los tokens de **Fire** = el aliento de `KHARZUL, THE RED`
- `Kobold` como enemigo básico = **en D&D los kobolds sirven a los dragones**
- `Wyrmling` como minion de la caja = **son sus crías**
- `Hoard card` para el loot = **el tesoro del dragón**

**Restricción de IP verificada:** el SRD 5.1 está bajo CC-BY-4.0, pero de **beholder y mind
flayer/illithid solo aparece el nombre** — sin stat block, descripción ni arte. `THE HUNDRED EYES`
ocupa ese lugar pero está **diseñado, no copiado**. El parásito de BG3 es tropo + mecánica (libre);
el illithid no. El resto del bestiario es SRD o mitología de dominio público, con la procedencia
anotada por criatura en el glosario.

**Decisión de diseño de los documentos:** los dos MD están **alineados línea a línea**
(1367 == 1367). Eso es deliberado y es lo que hace verificable la equivalencia mecánica. Por eso
la política de casing es "espejar el original" y los tokens de icono quedan byte-idénticos
(`[INSANITY-THRESHOLD]` sigue diciendo INSANITY en el doc biopunk). Si alguna vez se renombran
los tokens, se rompe esa propiedad y hay que rehacer el validador.

## Next action
La decisión de diseño más grande sigue abierta: **¿juego de mesa digital fiel** (turnos, dados,
cartas, tablero de casillas — línea Gloomhaven Digital) **o reinterpretación en tiempo real**
que conserva el trinquete de mutación y tira los dados? Multi-año y desafío-maestría admiten
las dos y salen juegos completamente distintos.

`/brainstorm` quedó en Fase 1 sin terminar. Faltan del usuario: los 3 juegos con más horas
(es la pregunta que decide el eje de arriba), un momento concreto que se le quedó pegado, y
las restricciones reales de solo dev. Nada escrito en `design/gdd/`.
