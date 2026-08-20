# 🎓 Alineación con el temario de la materia

Cruce entre el temario de la cursada (Unreal, 2026) y el diseño de Project_C, para poder
verificar cobertura contra una lista concreta en vez de a ojo.

**Objetivo declarado por el usuario: cumplir el temario en su totalidad.** Ningún tema
puede quedar sin un lugar propio en el juego.

## Fechas

| Hito | Fecha | Cubre hasta |
|---|---|---|
| Estado al escribir esto | 19/8/2026 | clase 2 dictada |
| Clase 3 | 20/8/2026 | — |
| **Entrega 1er Parcial** | **24/9/2026** | **clases 1-7** |
| **Entrega 2do Parcial** | **12/11 y 19/11/2026** | **clases 1-14** |
| Recuperatorio | 26/11/2026 | — |
| Final | 17/12/2026 | — |

**36 días hasta el 1er parcial. 85 hasta el 2do.**

## Condiciones de la cursada

- **No hay consigna escrita.** Por lo tanto **esta matriz de cobertura es el contrato**:
  es lo que se chequea tema por tema antes de cada entrega.
- **La entrega es grupal.** Ver la sección de trabajo en paralelo más abajo: en Unreal esto
  es un problema técnico antes que organizativo.
- **Se evalúa el juego Y la demostración de los temas.** Consecuencia directa: ningún tema
  puede quedar afuera aunque el juego no lo necesite. Chaos Destruction y Landscape dejan
  de ser opcionales.
- **El alcance no es una restricción** (decisión del usuario). El tamaño del equipo, sí.

## Restricciones duras que impone el temario

1. **Blueprint-only.** C++ es la clase 14, después del 2do parcial. Cierra el pendiente
   "C++ vs Blueprint-only" del backlog técnico: va Blueprint.
2. **La capa de datos es de 2do parcial.** Data Assets, Data Tables, Gameplay Tags y Save
   Game son clase 12 (22/10). El 1er parcial va con reglas en Blueprint, no en datos.
3. **Los personajes se mueven en 3D con locomoción animada.** Blendspace y State Machine
   (clase 6) lo exigen. Descarta la representación de ficha que teletransporta de casilla
   a casilla.
4. **Hay que llegar a un build empaquetado.** Packaging es clase 13.
5. **La forma converge a por turnos con presentación 3D.** Animación, posesión, NavMesh,
   Behaviour Trees, percepción y EQS piden agentes moviéndose en un espacio 3D; nada del
   temario pide combate en tiempo real.

## Matriz de cobertura

`P1` = tiene que estar para el 1er parcial. `P2` = para el 2do.

### Fundamentos del Gameplay

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 1 | Estructura de proyecto, Source Control | Ya hecho: repo, LFS, .gitignore, convenciones | listo |
| 1 | Templates, Editor | Ya hecho: proyecto Blueprint-only creado | listo |
| 2 | Actor, Blueprint, Componentes | Personaje, enemigo, dado, token, espacio del grafo | P1 |
| 2 | Ciclo de vida del Actor | Spawn de enemigos en las 3 puertas; muerte y retorno al pool | P1 |
| 2 | Depuración de Blueprints | Visualización del grafo de espacios y del cálculo de distancias | P1 |
| 3 | Gameplay Framework | GameMode = árbitro de reglas. PlayerController = el jugador. Pawn = personaje activo | P1 |
| 3 | Static Mesh | Salas, personajes, enemigos, dados | P1 |
| 3 | **Físicas y Colisiones** | **Los dados.** Tirada física real con colisión | P1 |
| 3 | Materiales | Caras de los dados, estado de las salas, feedback de trinquete en el personaje | P1 |

