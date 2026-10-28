# Project_C — El juego y cómo está hecho

Project_C es un juego de tablero táctico por turnos para un jugador, inspirado en *Cthulhu: Death May Die*. Controlás un grupo de 4 héroes que se mueven por un tablero de casillas y pelean contra enemigos. Cada decisión cuenta: hay pocas acciones por turno, poca vida y el enemigo castiga a quien se queda mal parado.

Está hecho en **Unreal Engine 5.8**, casi todo en Blueprints. Hay un módulo C++ chico (`ProjectC`) que resuelve el grafo del tablero, porque es la parte con más cálculo: distancias y caminos.

---

## 1. Mecánicas y jugabilidad

### El tablero
El mapa no es una grilla libre: es un **grafo de casillas**. Cada casilla conoce a sus vecinas y también hacia qué lados el paso está bloqueado, por ejemplo por una pared. Moverse "una casilla" significa ir a una vecina. La distancia entre dos casillas es la cantidad de saltos entre vecinas, no la distancia en el mundo.

Varias figuras pueden compartir la misma casilla. Cuando pasa, se acomodan en fila dentro de ella para que se vean todas y se puedan clickear.

### Rondas y turnos (estilo XCOM)
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
| **Atacar** | Seleccionar un héroe → botón **ATACAR** → click en un enemigo | 1 acción |
| **Terminar turno** | Tecla **Espacio** | — |
| **Cancelar** | Input de cancelar: suelta la selección | — |

Seleccionar un héroe no muestra nada en el tablero. Las casillas se iluminan recién al tocar **MOVERSE**, así no se confunde "estoy mirando a este héroe" con "voy a moverlo". Un click en una casilla sin haber tocado Moverse no hace nada y no te hace perder la selección.

### Vida, daño y fin de partida
- Los héroes tienen **2** de vida y los enemigos **3**. Cada ataque resta el daño del atacante.
- Una figura que llega a 0 muere: sale del tablero y ya no puede actuar ni ser elegida. Un héroe muerto no cuenta para cerrar la ronda, así que la ronda no se traba esperándolo.
- **Victoria:** no queda ningún enemigo vivo.
- **Derrota:** se pierden los héroes.

### HUD
La interfaz muestra todo lo necesario para decidir sin tener que recordar nada:
- abajo a la izquierda: **ronda** actual y **cuántos héroes faltan jugar**, **héroe activo** y **acciones** que le quedan, **figura seleccionada** y si estás apuntando, **vida** del héroe activo y **vida del enemigo** que tenés bajo el mouse.
- abajo a la derecha: los botones **MOVERSE** y **ATACAR**.
- al centro: un cartel cuando una acción no se puede hacer, por ejemplo "Fuera de alcance" o "No quedan acciones este turno".

---

## 2. Cómo está resuelto en Unreal

### Reparto de responsabilidades (Gameplay Framework)
Cada clase del framework de Unreal tiene un rol claro. Las reglas no están mezcladas con el input ni con la interfaz.

| Clase | Qué hace |
|---|---|
| `BP_GameMode` | **Las reglas.** Decide si una figura puede actuar, gasta acciones, valida y ejecuta movimientos y ataques, resuelve el Reckoning, cierra turnos y rondas y detecta victoria o derrota. Solo existe en el "servidor" de la partida. |
| `BP_GameState` | **El estado compartido de la partida.** `Round`, `Party` (los héroes registrados), `Retired` (los que ya jugaron esta ronda) y `ActiveFigure`. Cuando cambia la figura activa, avisa con el dispatcher `OnActiveFigureChanged`. |
| `BP_PlayerState` | **Estadísticas del jugador:** enemigos eliminados, personajes perdidos y casillas recorridas. |
| `BP_PlayerController` | **El input y la selección.** Detecta qué hay bajo el mouse, maneja la selección, los resaltados y los modos Moverse y Apuntar, y le pide al GameMode que ejecute las órdenes. No aplica reglas: pregunta. |
| `BP_CameraPawn` | La cámara de tablero: paneo con teclado, zoom con la rueda del mouse y órbita. Usa un Spring Arm con límites de distancia y suavizado. |
| `BP_HUD_Level` + `WBP_GameHUD` | El HUD en UMG. Se actualiza cada frame leyendo el GameMode, el GameState y el PlayerController, así siempre muestra el estado real sin tener que avisarle de cada cambio. |
| `BP_GameInstance` | Reservado para datos que tengan que sobrevivir entre niveles. |

### Figuras: herencia y una interfaz común
Todas las figuras salen de una misma base:

- **`BP_Character`** tiene lo común: vida, daño de ataque, la casilla en la que está, cómo se resalta y qué pasa al morir.
  - **`BP_Player`**: los héroes. Al aparecer se registra en el GameState para entrar en las rondas. Cada héroe concreto (`BP_Wraith`, `BP_Bulwark`, `BP_Hunter`, `BP_Ram`) hereda de esta clase y cambia sus valores y su modelo. Lleva el componente `BPC_Ratchet`, un track de desgaste del personaje.
  - **`BP_Enemy`**: los enemigos. Heredan todo de `BP_Character`, pero no se pueden seleccionar.

**`BPI_Selectable`** es la interfaz que implementan las figuras y las casillas. Define dos funciones: `CanBeSelected` (¿se puede elegir?) y `SetHighlight` (cómo resaltarse). Gracias a la interfaz, el PlayerController resalta y selecciona cualquier cosa del tablero sin saber si es un héroe, un enemigo o una casilla, y cada clase decide su propia respuesta. Un héroe se puede elegir si está vivo; un enemigo nunca.

### El tablero: casillas y un grafo en C++
- **`BP_Space`** es cada casilla. Tiene sus vecinas (`Neighbours`), los lados bloqueados (`Blocked Towards`) y la lista de figuras paradas encima (`Occupants`). Con `Slot Spacing` reparte a los ocupantes en fila dentro de la casilla.
- **`UGraphSubsystem`** (C++, un *World Subsystem*) guarda el grafo completo. Cada casilla se registra en su BeginPlay y, al empezar la partida, el GameMode lo cierra con `SealGraph`, que valida que las conexiones sean coherentes. Después responde las preguntas del juego:
  - `GetReachable(casilla, N)`: casillas a N pasos o menos. De acá salen las casillas que se iluminan al moverse, usando `SpacesPerMove` del GameMode.
  - `GetShortestPath`, `GetDistance` y `GetNearest`: caminos y distancias entre casillas.

  Está en C++ porque recorrer el grafo es la parte de cálculo pesado. En Blueprints sería lento y difícil de leer.
- **Cómo una figura cambia de casilla:** al empezar, cada figura busca la casilla que tiene debajo (`FindStartSpace`). Para moverse se usa `SetSpace`, que la saca de la casilla vieja, la agrega a la nueva y reacomoda a los que quedan en las dos.

### Input y selección con el mouse
- **Enhanced Input** con dos contextos: `IMC_Game` para seleccionar, cancelar y terminar turno, y `IMC_Camera` para mover la cámara.
- **Canales de colisión propios:** `Space` (casillas), `Figure` (personajes) y `Selectable`. Cada figura tiene una esfera de selección de tipo Figure y cada casilla un piso de tipo Space.
- **Dos traces bajo el cursor:** el PlayerController primero busca figuras y, si no corresponde elegir una, busca casillas. Así, cuando ya tenés un héroe seleccionado y querés moverlo a una casilla donde hay otra figura, el click llega a la casilla y no queda tapado por el personaje.
- **Recorrido de un click:** `HandleSelect` → trace → `ClickActor`. Según lo que tocaste:
  - si es una casilla y estás en modo Moverse, va a `TryGiveMoveOrder`;
  - si es un enemigo y estás apuntando, va a `TryGiveAttackOrder`;
  - si no, a `TrySelect`.

  El GameMode valida cada orden y, si no se puede, el HUD muestra el motivo.
- **Resaltados:** cada frame el Tick revisa qué hay bajo el mouse y, si cambió, `RefreshHighlights` actualiza los colores: lo que tenés encima, lo seleccionado y las casillas alcanzables. Antes de resaltar comprueba que el actor siga existiendo, por si un enemigo acaba de morir.

### Comunicación entre Blueprints
Se usan las tres formas que enseña la cursada, cada una donde corresponde:
- **Interfaz** (`BPI_Selectable`): para tratar distinto a figuras y casillas sin casts.
- **Event Dispatchers**: para avisar sin conocer al que escucha.
  - `OnActiveFigureChanged`: el GameState avisa que cambió el héroe activo.
  - `OnGameEnded`: el GameMode avisa al HUD que terminó la partida y muestra la pantalla final.
  - `OnCharacterLost`: el Ratchet le avisa a su héroe.

  Cada suscripción se hace al aparecer y se deshace al desaparecer.
- **Referencias tipadas**: los casts a `BP_GameMode`, `BP_GameState` y `BP_Character` están encapsulados en getters (`GetGameMode`, `GetGameState`). El resto del código no repite el cast.

### El nivel
`Levels/Level` es el mapa con el que arrancan el editor y el juego. Tiene las casillas ya conectadas con sus vecinas, los 4 héroes en la casilla de inicio, los enemigos y el PlayerStart donde aparece la cámara. El PlayerStart está elevado para que la cámara no arranque dentro de los héroes.
