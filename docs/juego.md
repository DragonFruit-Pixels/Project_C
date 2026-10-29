# Project_C — El juego y cómo está hecho

Project_C es un juego de tablero táctico por turnos para un jugador, inspirado en *Cthulhu: Death May Die*. Controlás un grupo de 4 héroes que se mueven por un tablero de casillas y pelean contra enemigos. Cada decisión cuenta: hay pocas acciones por turno, poca vida y el enemigo castiga a quien se queda mal parado.

Está hecho en **Unreal Engine 5.8**, casi todo en Blueprints. Hay un módulo C++ chico (`ProjectC`) que resuelve el grafo del tablero, porque es la parte con más cálculo: distancias y caminos.

---

## 1. Mecánicas y jugabilidad

### El tablero
El mapa no es una grilla libre: es un **grafo de casillas**. Cada casilla conoce a sus vecinas y también hacia qué lados el paso está bloqueado, por ejemplo por una pared. Moverse "una casilla" significa ir a una vecina. La distancia entre dos casillas es la cantidad de saltos entre vecinas, no la distancia en el mundo.

Varias figuras pueden compartir la misma casilla. Cuando pasa, se acomodan en fila dentro de ella para que se vean todas y se puedan clickear.

### Rondas y turnos
La partida avanza por **rondas**. En cada ronda, los 4 héroes juegan una vez cada uno.

- **El orden lo elegís vos.** Al empezar un turno elegís cualquier héroe que todavía no haya jugado en la ronda. No hay un orden fijo como en el juego de mesa. Ese orden ("sentido horario") tiene sentido en una mesa con varios jugadores, pero en un juego de un jugador no aporta nada.
- **Cada turno tiene 3 acciones.** El turno termina cuando se gastan o cuando pasás con **Espacio**.
- **Reckoning:** al terminar cada turno, los enemigos que están en la misma casilla que el héroe que acaba de jugar lo atacan. Quedarse al lado de un enemigo tiene costo.
- Cuando jugaron los 4, el contador pasa a la ronda siguiente y todos vuelven a estar disponibles. La partida arranca en la **Ronda 1**.

Que el orden sea libre podría hacer el juego demasiado fácil. Lo que lo equilibra es que en cada turno pasan cosas que no podés anticipar: elegir quién juega ahora es elegir quién se expone.

### Acciones
| Acción | Cómo se hace | Costo |
|---|---|---|
| **Moverse** | Seleccionar un héroe → botón **MOVERSE** → se iluminan las casillas alcanzables → click en una | 1 acción |
| **Atacar** | Seleccionar un héroe → botón **ATACAR** → click en un enemigo que esté en la misma casilla | 1 acción |
| **Terminar turno** | Tecla **Espacio** | — |
| **Cancelar** | Input de cancelar: suelta la selección | — |

Seleccionar un héroe no muestra nada en el tablero. Las casillas se iluminan recién al tocar **MOVERSE**, así no se confunde "estoy mirando a este héroe" con "voy a moverlo". Un click en una casilla sin haber tocado Moverse no hace nada y no te hace perder la selección.

### Vida, daño y fin de partida
- Los héroes tienen **2** de vida y los enemigos **3**. Cada ataque resta el daño del atacante (1 en héroes y enemigos).
- El enemigo no devuelve el golpe cuando lo atacás: solo hace daño en el Reckoning, al terminar el turno.
- Una figura que llega a 0 muere: sale del tablero y ya no puede actuar ni ser elegida.
- **Victoria:** no queda ningún enemigo vivo.
- **Derrota:** muere cualquiera de los héroes.

### HUD
La interfaz muestra todo lo necesario para decidir sin tener que recordar nada:
- abajo a la izquierda, un panel en tres bloques:
  - **partida:** ronda actual y cuántos héroes faltan jugar;
  - **figura activa:** héroe activo, acciones que le quedan, figura seleccionada y si estás apuntando, y su **vida** en verde. La vida sigue a la vista después de terminar el turno, así ves el daño del Reckoning;
  - **objetivo:** la **vida del enemigo** que tenés bajo el mouse, en rojo.
