# Carga por rondas en Laboratorio

Implementada con Blueprints y Unreal MCP. BP_SaveGame_Round y ST_SavedFigure mantienen su formato de versión 1. No se modificaron el menú, otros niveles ni código C++.

## Uso manual

1. Abrir /Game/Project_C/Levels/Laboratorio y ejecutar PIE; dar foco al viewport.
2. F5 solicita un guardado para el próximo fin de ronda. Completar los turnos normalmente y esperar [RoundSave] EXITO.
3. F6 muestra el contenido del archivo actual, sin aplicarlo al mundo.
4. Jugar más acciones sin volver a solicitar guardado para producir diferencias de vida o posición.
5. Pulsar Ctrl + Alt + Shift + L. Se lee el slot configurado en BP_GameMode y se reabre el mapa del archivo, incluso si ya estaba abierto.
6. Esperar [RoundLoad] EXITO. Comprobar vidas, celdas, asientos, Ratchet, BonusDice y próxima ronda. Seleccionar un héroe debe preparar sus ActionsPerTurn mediante ResetActions existente.
7. Repetir la carga para comprobar que no se duplican figuras.
8. Si hay un error, revisar [RoundLoad]. La misma combinación reintenta leer el archivo.
9. Ctrl + Alt + Shift + R reinicia Laboratorio SIN aplicar un guardado y deja SaveRequested=false. También sirve para abandonar una restauración fallida.
10. Después de cargar o reiniciar con la combinación segura, F5 vuelve a habilitar una solicitud de guardado.

Las combinaciones requieren los tres modificadores. Se conservan F5 y F6. Se revisaron los bindings de juego y comandos del editor; no apareció otra acción con esas combinaciones completas. La conexión de teclas se verificó por nodos y modificadores. Las ejecuciones automatizadas se hicieron invocando las funciones del controlador mediante la consola de Unreal/MCP.

El archivo sigue en Saved/SaveGames/RoundCheckpoint.sav. La carga no llama a SaveGameToSlot. SaveRoundCheckpoint y las órdenes de juego quedan bloqueadas mientras la fase de carga no es inactiva, incluido un error posterior al viaje. No se solicita guardar al terminar de restaurar.

## API preparada para un futuro botón Continuar

Obtener BP_GameInstance y llamar RequestLoadGame(SlotName), por ejemplo con RoundCheckpoint. No conectar el menú todavía.
RequestLoadGame verifica existencia, lectura, conversión y datos antes de viajar. BeginSnapshotLoad es la operación interna compartida que valida y programa el cambio de mundo.
En esta implementación se acepta exclusivamente el mapa preparado /Game/Project_C/Levels/Laboratorio. Los demás mapas se rechazan: no se prepararon ni se modificaron.

## Coordinación e inicialización

GameInstance conserva PendingRoundSave, una referencia temporal al objeto SaveGame; nunca conserva referencias a actores del mundo anterior.
LoadPhase usa: 0 inactivo, 1 abriendo mapa, 2 esperando dependencias, 3 restaurando, 4 error bloqueado, 5 reinicio sin carga. LastLoadError conserva el diagnóstico.

La ruta de carga en BeginPlay del GameMode no ejecuta el EndTurn de arranque normal. Espera:
- GameState, PlayerState y PlayerController válidos.
- RestoreReady del controlador y de todos los personajes.
- Todas las celdas registradas en GraphSubsystem.
- Sellado válido del grafo y correspondencia de IDs entre tablero y archivo.

Un temporizador reintenta las comprobaciones; no usa un Delay como prueba de preparación. Tras 200 comprobaciones fallidas se informa timeout y se mantiene bloqueado el guardado.
Los personajes omiten FindStartSpace durante la carga, completan estadísticas, arma, barra de vida y colisión y después marcan RestoreReady.
La IA no inicia el Behavior Tree en OnPossess durante la restauración. Los enemigos reconstruidos reciben SpawnDefaultController si lo necesitan, y se comprueba BP_EnemyAIController antes de continuar.

## Restauración

Se validan versión, mapa, NextRound, clases derivadas de BP_Character, filas de DT_Units, estadísticas, IDs, asientos y existencia de al menos un héroe.
LoadSpaceMap y LoadFigureMap son índices temporales del nuevo mundo. Se validan antes de aplicar y se vacían al terminar.

- Se reconstruyen Party, Retired, ActiveFigure y Round. BeginRound solo limpia Retired e incrementa Round: la carga limpia Retired y asigna Round=NextRound, sin llamar a BeginRound.
- Una figura colocada ausente de Figures se destruye sin Die ni NotifyDeath. EndPlay conserva su limpieza existente.
- Una figura existente se reutiliza por SaveId; si la clase no coincide, se rechaza la carga.
- Un registro faltante se genera desde ClassPath; se aplican UnitRow, estadísticas y SaveId después de BeginPlay.
- Figures es la lista completa de vivos. No se agrega una segunda lista de muertos.
- Se limpian CurrentSpace y Occupants antes de reconstruirlos. AddOccupant se reutiliza y después se restituye el SeatIndex guardado.
- RefreshOccupantPlacement conserva el cálculo existente. Solo durante restauración, WalkTo deriva a RestoreAtLocation para colocar instantáneamente y limpiar colas de movimiento.
- LoadStats y OnSpawned se reutilizan para configurar las estadísticas de la fila y registrar héroes; después se aplican los valores persistidos.
- Se restauran EnemiesKilled, CharactersLost y SpacesMoved; no se vuelven a contar muertes.
- ValidateRestoredState comprueba ronda, figuras, vida, estadísticas, asientos, ocupación y Party antes de anunciar éxito.
- ActiveFigure queda vacío y ActionsRemaining=0 hasta activar una figura; SetActiveFigure/ResetActions prepara sus acciones.
- Se limpian las colas de resolución y persecución; la IA se habilita al terminar. Los mapas temporales y PendingRoundSave se liberan.