### Mecánicas y Lógica del Juego

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 4 | Inputs | Selección de personaje, acciones, movimiento | P1 |
| 4 | **Posesión** | **Controlar los 4 personajes y alternar entre ellos.** Encaje directo | P1 |
| 4 | Raycast | Seleccionar dados, espacios y enemigos con el mouse | P1 |
| 5 | Diseño de clases | Jerarquía Figure -> Character / Enemy -> Adversary. Space, Deck, Track | P1 |
| 5 | Comunicación entre Blueprints | Interfaces y event dispatchers entre árbitro, personajes y HUD | P1 |
| 5 | **Levels y SubLevels** | **Cada sala es un sublevel.** Arma el mapa por composición | P1 |
| 12 | **Data Assets y Data Tables** | Cartas de presión, cartas de botín, skills, tipos de enemigo, stages del jefe | P2 |
| 12 | **Gameplay Tags** | Tipos de enemigo, fuentes de daño, efectos de carta, condiciones. Es el mecanismo que hace el core agnóstico al tema | P2 |
| 12 | Save Game | Guardar partida en curso y progresión entre partidas | P2 |

### Animación y Efectos Visuales

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 6 | Animation Blueprint | Personajes y enemigos | P1 |
| 6 | Blendspace | Locomoción direccional al moverse entre espacios | P1 |
| 6 | State Machine | Idle / caminar / atacar / herido / muerto | P1 |
| 6 | Notifies | Frame de impacto del ataque, pasos, tirada de dados | P1 |
| 7 | **UMG** | HUD completo: 3 barras por personaje, track del jefe, pila de descarte visible, cartas, dados, skills | P1 |
| 7 | Sonidos | Dados, impactos, umbral de trinquete, avance del jefe | P1 |
| 11 | Lights | Iluminación del mapa; cambio de estado al avanzar el jefe | P2 |
| 11 | **Sequencer** | **Los 4 reveals de stage del jefe.** Encaje directo | P2 |
| 11 | **Chaos Destruction** | **Paredes destructibles que modifican el grafo de espacios.** Ver abajo | P2 |
| 11 | **Landscape** | **La sección de superficie del mapa**, antes de descender. Ver abajo | P2 |

### Inteligencia Artificial

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 9 | Navegación y Pathfinding | Movimiento de enemigos entre espacios sobre NavMesh | P2 |
| 9 | **Percepción** | **El skill Stealth**: "1 enemigo no te sigue". Encaje directo | P2 |
| 10 | Behaviour Tree | Decisión del enemigo en su activación: seguir, atacar, reposicionar | P2 |
| 10 | **EQS** | **"El enemigo más cercano por camino más corto"** y elección de espacio destino | P2 |

### Optimización y Publicación

| Clase | Tema | Dónde vive en el juego | Fase |
|---|---|---|---|
| 13 | Profiler | Medición sobre el mapa completo con enemigos | P2 |
| 13 | Assets Streaming | Carga y descarga de sublevels de sala | P2 |
| 13 | Optimización | Presupuestos de performance | P2 |
| 13 | Packaging | Build jugable entregable | P2 |
| 14 | C++ | Fuera del alcance de las entregas. Opcional | — |

## Los dos temas que no tenían lugar natural

Como se evalúa la demostración de los temas, estos dos necesitan un destino diseñado y no
un agregado al final. Los dos tienen una versión que es mecánica en vez de decorativa.

### Chaos Destruction -> paredes destructibles que modifican el grafo

El mapa es un grafo de espacios, y el manual ya trae el concepto de pasajes bloqueados que
afectan el cálculo del camino más corto (página 20). Si una pared se puede romper, aparece
una arista nueva y cambian de golpe todas las distancias, las rutas de huida y quién es "el
enemigo más cercano".

Eso hace que la destrucción sea **una mecánica**, no un efecto. Quién puede romper qué
queda por decidir — candidatos: el jefe al cambiar de stage, un monstruo pesado, o una de
las 2 acciones propias de la misión.

### Landscape -> la sección de superficie del mapa

El mapa tiene una parte exterior donde arranca la partida y una subterránea donde está el
objetivo. Landscape arriba, meshes abajo. No es forzado: los episodios de DMD incluyen
mapas al aire libre, y **las dos temáticas candidatas son sobre descender**.

Beneficio lateral: le da trabajo real a Lights (clase 11) — luz de día arriba, oscuridad
abajo, y la iluminación del mapa entero cambiando a medida que el jefe avanza en su track.