- abajo a la derecha: los botones **MOVERSE** y **ATACAR**.
- al centro: un cartel cuando una acción no se puede hacer, por ejemplo "Fuera de alcance" o "No quedan acciones este turno".

---

## 2. Cómo está resuelto en Unreal

Los nombres en `código` son los Blueprints, las funciones y los nodos reales del proyecto. Se pueden buscar en el editor tal cual.

### Reparto de responsabilidades (Gameplay Framework)
Cada clase del framework de Unreal tiene un rol claro. Las reglas no están mezcladas con el input ni con la interfaz.

| Clase | Qué hace | Funciones principales |
|---|---|---|
| `BP_GameMode` | **Las reglas.** Decide si una figura puede actuar, gasta acciones, valida y ejecuta movimientos y ataques, resuelve el Reckoning, cierra turnos y rondas y detecta victoria o derrota. | `TryActivateFigure`, `SpendAction`, `TryMoveFigure`, `TryAttack`, `EndTurn`, `ResolveReckoning`, `AdvanceTurn`, `NotifyDeath`, `CheckVictory`, `EndGame` |
| `BP_GameState` | **El estado compartido de la partida.** `Round`, `Party` (los héroes registrados), `Retired` (los que ya jugaron esta ronda), `ActiveFigure` y el estado de la partida (`E_GameState`: en juego, victoria o derrota). | `RegisterFigure`, `SetActiveFigure`, `CanActThisRound`, `GetFiguresLeftThisRound`, `RetireActiveFigure`, `BeginRound` |
| `BP_PlayerState` | **Estadísticas del jugador:** `EnemiesKilled`, `CharactersLost` y casillas recorridas. Las escribe el GameMode. | — |
| `BP_PlayerController` | **El input y la selección.** Detecta qué hay bajo el mouse, maneja la selección, los resaltados y los modos Moverse y Apuntar, y le pide al GameMode que ejecute las órdenes. No aplica reglas: pregunta. | `HandleSelect`, `TraceSelectableUnderCursor`, `ClickActor`, `SelectActor`, `TryGiveMoveOrder`, `TryGiveAttackOrder`, `BeginMoving`, `BeginAiming`, `HandleEndTurn`, `RefreshHighlights` |
| `BP_CameraPawn` | La cámara de tablero: paneo con teclado, zoom con la rueda del mouse y órbita. Usa un Spring Arm con límites de distancia y suavizado. | `HandleOrbit` |
| `BP_HUD_Level` + `WBP_GameHUD` | El HUD en UMG. `BP_HUD_Level` crea el widget en su BeginPlay (`Create Widget` → `Add to Viewport`). El widget se actualiza cada frame leyendo el GameMode, el GameState y el PlayerController. | `Refresh`, `UpdateRound`, `UpdateTurn`, `UpdateActions`, `UpdateSelected`, `UpdateHealth`, `UpdateTarget`, `ShowEnding` |
| `BP_GameInstance` | Reservado para datos que tengan que sobrevivir entre niveles. | — |

### Cómo arranca la partida
1. Cada `BP_Space`, en su **BeginPlay**, se registra en el grafo con `RegisterSpace`, pasando sus `Neighbours` y `Blocked Towards`.
2. Cada figura (`BP_Character`), en su **BeginPlay**, busca la casilla que tiene debajo con `FindStartSpace`, arranca con la vida al máximo (`Health = MaxHealth`) y llama a `OnSpawned`.
3. En `BP_Player`, el evento `OnSpawned` llama a `RegisterFigure` del GameState: así el héroe entra en `Party` y participa de las rondas. Al desaparecer, `OnDespawned` llama a `UnregisterFigure`.
4. El **BeginPlay** de `BP_GameMode` cierra el grafo con `SealGraph` y llama a `EndTurn`. Como todavía no hay figura activa, eso solo deja `ActionsRemaining` en `ActionsPerTurn` (3).
5. El **BeginPlay** de `BP_PlayerController` agrega el contexto de input (`Add Mapping Context`) y se suscribe al GameState con `BindToTurnChanges`.
6. El HUD, en su **Construct**, guarda referencias tipadas al GameMode, al GameState y al PlayerController con `ResolveRefs` y se suscribe a `OnGameEnded`.

