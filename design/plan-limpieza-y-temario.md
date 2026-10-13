# Plan: limpieza del proyecto + cumplir el temario

**Fecha:** 2026-10-12 · **Alcance:** Clases 1, 3, 4, 5 y 7. **La Clase 6 (animaciones) queda afuera por ahora.**

Fuentes: los grafos de los 12 Blueprints leídos por el MCP y las clases en
`Downloads/OneDrive_1_10-12-2026` (texto y capturas).

## Regla que guía todo

Simplificar es **borrar lo duplicado y lo muerto**, no los mecanismos que evalúa la materia.
De cada cosa que se enseña en clase (GameState, dispatcher, interfaz, herencia, etc.) queda
**un ejemplo, y bien usado**.

---

## Parte A · Limpieza

Cada paso deja el juego andando. Se compila todo, se prueba en PIE y se hace un commit por paso.

> **Antes de A1 y A4:** guardar capturas de los datos de instancia del nivel (`Neighbours` de
> cada `BP_Space`, `CurrentSpace` de cada figura). El último reparent los borró.

### A1 · Turno: el GameMode decide, el GameState guarda
- [ ] Borrar del GameMode las 5 funciones que solo reenvían al GameState: `GetMissionState`,
      `GetActiveFigure`, `GetRound`, `GetFiguresLeftThisRound`, `IsFiguresTurn`.
- [ ] El HUD y el PlayerController leen el GameState directo (`Get Game State` + Cast).
- [ ] **`BP_GameState_Mission` se queda**: la Clase 3 lo define como "registro central: rondas".

### A2 · Dispatchers: de 9 a 3, con bind y unbind
- [ ] Borrar los 6 que no escucha nadie: `OnPhaseChanged`, `OnFigureMoved`, `OnRoundBegan`,
      `OnHoveredChanged`, `OnSelectionChanged`, `OnSpaceChanged`.
- [ ] Borrar el loop de `OnOrderRefused` (el controller se escucha a sí mismo) y la función
      `ForwardRefusalToHud`, que nadie llama. El rechazo llama directo a `MissionHud.ShowRefusal`.
- [ ] Quedan 3, cada uno con emisor y oyente reales:

| Dispatcher | Emisor → Oyente | Para |
|---|---|---|
| `OnActiveFigureChanged` | GameState → PlayerController | refrescar destinos y resaltados |
| `OnMissionEnded` (nuevo) | GameMode → HUD | mostrar la pantalla final (hoy se chequea en cada Tick) |
| `OnCharacterLost` | `BPC_Ratchet` → GameMode | derrota por trinquete |

- [ ] Bind en `BeginPlay` y **Unbind en `EndPlay`**: la Clase 5 enseña los dos.

### A3 · `E_TurnPhase` (13 valores) → `E_MissionState` (3)
- [ ] Nuevo enum `Playing / Won / Lost`. Borrar `EnterPhase` y `E_TurnPhase`.
- [ ] `ActionsRemaining = ActionsPerTurn` va dentro de `EndTurn`.
- [ ] El HUD deja de mostrar la línea "FASE".

### A4 · `BPC_Occupancy` → variable en `BP_Character`
- [ ] `CurrentSpace` + `SetSpace` en `BP_Character`. Se van el componente, `PreviousSpace`,
      su dispatcher y los `GetComponentByClass` + Cast del GameMode.

### A5 · Ubicar las figuras una sola vez
- [ ] Solo `BP_Space.RefreshOccupantPlacement` ubica, con la fórmula
      `Anchor + (i - (n-1)/2) * Spacing` en un eje.
- [ ] Borrar `GetFigurePlacement`, `GetOccupantLocation` y la grilla de ~40 nodos de `GetSlotLocation`.
- [ ] Por ahora sigue siendo teletransporte. Caminar con `Move To` entra con la Clase 6.

