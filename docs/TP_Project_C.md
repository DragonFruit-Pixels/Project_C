# Project_C

## 1. Título

**Project_C**: juego táctico por turnos para un jugador, hecho en Unreal Engine 5.8.

Grupo: Project_C. Integrantes: Sebastián Luser, Paola Vicente y Gaspar Orma.

## 2. Descripción

Project_C pone al jugador al mando de un equipo de cuatro operarios que bajan a una planta química tomada por un crecimiento biológico. Arriba está la planta y abajo el sumidero, donde el crecimiento engendra criaturas. El objetivo es limpiar el laboratorio antes de que las criaturas terminen con el equipo.

Es un juego de tablero llevado al videojuego. El mapa es un conjunto de salas conectadas por puertas, las figuras se mueven de sala en sala y cada ataque se resuelve con una tirada de dados. Hay pocas acciones por turno y poca vida, así que cada decisión pesa: quedarse en la misma sala que un enemigo al terminar el turno tiene costo.

### Contexto de diseño e inspiración

- **Cthulhu: Death May Die** (CMON, 2019) es la inspiración principal. De este juego de mesa cooperativo tomamos:
  - el tablero de salas;
  - la tirada de dados con caras de acierto y de costo;
  - el "Reckoning", el momento en que los enemigos castigan al final del turno;
  - la idea de un enemigo final que aparece a mitad de partida.
- **XCOM 2** (Firaxis Games, 2016) inspira el orden de turno. En cada ronda el jugador elige qué héroe activa, en lugar de seguir un orden fijo. En un juego de mesa el orden fijo ordena a varios jugadores en la mesa; en un juego de un solo jugador, elegir el orden es una decisión táctica más.
- **Gloomhaven** (Cephalofactory Games, 2017) es la referencia de juego de tablero táctico. Muestra cómo convertir en tensión una economía chica de acciones y un posicionamiento en espacios discretos.
- **Rules of Play** (Katie Salen y Eric Zimmerman, MIT Press, 2004) da el marco para describir el juego como sistema formal: reglas, metas y espacio. Este documento sigue esa descomposición.
- **The Art of Game Design: A Book of Lenses** (Jesse Schell, CRC Press, 2008) guió la revisión de la claridad del feedback y del equilibrio entre azar y control.

El ritmo se apoya en el riesgo. El jugador decide dónde termina cada héroe sabiendo que los enemigos solo castigan a quien comparte sala con ellos. El azar de los dados aporta tensión, pero el posicionamiento es lo que se controla.

## 3. Actores

### Héroes (controlados por el jugador)

| Héroe | Vida | Dados de ataque | Movimiento | Acciones por turno |
|---|---|---|---|---|
| Shiv | 4 | 3 | hasta 3 salas | 3 |
| Brace | 4 | 3 | hasta 3 salas | 3 |
| Spike | 4 | 3 | hasta 3 salas | 3 |
| Maul | 4 | 3 | hasta 3 salas | 3 |

Los cuatro comparten estadísticas base y se distinguen por su modelo, su arma y su sonido de ataque. Cada héroe tiene además un medidor de **Ratchet**: mide cuánto avanzó su mutación y sube con cada cara **Toll** de los dados.

### Enemigos (controlados por la IA)

| Enemigo | Vida | Dados de ataque | Cuándo aparece |
|---|---|---|---|
| **Broodling** (araña) | 3 | 3 | Tres están desde el inicio. Dos más salen de cápsulas en la ronda 3. |
| **Abomination** (jefe) | 9 | 4 | Sale del tanque central de la sala Batch en la ronda 5. |

El Broodling es una criatura de seis patas, no humanoide, con rig y animaciones propias. La Abomination es un coloso mutado de unos tres metros: el triple de vida y un dado más que un Broodling.

### Otros actores

- **Salas (losetas).** Cada sala es un actor que conoce a sus vecinas y a las figuras que tiene adentro. Cubre todo el piso de la habitación, así que se puede apuntar y hacer click en cualquier parte.
- **Cápsulas de spawn.** Marcan dónde va a aparecer una criatura y en qué ronda. Se abren cuando llega esa ronda.
- **Cámara de tablero.** Paneo, zoom y órbita sobre el mapa.

## 4. Mecánicas

### Turnos y rondas

- La partida avanza por **rondas**. En cada ronda, cada héroe vivo juega un turno.
- **El orden lo elige el jugador**, al estilo XCOM. Al empezar un turno se elige cualquier héroe que todavía no haya jugado en la ronda.
- Cada turno tiene **3 acciones**. El turno termina cuando se gastan o cuando el jugador pasa con **Espacio**.
- Cuando juegan todos los héroes, empieza la ronda siguiente y todos vuelven a estar disponibles.

### Acciones

| Acción | Cómo se hace | Costo |
|---|---|---|
| **Move** | Elegir el héroe, tocar MOVE y hacer click en una sala resaltada (hasta 3 salas de distancia) | 1 acción |
| **Attack** | Elegir el héroe, tocar ATTACK y hacer click en un enemigo de la misma sala | 1 acción |
| **End Turn** | Tecla Espacio o botón End Turn | — |