### Una ronda de principio a fin
Este es el recorrido completo desde que apretás Play hasta que empieza la segunda ronda, con 4 héroes: A, B, C y D. La tabla muestra cómo cambia el estado de la partida en cada paso.

| Momento | `Round` | `Retired` | `ActiveFigure` | `ActionsRemaining` | HUD: "quedan" |
|---|---|---|---|---|---|
| Después de Play | 0 | vacío | nadie | 3 | 4 |
| Elegís a A | 1 | vacío | A | 3 | 4 |
| A gasta sus acciones | 1 | vacío | A | 3 → 2 → 1 → 0 | 4 |
| Termina el turno de A | 1 | A | nadie | 3 | 3 |
| Juegan B y C | 1 | A, B, C | nadie | 3 | 1 |
| Termina el turno de D | **2** | vacío | nadie | 3 | 4 |

**1. Play.** Se ejecuta lo de "Cómo arranca la partida": las casillas arman el grafo, cada figura se ubica en su casilla y los 4 héroes entran en `Party`. El estado de la partida (`E_GameState`) queda en "en juego". Todavía no hay ronda: `Round` vale 0.

**2. Elegís un héroe (A).** Click → `HandleSelect` → `ClickActor` → `TrySelect` → `SelectActor` → `TryActivateFigure` (GameMode):
- Como `Round` es 0, llama a `BeginRound`, que lo pasa a **1**. Esto pasa una sola vez, con la primera figura de la partida.
- `SetActiveFigure` (GameState) acepta a A porque está en `Party`, no está en `Retired` y no hay otro héroe activo. Dispara `OnActiveFigureChanged`, y el PlayerController recalcula destinos y resaltados.

**3. A usa sus 3 acciones.** Cada acción pasa por `SpendAction`, que resta 1 a `ActionsRemaining`.
- **Moverse:** `TryMoveFigure` → `SetSpace`. En el mapa, A sale de su casilla (`RemoveOccupant`) y entra en la nueva (`AddOccupant`). Las dos casillas reacomodan en fila a los que quedan (`RefreshOccupantPlacement`). `AddSpacesMoved` suma las casillas recorridas en el PlayerState.
- **Atacar:** `TryAttack` → `ApplyDamage` en el enemigo. Si muere, puede terminar la partida (paso 7).
- Mientras A es la figura activa, ningún otro héroe se puede elegir: aparece "Termina el turno de A primero (Espacio)".

**4. Termina el turno de A.** Pasa solo cuando `ActionsRemaining` llega a 0 (`EndTurnIfOutOfActions`) o cuando apretás Espacio (`HandleEndTurn`). En los dos casos se ejecuta `EndTurn`:
1. `ResolveReckoning`: los enemigos vivos que están en la casilla de A le pegan. Si A muere, es DERROTA y la ronda se corta acá.
2. `AdvanceTurn` → `RetireActiveFigure`: A pasa a `Retired` y `ActiveFigure` queda vacía. `GetFiguresLeftThisRound` devuelve 3 (B, C y D), así que no empieza una ronda nueva.
3. `ActionsRemaining` vuelve a 3 para el próximo héroe.

A queda parado donde terminó. Si intentás elegirlo de nuevo, `CanActThisRound` da falso porque está en `Retired`, y aparece "Esa figura no puede actuar esta ronda".

**5. Juegan B, C y D.** Cada uno repite los pasos 2 a 4, en el orden que vos elijas. El contador "quedan" del HUD baja de a uno.

**6. Termina el turno de D: empieza la ronda 2.** En `AdvanceTurn`, `GetFiguresLeftThisRound` devuelve una lista vacía. Entonces llama a `BeginRound`, que vacía `Retired` y suma 1 a `Round`. Los 4 héroes vuelven a estar disponibles, cada uno donde quedó en el mapa.

Los enemigos no tienen turno propio: no se mueven y solo hacen daño en el Reckoning del paso 4.