### A6 · Cámara
- [ ] Input directo (offset al actor, yaw y largo del brazo clampeado) + `Enable Camera Lag` y
      `Enable Camera Rotation Lag` en el SpringArm.
- [ ] Borrar las 6 variables Pending/Target/Current y `ConsumePan`, `ConsumeOrbit`, `ApplyToCamera`.

### A7 · Limpieza menor
- [ ] `IsSet` (copiado en 3 BPs) → `IsValid` directo.
- [ ] Pasar a variables locales: `PartySnapshot`, `Reach`, `Valid`, `Left`, `Tally`, `Best`,
      `Crossed`, `OldPosition`, `NewPosition`.
- [ ] Borrar `GetFiguresLeftCount`; se usa `Length` de `GetFiguresLeftThisRound`.
- [ ] Borrar `GetSelectableName` de `BPI_Selectable` y de sus 2 implementaciones.
- [ ] Borrar `E_OrderMode`. **Confirmar antes con el compañero**, porque entró en el último merge.
- [ ] Borrar los eventos vacíos `EventTick`/`BeginOverlap` (6 BPs).
- [ ] Fix Up Redirectors: `Core/BP_Character`, `Placeables/BP_Space`.
- [ ] `TraceSelectableUnderCursor`: un solo trace por canal y `Draw Debug Type = None`.
- [ ] `BPC_Ratchet`: queda `Position` + `AddToll` + `IsLost` + `OnCharacterLost`. Se borran
      `ThresholdsCrossed`, `OnRatchetAdvanced` y `OnThresholdCrossed` hasta que un umbral tenga efecto.
- [ ] **No se borran** `BP_PlayerState_Mission` ni `BP_GameInstance`: se usan en la Parte B.

### A8 · C++
- [x] Se mantiene `UGraphSubsystem` ("Implementación con C++" está en el programa).
- [x] ~~Sacar `BlockedTowards`~~ **Se queda.** D-31 sacó las paredes *destructibles*, pero los
      pasajes bloqueados siguen siendo regla del GDD (`mapa-y-espacios.md`: los define cada
      misión y el BFS los respeta). Hoy están vacíos en `L_Mission_01`; el segundo nivel puede usarlos.
- [x] `GetNearest` y `GetShortestPath` se quedan: los va a usar la IA enemiga (B · Clase 3).

### A9 · Documentación
- [ ] Marcar `design/architecture/` como histórica: describe clases de C++ que ya no existen y su
      regla 2 ("nunca Cast") contradice el estilo actual.

---

## Parte B · Cumplir el temario

Ya cumplido, sin cambios: estructura del proyecto y UE 5.8, Git + LFS, GameMode con
victoria/derrota, PlayerController + Pawn, canales y presets de colisión, Enhanced Input
(IA + IMC + Add Mapping Context), Line Trace, Material padre + MI, PBR, herencia, polimorfismo
por override, interfaz, UserWidget + Add to Viewport + Input Mode Game and UI.

### Clase 1 · Git
- [ ] Vigilar la cuota de LFS de GitHub (10 GB de almacenamiento y 10 GB/mes de descarga):
      `assets/3d` pesa 239 MB y cada clon la gasta.
- [ ] README con los pasos para un compañero nuevo: instalar LFS **antes** de clonar
      (`git lfs install`), checkout de `develop`, y Visual Studio con C++ para compilar el módulo,
      porque `Binaries/` no se sube.

### Clase 3 · Gameplay Framework + Físicas
- [ ] **PlayerState:** escribir `EnemiesKilled`, `SpacesMoved` y `CharactersLost` desde el
      GameMode, y mostrarlos en la pantalla final.
- [ ] **GameInstance:** guardar el nivel actual y la progresión para pasar de `L_Mission_01` a `L_Mission_02`.
- [ ] **AIController:** `BP_AIC_Enemy`, asignado en `BP_Enemy` con `Auto Possess AI`. Decide a
      qué casilla va el enemigo (`GetNearest` hacia el personaje más cercano).
