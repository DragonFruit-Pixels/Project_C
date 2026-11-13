# Guardado por rondas — etapa 1

Implementado con Blueprints y Unreal MCP el 2026-11-13. No incluye restauración ni cambios en el menú.

## Assets nuevos

- /Game/Project_C/Save/BP_SaveGame_Round (padre: SaveGame).
- /Game/Project_C/Save/ST_SavedFigure.

BP_SaveGame_Round:
- SaveVersion (int, actualmente 1).
- MapName (string: ruta completa del paquete del mapa, sin prefijo PIE).
- NextRound (int: ronda cerrada + 1).
- EnemiesKilled, CharactersLost, SpacesMoved (int: copias persistidas de BP_PlayerState).
- CaptureValid (bool: validación de la captura; no es estado de juego).
- Figures (array de ST_SavedFigure).
- SpaceIds (array de string: identidades del tablero).
- CaptureSpace(Space): valida IDs de celdas vacíos/duplicados.
- CaptureFigure(Figure): excluye Health <= 0; valida movimiento, celda, ocupación, identidad y asiento.
- AppendFigureRecord(Figure, RatchetValue, BonusDiceValue): construye el registro y muestra sus valores en Output Log.
Las referencias de parámetros son temporales: ninguna se almacena en el archivo.

ST_SavedFigure:
- FigureId, ClassPath, UnitRowName, SpaceId (string).
- SeatIndex, Health, MaxHealth, ActionsPerTurn, SpacesPerMove, DicePerRoll, Ratchet, BonusDice (int).

Figures es la lista completa de unidades vivas. La futura carga debe reconciliar TODAS las figuras del mapa contra esa lista: una figura colocada que no esté en ella no debe reaparecer. No hay historial de motivos de baja ni tombstones. Retired es transitorio y no se serializa.

## Blueprints existentes modificados

BP_Character y BP_Space:
- SaveId (string, editable por instancia).
- Sin cambios en eventos de inicialización, movimiento o combate.

BP_GameMode:
- SaveRequested (bool), true por defecto para probar el primer cierre de ronda.
- SaveSlotName (string), RoundCheckpoint por defecto.
- CollectRoundSave() -> Snapshot: crea una captura nueva, enumera todas las celdas y personajes (incluidos enemigos vivos), copia la próxima ronda y estadísticas.
- SaveRoundCheckpoint(): comprueba la solicitud y el límite, captura y escribe con SaveGameToSlot, UserIndex 0.
- Debug_RequestRoundSave(): activa una solicitud y muestra un mensaje.
- AdvanceTurn: una llamada nueva entre la rama existente que detecta el fin de ronda y BeginRound. La condición y la llamada originales se conservan.

BP_PlayerController:
- Debug_RequestRoundSave(): activa SaveRequested en el GameMode y muestra un mensaje.
- EventGraph: F5/Pressed llama esa función. No reemplaza los eventos existentes.

No se modificó BP_GameState ni el sistema de combate. No se agregó un administrador de turnos paralelo.

## Identidad y mapa

En /Game/Project_C/Levels/Prototype se asignaron SaveIds persistentes a las 9 celdas y las 4 figuras colocadas. El mapa se guardó para persistirlos.
Los IDs colocados son strings asignados una vez, por ejemplo placed:BP_Space_C_0. No se recalculan del nombre del actor durante la partida.
Las figuras vivas sin SaveId reciben un GUID al capturarse por primera vez; conservan ese valor en su instancia durante la sesión. La futura carga deberá restaurarlo.
Las celdas requieren ID asignado en el editor: no se inventa un ID de celda distinto en cada ejecución.
Para otro mapa, asignar SaveId únicos en Details a sus BP_Space y figuras colocadas, guardar el mapa y comprobar la captura. Al duplicar una instancia, asignarle otro ID porque Unreal copiará el anterior. Los validadores rechazan duplicados.
Solo se preparó Prototype; no se modificaron los demás mapas.

## Prueba manual

1. Abrir /Game/Project_C/Levels/Prototype y ejecutar PIE.
2. F5 solicita guardar en el próximo fin de ronda. La primera solicitud ya viene activada por defecto.
3. Completar normalmente los turnos de todas las figuras de la ronda.
4. Buscar [RoundSave] en Output Log:
   - Limite de ronda: recopilando estado...
   - Una línea por figura con ID, HP, celda, Ratchet y BonusDice.
   - EXITO. Proxima ronda: N.
   - Figuras vivas guardadas: cantidad.
5. Comprobar Saved/SaveGames/RoundCheckpoint.sav.
6. Tras un éxito, pulsar F5 para solicitar otro guardado. Esta etapa utiliza solicitudes de una sola escritura; no hace autoguardado continuo.
7. Modificar vida/ratchet jugando y repetir. Comparar las líneas con el estado real.
8. Probar bajas y enemigos generados en un escenario que los contenga. Confirmar que solo los vivos aparecen y que conservan su identidad entre guardados.
9. No llamar directamente SaveRoundCheckpoint durante una acción: se rechaza si Round <= 0, quedan figuras pendientes, Resolving o ChasePhase están activos.

Una captura con movimiento activo, IDs incorrectos u ocupación inconsistente no escribe el slot; la solicitud sigue pendiente para otra ronda. La escritura fallida también mantiene la solicitud. No se detiene ni retrasa el avance normal de rondas.
El slot de prueba se sobrescribe en cada guardado exitoso.

## Validación ejecutada

- Los cinco Blueprints modificados/nuevos compilaron con warnings_as_errors=true.
- PIE: cierre de ronda 1 -> archivo con próxima ronda 2, cuatro figuras; el GameState continuó en ronda 2, ActiveFigure vacío y Retired vacío.
- Se probó la función de solicitud del PlayerController y la conexión F5 se verificó por nodos/pines. No se simuló físicamente la tecla.
- Una llamada fuera del límite fue rechazada sin escritura.
- Otra captura cerró ronda 3 con cuatro figuras: Health=3, Ratchet=3, BonusDice=1 y NextRound=4.
- Se inspeccionaron directamente los campos enteros serializados del .sav mediante lectura binaria: SaveVersion=1, NextRound=4, cuatro Health=3, cuatro Ratchet=3 y cuatro BonusDice=1.
- El archivo contiene /Game/Project_C/Levels/Prototype y rutas de clases, IDs y nombres de filas; no contiene referencias PIE a instancias.
- No se ejecutó LoadGameFromSlot ni se aplicó restauración.
- PIE se detuvo al terminar.

Pendientes de validación: enemigos dinámicos, bajas, cambio de celda, duplicados/ocupación incorrecta y fallo real de escritura. No se localizó el generador de enemigos en la inspección anterior, y no se modificó ningún generador.
La futura restauración y cualquier estado adicional de misión/oleada requieren una etapa separada aprobada por el usuario.