**7. Cómo se corta una ronda.** En cualquier momento, si muere un héroe, `NotifyDeath` llama a `EndGame(false)` (DERROTA). Si muere el último enemigo, `CheckVictory` llama a `EndGame(true)` (VICTORIA). `EndGame` cambia `E_GameState`, y desde ese momento `SpendAction` y `EndTurn` ya no hacen nada. `OnGameEnded` le avisa al HUD, que muestra la pantalla final.

### Figuras: herencia y una interfaz común
Todas las figuras salen de una misma base:

- **`BP_Character`** tiene lo común: vida (`Health`, `MaxHealth`), daño (`AttackDamage`), la casilla en la que está (`CurrentSpace`), cómo se resalta (`ApplyHighlight`) y qué pasa al morir (`Die`).
  - **`BP_Player`**: los héroes. Al aparecer se registran en el GameState (`OnSpawned` → `RegisterFigure`). Cada héroe concreto (`BP_Wraith`, `BP_Bulwark`, `BP_Hunter`, `BP_Ram`) hereda de esta clase y cambia sus valores y su modelo.
  - **`BP_Enemy`**: los enemigos. Sobrescriben `IsHostile` para devolver `true`, `GetBaseColour` para tener otro color y `CanBeSelected` para devolver `false`.

`IsHostile` es la forma en que todo el juego distingue bandos: el ataque, el Reckoning y la muerte preguntan `IsHostile` en vez de castear a `BP_Enemy`.

**`BPI_Selectable`** es la interfaz que implementan las figuras y las casillas. Define dos funciones:
- `CanBeSelected`: ¿se puede elegir?
- `SetHighlight`: cómo resaltarse, según el enum `E_SelectionHighlight` (nada, destino posible, bajo el mouse, seleccionado).

Gracias a la interfaz, el PlayerController resalta y selecciona cualquier cosa del tablero sin saber si es un héroe, un enemigo o una casilla, y cada clase decide su propia respuesta. Un héroe se puede elegir si está vivo; un enemigo nunca. En `BP_Character` y en `BP_Space`, el evento `SetHighlight` llama a su propio `ApplyHighlight`.

### El tablero: casillas y un grafo en C++
- **`BP_Space`** es cada casilla. Tiene sus vecinas (`Neighbours`), los lados bloqueados (`Blocked Towards`) y la lista de figuras paradas encima (`Occupants`). Con `AddOccupant` y `RemoveOccupant` cambia esa lista. `RefreshOccupantPlacement` los reparte en fila usando `GetSlotLocation` y `Slot Spacing`.
- **`UGraphSubsystem`** (C++, un *World Subsystem*, en `Source/ProjectC/Public/Map/GraphSubsystem.h`) guarda el grafo completo. Las casillas se registran con `RegisterSpace`. `SealGraph` valida que las conexiones sean coherentes. Después responde las preguntas del juego:
  - `GetReachable(casilla, N)`: casillas a N pasos o menos. La usa `GetLegalDestinations` del GameMode, con `SpacesPerMove`, para saber a dónde se puede mover el héroe activo.
  - `GetShortestPath`, `GetDistance` y `GetNearest`: caminos y distancias entre casillas.

  Está en C++ porque recorrer el grafo es la parte de cálculo pesado. En Blueprints sería lento y difícil de leer.
- **Cómo una figura cambia de casilla:** `SetSpace` (en `BP_Character`) hace `RemoveOccupant` en la casilla vieja, actualiza `CurrentSpace`, hace `AddOccupant` en la nueva y llama a `RefreshOccupantPlacement` en las dos.