## Trabajo en paralelo: los assets de Unreal son binarios

El problema técnico del trabajo grupal en Unreal, y no tiene nada que ver con el diseño.
`.uasset` y `.umap` son binarios: si dos personas tocan el mismo Blueprint en ramas
distintas, git no puede mergear — **se pierde el trabajo de una de las dos**. No hay
resolución de conflicto posible, hay que elegir un archivo y descartar el otro.

Tres defensas, en orden de importancia:

1. **Una sala por sublevel.** Si el mapa es un solo `.umap`, una sola persona puede tocar
   el nivel a la vez. Con sublevels, cada uno trabaja en el suyo. Los SubLevels (clase 5)
   dejan de ser un tema del temario y pasan a ser la condición para trabajar en paralelo.
2. **Muchos Blueprints chicos en vez de pocos grandes.** No es solo buena práctica: baja la
   probabilidad de que dos personas necesiten el mismo archivo el mismo día.
3. **Git LFS locking.** Marcar `*.uasset` y `*.umap` como `lockable` en `.gitattributes`
   deja los archivos en solo-lectura hasta que alguien los bloquea explícitamente. Es
   fricción, pero avisa antes en vez de después. Cae en Source Control, que es clase 1.

Además hay que definir **ownership por área** — quién es dueño de qué Blueprints — para
que la defensa 2 funcione de verdad.

## Plan de dos fases

### Fase 1 — hasta el 24/9 (36 días)

El loop completo jugable, con reglas en Blueprint y enemigos scripteados. **Los enemigos de
DMD no necesitan IA para las reglas base**: te siguen al salir de un espacio y te atacan si
comparten espacio. Eso es scripting legítimo, no una simplificación tramposa, y deja a las
clases 9-10 el trabajo real de reemplazarlo.

Alcance sugerido, más chico que la misión completa:

- Grafo de espacios con 4-6 salas como sublevels
- 2 personajes con posesión y alternancia (no los 4 todavía)
- 1 tipo de enemigo, spawn en 1 puerta
- Dados físicos con las 4 caras y la tirada completa
- Las 3 barras y los umbrales de trinquete con subida de skill
- La secuencia de turno completa: 3 acciones, carta de presión, referéndum, fin de turno
- HUD en UMG y sonidos
- Condición de victoria y de derrota

### Fase 2 — hasta el 12/11 (49 días más)

- Reemplazar el scripting de enemigos por NavMesh + Behaviour Tree + EQS + Percepción
- Extraer las reglas a Data Tables y Data Assets, con Gameplay Tags como vocabulario
- Los 4 personajes, el jefe con sus 4 stages y Sequencer en los reveals
- Save Game
- Landscape, Lights, Chaos Destruction
- Profiler, streaming de sublevels y packaging

## Riesgos

**El más grande: 36 días, y la fase 1 pide UMG pesado.** El HUD de este juego no es un HUD:
son 3 barras por personaje, el track del jefe, la pila de descarte visible, las cartas, los
dados y los skills. Es el ítem que más se subestima y cae justo antes del 1er parcial.

**Un merge perdido cuesta más que una semana de diseño.** Si el equipo empieza a trabajar
antes de tener los sublevels y el ownership definidos, la primera colisión sobre un
Blueprint grande borra trabajo real. Es el riesgo más barato de evitar y el más caro de
descubrir tarde.

**La pila de descarte tiene que ser visible.** En la mesa es información pública gratis; en
pantalla, si no se construye, se pierde el mecanismo de presión entero. Es UMG de fase 1.

**El alcance del juego excede el de la materia.** 1 misión + 1 boss + 4 personajes + 24
efectos de skill + 31 cartas es el juego; la materia termina el 17/12. La materia es un
hito adentro del juego, no el juego.

## Pendiente de confirmar

- **Tamaño del equipo y qué sabe hacer cada uno.** Define el alcance real de la fase 1 y el
  ownership por área. Es lo único que bloquea planificar en serio.
- ¿El repo es compartido por el grupo o cada uno trabaja aparte y se integra? Cambia la
  urgencia del LFS locking.