### La tirada de dados

Cada dado tiene seis caras:

| Caras | Resultado |
|---|---|
| 2 | Fallo |
| 2 | Acierto |
| 1 | Acierto + Toll |
| 1 | Toll |

- Cada **acierto** saca 1 de vida al objetivo.
- Cada **Toll** suma 1 al Ratchet del héroe que está en la pelea, ya sea que ataque o que lo ataquen. El poder tiene un precio: cuanto más se pelea, más avanza la mutación.

### El Reckoning

Al terminar el turno de un héroe, **cada enemigo que comparte sala con él lo ataca**, de a uno y cada uno con su propia tirada. Si comparte sala con dos enemigos, recibe dos tiradas.

### Comportamiento enemigo

- Un enemigo **solo ataca en el Reckoning** y solo al héroe que terminó el turno en su sala.
- Si el movimiento de un héroe **pasó por la sala de un enemigo**, ese enemigo lo sigue hasta su sala.
- Los enemigos no ven a distancia: reaccionan a lo que pasa en su sala, como las miniaturas de un juego de mesa.

## 5. Metas y obstáculos

### Meta

**Limpiar el laboratorio:** eliminar a todos los enemigos, incluida la Abomination. El objetivo aparece en el HUD.

### Obstáculos

- **Los enemigos y el Reckoning.** Cada turno que un héroe termina junto a un enemigo cuesta una o más tiradas en contra.
- **La persecución.** Cruzar una sala ocupada atrae al enemigo. Conviene planear el recorrido para no arrastrar criaturas.
- **El tiempo.** Las rondas suben la dificultad:

| Ronda | Qué pasa |
|---|---|
| 1 y 2 | Tres Broodlings esperan en el sumidero (Gallery, Cold y Cook). Es el momento de avanzar y posicionarse. |
| 3 | Se abren dos cápsulas en la planta (Decon y Office): dos Broodlings más aparecen detrás del equipo. |
| 5 | La Abomination emerge del tanque central de Batch, la sala del objetivo. |

- **Los recursos escasos.** Hay 3 acciones por turno y 4 de vida por héroe, y no hay forma de curarse. Cada movimiento compite con cada ataque.
- **El azar.** Los dados pueden fallar. La mitad de las caras no hace daño y dos de las seis suben el Ratchet.

## 6. Reglas

1. La partida empieza con los cuatro héroes en el Loading Dock y la Ronda 1.
2. En su turno, un héroe tiene 3 acciones. Puede repetir la misma acción.
3. **Move:** el destino tiene que estar a 3 salas o menos, contando saltos entre salas vecinas y no distancia en metros.
4. **Attack:** el objetivo tiene que ser un enemigo vivo en la misma sala que el atacante.
5. Un héroe que ya jugó en la ronda no puede volver a elegirse hasta la ronda siguiente.
6. Mientras un héroe está activo, no se puede elegir otro.
7. Al terminar el turno se resuelve el Reckoning de su sala. Si el héroe muere, los enemigos que faltaban no atacan.
8. Una figura con 0 de vida muere y sale del tablero.
9. Los enemigos que aparecen por ronda lo hacen al comenzar esa ronda, en su cápsula o en su punto de origen.
10. **Victoria:** no queda ningún enemigo vivo. **Derrota:** mueren los cuatro héroes.
11. El juego se puede pausar en cualquier momento, y desde la pausa o el final se puede reiniciar o salir.

## 7. Espacio

El espacio de juego es un **mapa único, "Laboratorio"**, con once salas conectadas como un **grafo**. Cada sala conoce a sus vecinas y moverse una casilla significa ir a una vecina. La lectura es vertical: se entra por la planta, arriba, y se baja al sumidero, abajo.

```
        c0        c1        c2        c3        c4        c5        c6
r0  [Lockers ][ Decon  ][======= Loading Dock (START) ======][Control ][ Office ]   PLANTA
r1                                                                     [  Lift  ]
r2  [ Batch Chamber    ][Cook lab][Cold st.][Vat room]                 [  Lift  ]   SUMIDERO
r3  [   (OBJETIVO)     ]          [========== Pipe Gallery ==========][  Lift  ]
```

| Sala | Conecta con | Rol |
|---|---|---|
| Loading Dock | Decon, Control | Inicio del equipo |
| Lockers | Decon | Callejón sin salida |
| Decon | Lockers, Dock | Cápsula de spawn (ronda 3) |
| Control | Dock, Office | Paso |
| Office | Control, Lift | Cápsula de spawn (ronda 3) |
| Lift | Office, Pipe Gallery | Única bajada al sumidero |
| Pipe Gallery | Lift, Cold, Vat | Pasillo largo del sumidero |
| Vat room | Gallery, Cold | Sala de cubas |
| Cold store | Gallery, Cook, Vat | Nudo del sumidero |
| Cook lab | Cold, Batch | Única entrada a la sala final |
| Batch Chamber | Cook | Sala del jefe: el tanque central |

**Decisiones de diseño del espacio:**