### Input y selección con el mouse
- **Enhanced Input** con dos contextos: `IMC_Game` (`IA_Select`, `IA_Cancel`, `IA_EndTurn`) y `IMC_Camera` (paneo, zoom y `IA_CameraOrbit`).
- **Canales de colisión propios:** `Space` (casillas), `Figure` (personajes) y `Selectable`. Cada figura tiene una esfera de selección de tipo Figure y cada casilla un piso de tipo Space.
- **Dos traces bajo el cursor (`TraceSelectableUnderCursor`):** convierte la posición del mouse a un rayo en el mundo (`Get Mouse Position` → `Deproject Screen to World`). Después hace `Line Trace For Objects` contra figuras. Si no hay nada seleccionado o la figura no se puede elegir, devuelve esa figura. Si ya hay algo seleccionado, hace un segundo trace contra casillas. Así, cuando querés mover un héroe a una casilla donde hay otra figura, el click llega a la casilla y no queda tapado por el personaje.
- **Recorrido de un click:** `IA_Select` → `HandleSelect` → `TraceSelectableUnderCursor` → `ClickActor`. `ClickActor` decide con un `Cast To BP_Space`:
  - si es una casilla, apaga el modo Apuntar y prueba `TryGiveMoveOrder`; si no corresponde mover, `TrySelect`;
  - si no es casilla y `Aiming` está activo, prueba `TryGiveAttackOrder`;
  - si no, `TrySelect`, que pregunta `CanBeSelected` por la interfaz y llama a `SelectActor`;
  - si no tocaste nada, limpia la selección.
- **Resaltados:** el **Tick** del PlayerController hace el trace y, solo si cambió lo que hay bajo el mouse (`HoveredActor`), llama a `RefreshHighlights`. Esa función apaga lo que estaba resaltado y pinta, en orden, los destinos posibles (`LegalDestinations`), lo que está bajo el mouse y lo seleccionado. Antes de pintar pasa por `IsValid`, por si un enemigo acaba de morir.

### Recorrido de cada acción
Todas siguen el mismo patrón: el PlayerController **pide** y el GameMode **valida y ejecuta**. Si el GameMode dice que no, el PlayerController llama a `ShowRefusal` y el HUD muestra el motivo.

**Elegir un héroe**
`SelectActor` (PlayerController) → `TryActivateFigure` (GameMode) → `SetActiveFigure` (GameState). El GameState solo acepta si no hay otro héroe activo y si `CanActThisRound` da verdadero: el héroe está en `Party`, no está en `Retired` y `CanBeSelected` da verdadero. Si acepta, dispara `OnActiveFigureChanged`. Si no, aparece "Termina el turno de X primero (Espacio)". En el primer turno, `TryActivateFigure` llama a `BeginRound` para arrancar la Ronda 1.

**Moverse**
1. Botón **MOVERSE** (`OnClicked(BtnMove)` en el HUD) → `BeginMoving`: activa `Moving` y llama a `RefreshLegalDestinations`, que le pide al GameMode `GetLegalDestinations`.
2. Click en una casilla → `TryGiveMoveOrder`. Si la casilla no está en `LegalDestinations`, aparece "Fuera de alcance".
3. `TryMoveFigure` (GameMode) → `CanMoveFigure` → `SpendAction` → `AddSpacesMoved` (estadística) → `SetSpace` → `EndTurnIfOutOfActions`.

**Atacar**
1. Botón **ATACAR** (`OnClicked(BtnAttack)`) → `BeginAiming`: activa `Aiming`.
2. Click en un enemigo → `TryGiveAttackOrder`: castea a `BP_Character` y verifica `IsHostile`.
3. `TryAttack` (GameMode) verifica que el objetivo esté vivo, que sean de bandos distintos (`IsHostile` diferente) y que compartan casilla (`GetCurrentSpace` igual). Después: `SpendAction` → `ApplyDamage` en el objetivo con el `AttackDamage` del atacante → `EndTurnIfOutOfActions`.

**Terminar turno**
`IA_EndTurn` (Espacio) → `HandleEndTurn` → `EndTurn` (GameMode), o automáticamente desde `EndTurnIfOutOfActions` cuando `ActionsRemaining` llega a 0. `EndTurn` hace, en orden:
1. `ResolveReckoning`: toma la casilla del héroe activo, recorre sus `Occupants` con un **For Each** y cada ocupante con `IsHostile` y vida mayor a 0 le hace `ApplyDamage` con su `AttackDamage`.
2. `AdvanceTurn`: `RetireActiveFigure` pasa al héroe a `Retired` y libera `ActiveFigure`. Si `GetFiguresLeftThisRound` queda vacío, `BeginRound` vacía `Retired` y suma 1 a `Round`.
3. Repone `ActionsRemaining` con `ActionsPerTurn`.