- [ ] **Eventos de colisión:** al menos un interactuable con `On Component Begin/End Overlap`
      (ver Clase 7 · TP: 2 interactuables).
- [ ] **Física simulada:** un uso real, con `Simulate Physics` + `Add Impulse` + Physics
      Material. Candidato: los dados (decisión abierta A-09).

### Clase 4 · Inputs, Raycast y Posesión
- [ ] Usar Modifiers en los IA de cámara (por ejemplo `Negate` o `Swizzle` en el pan, `Scalar` en el zoom).
- [ ] Posesión de IA: la cubre el AIController de la Clase 3.

### Clase 4 · Materiales
- [ ] `M_ScrapBlade` y `M_ValveMaul` → Material Instances de un padre compartido.
- [ ] Revisar la resolución de las texturas de Meshy: ninguna de 4K (`Maximum Texture Size` o LOD group).

### Clase 5 · Diseño de clases y comunicación
- [ ] **Abstracción:** `Generate Abstract Class` en `BP_Character`.
- [ ] **Encapsulamiento:** `Health`, `MaxHealth`, `AttackDamage` y `CurrentSpace` como `Private`,
      con `Get`/`Set`. `Instance Editable` donde se tunea por instancia.
- [ ] **Polimorfismo:** sacar los Cast a `BP_Enemy` de `CheckVictory` y del Reckoning; se usa
      `IsHostile` sobre `BP_Character`.
- [ ] **Dispatchers:** bind y unbind (A2).
- [ ] **Niveles:** `Open Level (by Name)` para Menú → `L_Mission_01` → `L_Mission_02`.
- [ ] **Level Blueprint:** verificar que `L_Mission_01` no tenga lógica general.

### Clase 7 · UI y Sonidos
- [ ] `WBP_MainMenu`: Jugar y Salir (opcional: Controles). `Set Input Mode UI Only` + `Show Mouse Cursor`.
- [ ] `WBP_Pause`: `IA_Pause` en el IMC, `Set Game Paused`, Reanudar / Menú.
- [ ] `WBP_EndScreen`: victoria o derrota + estadísticas del PlayerState + Reintentar / Menú.
      Se abre desde `OnMissionEnded`.
- [ ] Objetivo comunicado en el HUD al empezar la misión.
- [ ] Animación de widget en el cartel de rechazo (fade in/out).
- [ ] **Sound Classes:** `Master` → `Sfx` / `Music` / `Ambient`.
- [ ] **Sound Cues** con `Random` + `Modulator` para seleccionar, mover, atacar, recibir golpe y
      botones de UI. Música de menú y de misión.

### Clase 6 · Animaciones (fuera de este plan)
Queda anotado lo que va a hacer falta: AnimBP con State Machine, Blend Space, Montages de ataque
y golpe, Anim Notify (daño en el frame del golpe, pasos), y que las figuras **caminen** con
`Move To` en lugar de teletransportarse (A5).

---

## Orden de ejecución

1. **A1 → A2 → A3**: se tocan entre sí y son lo que más simplifica.
2. **A4 → A5**, después **A6 → A9**.
3. **Clase 5**: abstracto, encapsulamiento, sacar los casts.
4. **Clase 7 + Clase 3**: menú, pausa, pantalla final, PlayerState, GameInstance y segundo nivel.
5. **Clase 3**: AIController, interactuables con overlap, física.
6. **Clase 4**: modifiers, materiales y texturas. **Clase 7**: sonido.
7. **Clase 1**: README para el grupo.

## Pendiente de verificar

- El cuerpo de los eventos de input (`IA_Select`, `IA_Cancel`, `IA_EndTurn`): el DSL los muestra vacíos.
- Los Modifiers y Triggers actuales del IMC.
- El Level Blueprint de `L_Mission_01`.
- La resolución de las texturas `T_*`.
- `E_OrderMode`, con el compañero.