- **Un solo camino al objetivo.** Lift → Gallery → Cold → Cook → Batch. Concentra la tensión y hace que la persecución de los enemigos importe.
- **Las cápsulas de la ronda 3 están en la planta**, detrás del equipo: castigan al que se queda atrás.
- **Salas de distinto tamaño.** Los salones largos (Dock, Gallery, Lift) y la arena de Batch reparten a las figuras en todo su espacio. Las salas chicas obligan a amontonarse.

## 8. Implementación en Unreal

| Requisito | Cómo se resolvió |
|---|---|
| **Gameplay Framework** | `BP_GameMode` aplica las reglas (acciones, movimiento, ataque, Reckoning, rondas, spawns, victoria y derrota). `BP_GameState` guarda el estado compartido (ronda, héroes, héroe activo, estado de la partida). `BP_PlayerState` lleva las estadísticas. `BP_PlayerController` maneja el input y la selección, y pide; el GameMode valida. Además están `BP_CameraPawn` (cámara de tablero), `BP_HUD_Level` y `BP_GameInstance`. |
| **Enhanced Input** | Contextos `IMC_Game` (`IA_Select`, `IA_Cancel`, `IA_Confirm`, `IA_EndTurn`, `IA_Pause`) e `IMC_Camera` (`IA_CameraPan`, `IA_CameraZoom`, `IA_CameraOrbit`). |
| **IA con Behaviour Tree** | `BT_Enemy` es el árbol base y maneja el turno del enemigo. Cada tipo de enemigo aporta su subárbol de decisión (`BT_Decide_Broodling`), inyectado en el slot del Gameplay Tag `AI.Decide` con *Run Behavior Dynamic*. Sumar un enemigo nuevo es sumar un subárbol. |
| **Data Tables** | `DT_Units` define vida, acciones, movimiento, dados y Ratchet de cada unidad. El balance se ajusta sin tocar Blueprints. |
| **Animación** | `ABP_Character` con State Machine (idle, caminar, atacar, recibir daño, morir) para los héroes. El Broodling y la Abomination tienen sus propias animaciones de idle, caminar, ataque, daño y muerte. |
| **UI (UMG)** | `WBP_GameHUD`: ronda, héroes que faltan jugar, acciones restantes, vida de la figura activa y del enemigo apuntado, tirada de dados, confirmación de ataque, mensajes de "no se puede", pausa y pantalla final de VICTORY o DEFEAT. Además está el menú principal. |
| **Comunicación entre Blueprints** | La interfaz `BPI_Selectable` permite seleccionar y resaltar figuras y salas sin casts. Los Event Dispatchers `OnActiveFigureChanged` (GameState) y `OnGameEnded` (GameMode) se suscriben al aparecer y se desuscriben al desaparecer. Las referencias tipadas están encapsuladas en getters. |
| **Herencia y polimorfismo** | Todas las figuras salen de `BP_Character`. `BP_Player` agrupa a los cuatro héroes; `BP_Enemy` es la base abstracta de `BP_Broodling` y `BP_Boss`. `IsHostile` distingue bandos sin castear. |
| **C++** | `UGraphSubsystem` (World Subsystem) guarda el grafo de salas y resuelve las salas alcanzables, el camino más corto y las distancias. Es la parte de cálculo pesado y tiene tests automáticos. |
| **VFX y sonido** | Niagara `NS_Hit` y `NS_Death`. Sonidos de ataque por héroe, pasos, golpe, daño, muerte, Toll, interfaz, ambiente y música. |
| **Optimización** | Nanite en la escenografía, que concentra el 90 % de los triángulos. Una sola luz con sombra por sala: las luces de relleno no proyectan sombra y tienen radio acotado. Lumen para la iluminación global. |
| **Save Game** | Guardado y carga por ronda (aporte de Paola, rama `demo-pao`). |
| **Menú principal** | Inicio de partida y salida (aporte de Gaspar). |

## 9. Pulido audiovisual

- **Dados en pantalla** en cada tirada: se ven las caras de acierto, fallo y Toll antes de aplicar el daño.
- **Impacto de golpe:** un temblor corto de cámara, un *hit-stop* de una fracción de segundo y números de daño flotantes.
- **Momento de spawn:** la cápsula se abre con VFX y sonido, la cámara va hacia ella y aparece un cartel con la ronda.
- **Entrada del jefe:** una cinemática hecha con **Sequencer** cuando la Abomination emerge del tanque de Batch.

## 10. Referencias

- CMON. *Cthulhu: Death May Die* [juego de mesa]. 2019.
- Firaxis Games. *XCOM 2* [videojuego]. 2K Games, 2016.
- Cephalofactory Games. *Gloomhaven* [juego de mesa]. 2017.
- Salen, K. y Zimmerman, E. *Rules of Play: Game Design Fundamentals*. MIT Press, 2004.
- Schell, J. *The Art of Game Design: A Book of Lenses*. CRC Press, 2008.
- Epic Games. *Unreal Engine 5 Documentation*. https://dev.epicgames.com/documentation/en-us/unreal-engine