## Blueprints modificados y funciones nuevas

BP_GameInstance:
- FailLoad(Message)
- ValidateFigureRecord(Record) -> Valid
- ValidateRoundSave(Snapshot) -> Valid
- RequestLoadGame(SlotName)
- BeginSnapshotLoad(Snapshot)
- CompleteLoad()
- RestartWithoutLoad()
Variables: LoadPhase, LastLoadError, PendingRoundSave. Las listas de IDs/asientos usados para validar son locales.

BP_GameMode:
- IsLoadBlocked() -> Blocked
- HandleLoadStartup() -> Handled
- WorldReadyForRestore() -> Ready
- TryFinishLoadInitialization()
- ValidateRestoreWorld(Snapshot) -> Valid
- SnapshotContainsFigure(Id, Snapshot) -> Found
- ResolveSavedFigure(Record) -> Figure
- PrepareRestoredEnemy(Figure) -> Ready
- RestorePendingRound()
- ValidateRestoredState(Snapshot) -> Valid
- FinishRoundRestore()
- FailRoundRestore(Message)
Variables: RestoreAttempts, LoadSpaceMap, LoadFigureMap. Contadores de verificación locales.
Se agregaron comprobaciones de carga al inicio de SaveRoundCheckpoint, TryActivateFigure, EndTurn, TryMoveFigure, TryAttack y Debug_RequestRoundSave. La ruta normal de esas funciones se conserva.

BP_GameState:
- RestoreRoundBoundary(NextRound)
No se duplican las variables de ronda.

BP_Character:
- ShouldSkipStartSpace() -> Skip
- MarkRestoreReady()
- ApplySavedRecord(Record, Space)
- ClearRestorePlacement()
- RestoreAtLocation(Location)
- TryRestoreTeleport(Location) -> Handled
Variable: RestoreReady. Conexión al final de BeginPlay; comprobaciones de carga al inicio de FindStartSpace y WalkTo.

BP_Space:
- ResetSavedOccupants()
- RestoreOccupant(Figure, SavedSeat)
Sin nuevas variables persistentes ni cambios de conexiones del tablero.

BP_EnemyAIController:
- StartDecisionTree()
- InitializeForPossession()
- ResumeAfterRestore()
La inicialización existente del Behavior Tree y subárbol se reutiliza en StartDecisionTree; OnPossess pasa por la comprobación de carga.

BP_PlayerController:
- MarkRestoreReady()
- IsRestoreInputBlocked() -> Blocked
- Debug_LoadRoundSave()
- Debug_RestartWithoutLoad()
Variable: RestoreReady. Se protegen los puntos de entrada de selección, movimiento, ataque y fin de turno. F5/F6 se conservan.

No se modificaron BP_PlayerState, BP_Player, BP_Enemy, BP_Cook, BP_SaveGame_Round ni ST_SavedFigure.

## Validación ejecutada

Consultar round-load-validation.json.
- Compilación de los siete Blueprints con warnings_as_errors=true después de las etapas.
- Carga del RoundCheckpoint.sav real tras reabrir Laboratorio; comparación de los campos serializados con el mundo restaurado.
- Resultado: 7 figuras, Party=4, Retired vacío, ActiveFigure vacío, Round=2. Brace recuperó Health=2, Ratchet=1, celda 61 y asiento 1. Contador SpacesMoved=9.
- Activar Spike después de la carga dejó ActionsRemaining=3 y Round=2.
- El SHA-256 del archivo permaneció A8AF2C47D4800C59C05A7E963AA9F83BDE4642926ED8E141A91E529C5EEB1BAE durante cargas, errores y reinicios.
- Prueba específica de ausentes: copia en memoria sin un héroe y un enemigo. Quedaron 5 figuras, Party=3, dos controladores enemigos; los contadores restaurados fueron 1 y 1, sin volver a incrementarse.
- Prueba específica de reconstrucción: copia en memoria con TEST:DYNAMIC_COOK. Se crearon 8 figuras, un único enemigo adicional con Health=2, celda 55, asiento 0, BP_EnemyAIController y componentes de IA válidos. Se repitió sin duplicados.
- IDs duplicados se rechazaron antes del viaje. Una copia con tablero incompatible quedó bloqueada después del viaje, sin guardar; se verificaron reintento y reinicio seguro.
- La función temporal que fabricaba variantes de prueba fue retirada.

Alcance: las pruebas de ausentes y enemigo adicional ejercitan la restauración con copias del SaveGame en memoria. No sustituyen una prueba completa de matar/generar durante juego -> guardar en disco -> cargar, ni validan un generador de oleadas no localizado. La IA se difiere mediante el flujo comprobado de OnPossess; no se persistieron acciones intermedias.