`SpendAction` y `EndTurn` no hacen nada si la partida ya terminó (`E_GameState` distinto de "en juego").

### Daño, muerte y fin de partida
- `ApplyDamage` (en `BP_Character`) resta el daño con un `Max(0, Health - Amount)` para que la vida no quede negativa. Si llega a 0, llama a `Die`.
- `Die` le avisa al GameMode con `NotifyDeath` y después hace `Destroy Actor`. Al destruirse, el **EndPlay** de la figura la saca de su casilla (`SetSpace` con casilla vacía) y llama a `OnDespawned`.
- `NotifyDeath` (GameMode) pregunta `IsHostile`:
  - **enemigo:** suma 1 a `EnemiesKilled` en el PlayerState y llama a `CheckVictory`, que recorre las figuras con `Get All Actors Of Class` y, si no queda ningún hostil vivo, llama a `EndGame(true)`;
  - **héroe:** suma 1 a `CharactersLost` y llama a `EndGame(false)`.
- `EndGame` guarda victoria o derrota en `E_GameState` del GameState y dispara `OnGameEnded`. El HUD lo escucha y `ShowEnding` muestra **VICTORIA** o **DERROTA** con el panel en verde o en rojo.

### HUD
- El **Tick** de `WBP_GameHUD` llama a `Refresh` y a `UpdateRefusal`. `Refresh` llama a una función por bloque del panel: `UpdateRound`, `UpdateTurn`, `UpdateActions`, `UpdateSelected`, `UpdateHealth` y `UpdateTarget`.
- `UpdateHealth` guarda la última figura activa en `LastFigure` para seguir mostrando su vida cuando termina el turno. Antes de leerla pasa por `IsValid`, porque la figura puede haber muerto en el Reckoning.
- `UpdateTarget` castea a `BP_Enemy` lo que hay bajo el mouse (`HoveredActor` del PlayerController) y muestra su vida.
- Los botones llaman directo al PlayerController: `BtnMove` → `BeginMoving`, `BtnAttack` → `BeginAiming`.
- El cartel de "no se puede" lo pone `ShowRefusal` y lo apaga `UpdateRefusal` cuando `Get Game Time in Seconds` pasa de `RefusalUntil`.

### Comunicación entre Blueprints
Se usan las tres formas que enseña la cursada, cada una donde corresponde:
- **Interfaz** (`BPI_Selectable`): para tratar distinto a figuras y casillas sin casts. La usan `TrySelect`, `CanActThisRound`, `TraceSelectableUnderCursor` y `RefreshHighlights`.
- **Event Dispatchers**: para avisar sin conocer al que escucha.
  - `OnActiveFigureChanged` (GameState): lo dispara `SetActiveFigure`. Lo escucha el PlayerController (`BindToTurnChanges` → `HadleActiveFigureChanged`), que recalcula destinos y resaltados.
  - `OnGameEnded` (GameMode): lo dispara `EndGame`. Lo escucha `WBP_GameHUD`, que llama a `ShowEnding`.

  Cada suscripción se hace al aparecer y se deshace al desaparecer: `BindToTurnChanges` en el BeginPlay y `UnbindFromTurnChanges` en el EndPlay del PlayerController; `Bind` en el Construct y `Unbind All` en el Destruct del HUD.
- **Referencias tipadas**: los casts a `BP_GameMode`, `BP_GameState` y `BP_Character` están encapsulados en getters (`GetGameMode`, `GetGameState`) y en `ResolveRefs` del HUD. El resto del código no repite el cast.

### El nivel
`Levels/Level` es el mapa con el que arrancan el editor y el juego. Tiene las casillas ya conectadas con sus vecinas, los 4 héroes en la casilla de inicio, los enemigos y el PlayerStart donde aparece la cámara. El PlayerStart está elevado para que la cámara no arranque dentro de los héroes.
