# Comentarios de los Blueprints

Estado al 15/11/2026, rama `demo`. Para cada grafo, la columna **Antes** dice cómo estaba (sin comentarios, parcial, completo, o trivial si no hace falta comentario). **Secciones** lista las cajas que tiene ahora, las de antes y las nuevas.

- **Cajas nuevas:** 347. **Textos corregidos:** 70 (tipeos, primera persona, textos desactualizados).
- **Nodos muertos borrados:** 357, en 13 grafos. Eran copias viejas sin conexión, apiladas sobre la lógica viva.
- **Arreglado:** el anuncio del Boss decía "Â·" y ahora usa un guion.

## Para revisar a mano

- **Cajas corridas:** algunas cajas viejas ya no envuelven los nodos que describen, porque los nodos se movieron después. Pasa en TryMoveFigure, TryAttack, CheckVictory, NotifyDeath (GameMode) y en RefreshOccupantPlacement (Space). Solo se corrigió el texto; para acomodarlas hay que arrastrarlas.
- **Cajas vacías:** en PlayerController quedaron TryGiveAttackOrder (3), ClickActor (2) y HandleEndTurn (1). En Character quedaron EventGraph (2) y FindStartSpace (1). En WBP_GameHUD quedó UpdateTarget (1). Se pueden borrar o mover.
- **Sin cajas nuevas:** hay nodos encimados en la misma posición en ClickActor (PlayerController), AddOccupant (Space), y las cadenas EventConstruct, OnExitClicked y OnContinueClicked (MainMenu). Los textos propuestos están en la tabla de cada uno.


### BP_GameMode

| Grafo | Antes | Secciones |
|---|---|---|
| GetGameState | trivial | — |
| SpendAction | completo | Si está resolviendo, no está en modo Playing o se quedó sin acciones · Vuelve sin gastar acciones · Resta 1 a las acciones restantes y vuelve |
| TryActivateFigure | parcial | Setea variables auxiliares · Si el state o la figura ESTÁN vacíos, vuelve con Active = False · Si el state y la figura NO ESTÁN vacíos: comienza la ronda si no empezó, activa la figura y devuelve si cambió · Si hay una carga de partida en curso, no activa nada |
| AdvanceTurn | parcial | Retira la figura que terminó su turno · Si ya actuaron todos, empieza una ronda nueva · Setea variable auxiliar · Muestra el banner YOUR TURN · Spawnea los enemigos de la nueva ronda y actualiza la música |
| EndTurn | parcial | Si no se está resolviendo y está en modo Playing, resuelve la acción · Marca Resolving; si la figura activa no es un personaje, vacía la cola y pone el timer en 0 · Si hay una carga de partida en curso, no hace nada · Si es un personaje, arma la cola de enemigos, arranca el timer y anuncia el turno enemigo |
| EndTurnIfOutOfActions | parcial | Si no quedan más acciones o la figura murió, el turno termina · Compara las acciones restantes y la vida del personaje con 0; si alguna llegó, llama a EndTurn |
| GetLegalDestinations | completo | Si no es un personaje, devuelve la lista vacía · Calcula destinos solo si es la figura activa y está parada en una casilla · Obtiene las posiciones alcanzables y le quita la posición actual |
| CanMoveFigure | completo | Sin destino no se mueve · Se puede si el destino es válido |
| AddSpacesMoved | completo | Suma al PlayerState las casillas que recorrió |
| TryMoveFigure | parcial | Setea variables auxiliares · Si NO puede mover la figura seleccionada vuelve · Si la PUEDE mover gasta una acción · Si pudo gastar la acción, suma las casillas recorridas este turno · Si hay una carga de partida en curso, no mueve nada · Guarda el camino en LastMovePath, mueve la figura al destino y arranca la persecución |
| TryAttack | parcial | Setea variables auxiliares · Si el attacker o el target están vacíos, vuelve sin hacer nada · Solo si son de bandos distintos · Si la vida del target es mayor a 0 (esto siempre debería pasar) · Atacante y objetivo están en el mismo espacio · Si alguno NO se cumple vuelve · Si SE CUMPLIERON gasta la acción, aplica daño y termina el turno en caso de quedarse sin acciones · Si hay una carga de partida en curso, no ataca · Gasta una acción; si no puede, vuelve sin atacar · Reproduce la animación de ataque contra el objetivo · Tira los dados del atacante (más sus BonusDice) y aplica al objetivo un daño igual a los aciertos · Suma los Toll como Ratchet y termina el turno si no quedan acciones |
| CheckVictory | parcial | Recorre todos los characters · Si es hostil, chequea si está vivo. SI ESTÁ, suma 1 a EnemiesAlive · Una vez recorridos todos, chequea si queda algún enemigo vivo. SI NO queda, termina el juego con victoria · Solo chequea la victoria si el Boss ya fue derrotado |
| NotifyDeath | parcial | Si la figura no vino vacía · Setea variable auxiliar · Si la figura es hostil, suma 1 a los enemigos asesinados y chequea si se ganó · Si NO era hostil, suma 1 a los characters muertos y chequea si se terminó el juego · Si murió el Boss, recorre todos los characters · Remata a cada enemigo vivo con daño igual a su vida y vuelve a chequear la victoria |
| EndGame | completo | Setea el GameState según Victory y llama al evento OnGameEnded |
| RollDice | sin comentarios | Guarda quién tira en LastRoller y vacía las caras de la tirada anterior · Tira Dice dados comunes y Bonus dados extra · Cada dado suma una cara al azar: StandardDie a RollFaces, BonusDie a RollBonusFaces · Pone en 0 aciertos y Toll y recorre las caras comunes y las bonus · Cuenta aciertos: en comunes valen acierto y acierto+Toll, en bonus solo acierto · Cuenta Toll: cada cara acierto+Toll o Toll suma 1 a RollTolls · Suma 1 a RollCount |
| AnyHeroAlive | trivial | — |
| BuildReckoningQueue | sin comentarios | Vacía la cola y recorre todos los characters · Agrega a la cola cada enemigo que siga vivo |
| ReckonStep | sin comentarios | Toma la figura activa como héroe · Sin héroe activo pasa el turno; si ya no está en Playing, deja de resolver · Si el héroe todavía se está moviendo, reintenta en 0.2 s · Sigue si quedan enemigos en la cola y el héroe está vivo · Saca al primer enemigo de la cola y le da EnemyTurnTimeout para actuar · Le da el turno a su IA (persecución o ataque); si no tiene IA, sigue de inmediato · Cola vacía: deja de resolver; tras la persecución, el héroe sigue con sus acciones · Tras el turno enemigo, pasa al siguiente turno y repone las acciones |
| EnemyAttack | sin comentarios | Calcula la distancia entre las casillas del enemigo y del héroe · Ataca solo si el héroe está vivo, no es hostil y está dentro de AttackRange · Marca que el enemigo actuó, centra la cámara en él y reproduce el ataque · Tira los dados del enemigo, daña al héroe con los aciertos y le suma los Toll |
| FinishEnemyTurn | trivial | — |
| StartChase | trivial | — |
| IsOnHeroPath | trivial | — |
| EnemyFollow | trivial | — |
| SpawnRoundEnemies | sin comentarios | Recorre todas las casillas con la ronda actual · Solo las casillas cuyo SpawnRound es la ronda actual · Calcula dónde aparece el enemigo: el SpawnLocation de la casilla, 97 unidades más arriba · Muestra el marcador, el efecto y el sonido de aparición, y centra la cámara ahí · Anuncia la ronda (texto especial si aparece el Boss) y suma 1 a AnnounceCount · Si toca un Broodling, lo spawnea en la casilla y acomoda a los ocupantes · Si toca el Boss, lo spawnea en la casilla y acomoda a los ocupantes · Reproduce la cinemática de aparición del Boss (LS_BossIntro) |
| HitStop | trivial | — |
| EndHitStop | trivial | — |
| IsLoadBlocked | trivial | — |
| SnapshotContainsFigure | trivial | — |
| FailRoundRestore | trivial | — |
| Debug_RequestRoundSave | trivial | — |
| CollectRoundSave | sin comentarios | Crea el objeto de guardado de la ronda · Marca la captura como válida, con SaveVersion 1 · Arma el MapName con la ruta del nivel actual · Guarda que al cargar se sigue en la ronda siguiente (NextRound) · Captura el estado de cada casilla y de cada figura, y toma el PlayerState · Copia las estadísticas del PlayerState; si no hay PlayerState, la captura queda inválida |
| SaveRoundCheckpoint | sin comentarios | Si hay una carga de partida en curso, no guarda · Solo guarda si se pidió (SaveRequested) · Espera a que la ronda haya empezado, todos hayan actuado y no haya nada resolviéndose · Arma la foto de la ronda y la descarta si es inválida o no tiene figuras · Guarda en el slot y, si salió bien, baja SaveRequested |
| PrepareRestoredEnemy | sin comentarios | Si el enemigo restaurado no tiene IA, le crea una; está listo si es un BP_EnemyAIController |
| ResolveSavedFigure | sin comentarios | Si la figura ya se resolvió, la devuelve desde LoadFigureMap · Si su casilla guardada no existe, devuelve vacío · Carga la clase guardada de la figura · Spawnea la figura en su casilla · Le pone su SaveId y la apoya sobre la casilla según la altura de su cápsula · La registra en LoadFigureMap |
| WorldReadyForRestore | sin comentarios | No está listo si no hay casillas o no coinciden con las del grafo · Espera a que cada figura y el PlayerController estén RestoreReady · Y que ya existan el PlayerState y el GameState |
| ValidateRestoreWorld | sin comentarios | Vacía los mapas de casillas y figuras de la carga · Falla si la cantidad de casillas no coincide con la guardada · Recorre las casillas y figuras del mundo y las figuras guardadas · Registra cada casilla y figura por su SaveId; falla si falta o se repite · Cada figura guardada debe tener su casilla y, si ya existe, ser de la misma clase |
| ValidateRestoredState | sin comentarios | Pone en 0 los contadores y falla si la ronda no es la NextRound guardada · Falla si ya hay figuras retiradas o una figura activa · Falla si la cantidad de figuras no coincide con la guardada; si coincide, las recorre · Busca la figura por su id; falla si no está o si su SaveId no coincide · Falla si cambió su vida o su SeatIndex · Falla si cambiaron MaxHealth, ActionsPerTurn, SpacesPerMove o DicePerRoll · Falla si no está en la casilla guardada · Falla si se está moviendo o no terminó de restaurarse · Falla si su casilla no la tiene como ocupante · Si es héroe, lo cuenta en HeroTotal y falla si no está en el Party · Falla si cambiaron su Ratchet o sus BonusDice · Al terminar, falla si no aparecieron todos los héroes del Party · Recorre todas las casillas · Suma los ocupantes de cada casilla a OccupantTotal · Falla si algún ocupante no es una figura del mundo · Es válido si OccupantTotal coincide con la cantidad de figuras |
| FinishRoundRestore | sin comentarios | Solo sigue si la carga está en LoadPhase 3 · Copia al PlayerState las estadísticas guardadas; sin PlayerState, falla · Reinicia el estado del turno: acciones, resolución, persecución, auxiliares y colas · Limpia la selección, da por terminada la carga y vacía los mapas de carga · Reanuda la IA de cada enemigo y spawnea los enemigos de la ronda |
| RestorePendingRound | sin comentarios | Vuelve la ronda del GameState a la NextRound guardada · Vacía los ocupantes de cada casilla y recorre las figuras del mundo y las guardadas · Destruye las figuras que no están en el guardado, reconstruye las guardadas y acomoda ocupantes · Verifica el estado restaurado: si falla avisa, si no termina la restauración |
| TryFinishLoadInitialization | sin comentarios | Solo sigue mientras la carga esté en LoadPhase 2 (esperando al mundo) · Suma un intento; pasados 200 intentos falla por timeout · Si el mundo todavía no está listo, reintenta en 0.1 s · Cierra el grafo del tablero; si no es válido, falla · Valida el mundo contra el guardado; si coincide, pasa a LoadPhase 3 y restaura la ronda |
| HandleLoadStartup | sin comentarios | Si LoadPhase es 5, cancela el guardado pendiente y cierra la carga sin restaurar · Si hay una carga en curso, pasa a LoadPhase 2 y espera al mundo con un timer |
| FocusCamera | trivial | — |
| HeroHurt | trivial | — |
| AnnounceTurn | trivial | — |
| AddRoundStat | sin comentarios | Toma el daño real (sin pasar la vida que tenía); si es mayor a 0, mira de qué bando es · Si la víctima es enemiga, suma el daño a RoundDealt y, si murió, una baja a RoundKills · Si es un héroe, suma el daño a RoundTaken |
| ShowRoundNote | sin comentarios | Calcula la nota según el daño recibido: S sin daño, A hasta 2, B hasta 4, si no C · Arma el título: ROUND n SURVIVED - GRADE x · Arma el detalle: bajas, daño hecho y daño recibido · Muestra la nota en pantalla y reinicia los contadores de la ronda |
| UpdateMusic | sin comentarios | Elige la pista según la ronda: calma (1-2), combate (3-4) o Boss (5) · Solo cambia si la pista elegida no es la que ya suena (MusicTrack) · Guarda la pista, apaga la anterior con fundido y arranca la nueva |
| EnsureFeedback | trivial | — |
| ImpactFeedback | trivial | — |
| AnnounceEnemyTurn | trivial | — |
| FocusOnActor | trivial | — |
| FadeOutMusic | trivial | — |
| StartMusic | trivial | — |
| EventGraph | parcial | Al empezar, si no hay carga pendiente, cierra el grafo de casillas y arranca el primer turno · Si hay una partida guardada para cargar, la restauración se encarga del inicio · En cada Tick, mientras Resolving, descuenta ReckonTimer y al llegar a 0 llama a ReckonStep |

### Notas para quien aplica

- **Cajas existentes corridas.** En TryMoveFigure, TryAttack, CheckVictory y NotifyDeath los nodos se movieron (auto-layout cada 280 px) después de crear las cajas: varias cajas del usuario ya no envuelven los nodos que describen (p. ej. TryAttack `Comment_19` está sobre la comparación de casillas, no sobre SpendAction/RollDice; NotifyDeath `Comment_24` "Setea variable auxiliar" quedó arriba de GetPlayerState/Cast/SetCurrentPlayerStateAux). Solo corregí el texto; reubicarlas queda a criterio.
- **Nodos encimados.** En RollDice, ReckonStep, RestorePendingRound, TryFinishLoadInitialization, HandleLoadStartup y CollectRoundSave hay nodos con la misma posición exacta (dos ramas dibujadas una encima de otra). Agrupé por filas; en RestorePendingRound el grupo "Destruye las figuras…" junta tres cosas porque están superpuestas. Conviene reacomodar esos grafos antes o después de aplicar.
- **ValidateRestoredState tiene una copia muerta.** 106 nodos (VariableSet_0..3, CallFunction_0..21, IfThenElse_0..13, etc.) forman una versión vieja de la función, sin conexión de ejecución desde la entrada (arranca en `K2Node_VariableSet_0`, que nadie llama) y dibujada encima de la versión viva. Las cajas propuestas cubren solo la versión viva (VariableSet_4 en adelante); hasta borrar la copia, las cajas van a contener esos nodos muertos.
- **AnnounceTurn**: `K2Node_CallFunction_0` (GetFeedback) y `K2Node_CallFunction_1` (ShowBanner) no tienen ejecución entrante: son restos encimados sobre EnsureFeedback/ShowBanner.
- **SpawnRoundEnemies**: los literales del anuncio salen en el dump como `  Â·  THE ABOMINATION AWAKENS` / `  Â·  SOMETHING STIRS`. Puede ser solo el encoding del dump, o un "·" mal guardado que se ve roto en pantalla: vale mirarlo en el juego.
- Nodos sueltos dentro de una caja nueva (inevitable por la grilla): EndTurn lit `CallFunction_78` en la caja de IsLoadBlocked; ValidateRestoredState `DynamicCast_4` (encimado con `CallFunction_30`); TryFinishLoadInitialization `GetSubsystem_0` (encimado con SetTimer).
- La regla de "rectángulo sin nodos ajenos" se chequeó con un script sobre pos..pos+(300,150), tolerando 20 px en el borde derecho: en cadenas con nodos cada 280 px, el siguiente nodo siempre cae en ese margen.

### BP_PlayerController

| Grafo | Antes | Secciones |
|---|---|---|
| RefreshLegalDestinations | completo | Limpia el array de destinos legales · Chequea que haya figura seleccionada (Selected Actor) y que esté en modo Mover · Chequea que no sea null · Obtiene los destinos legales y los setea |
| RefreshHighlights | parcial | Resetea los highlights anteriores a None y aplica los de apuntado con AimHighlights · Highlightea las locaciones legales y agrega los actores al array de Highlighted Actors · Si hay un actor siendo hovereado se Highlightea y se agrega al array de Highlighted Actors · Si hay un actor seleccionado se highlightea y se agrega al array de Highlighted Actors · Si el hover es una figura, la suma a HighlightedActor; si es hostil, la resalta como objetivo · Si es aliada, la resalta como hover solo si puede actuar y no hay otra figura activa |
| ClearSelection | parcial | Sale de los modos Apuntar y Mover · Vuelve a seleccionar la figura activa y actualiza destinos y highlights |
| TryGiveMoveOrder | completo | Si el input está bloqueado, no está en modo Mover o falta destino o figura, vuelve · Si el destino no forma parte de los destinos legales vuelve con error · Si no encuentra el GameMode, vuelve · Intenta mover la figura: si lo logra sale de Mover y actualiza destinos y highlights; si no, avisa |
| TryGiveAttackOrder | parcial | Si el input está bloqueado o falta figura u objetivo, vuelve · Si el objetivo que se intenta atacar no es hostil, vuelve · Si el objetivo no es hostil o no hay BP_GameMode, vuelve sin atacar · Si NO tiene GameMode vuelve · Intenta atacar; si lo logra, actualiza destinos y highlights · Si falló por falta de acciones, avisa que hay que terminar el turno · Intenta atacar al target. Si lo consigue actualiza los destinos legales y los Highlights · Si comparten casilla, el enemigo ya no se puede atacar; si no, está fuera de rango |
| TrySelect | trivial | — |
| TraceSelectableUnderCursor | completo | Obtiene la locación del cursor como vector 2D · Pasa el vector 2D a 3D · Proyecta el trace · Si no hay figura seleccionada o lo tocado no es seleccionable, devuelve lo que tocó · Tira otro line trace y vuelve si no pega contra nada |
| SelectActor | completo | Si la selección cambió, pide al GameMode activar la figura elegida · Si se pudo activar la figura (o se soltó la selección), la guarda y actualiza la vista · Guarda la selección y actualiza las locaciones legales y los highlights · Muestra el mensaje de error |
| ShowRefusal | trivial | — |
| ClickActor | completo | Chequea que el actor no esté vacío · Si está vacío, limpia la selección · Si se eligió un espacio/tile, limpia aiming e intenta mover · Si lo elegido no es un espacio/tile, limpia aiming e intenta atacarlo · Si no se pudo, intenta seleccionar · Si no pudo vuelve al select · Si no pudo vuelve a Select |
| BeginAiming | completo | Con una figura elegida entra en modo Apuntar y actualiza los resaltados; si no, avisa |
| HandleSelect | trivial | — |
| HandleEndTurn | parcial | Chequea que haya una figura activa; si no, avisa que hay que elegir una · Actualiza las locaciones legales y los Highlights · Termina el turno en el GameMode, sale de Apuntar/Mover y actualiza destinos y highlights |
| HandleCancel | sin comentarios | Si hay un PendingTarget, solo lo descarta · Si está apuntando o moviendo, sale de ese modo; si no, limpia la selección |
| BindToTurnChanges | completo | Cuando cambia el turno llama a HandleActiveFigureChanged |
| UnbindFromTurnChanges | trivial | — |
| GetGameMode | trivial | — |
| BeginMoving | completo | Con una figura elegida entra en modo Moverse y muestra los destinos; si no, avisa |
| ConfirmAttack | sin comentarios | Si el input no está bloqueado y hay PendingTarget, lo ataca y lo limpia |
| TryQueueAttack | sin comentarios | Si se vuelve a elegir el mismo objetivo, confirma el ataque · Toma la figura elegida y el objetivo como BP_Character; si no lo son, vuelve · Chequea que sea enemigo, esté en la misma casilla y queden acciones · Si se cumple, lo guarda en PendingTarget; si no, delega en TryGiveAttackOrder |
| HandleActiveFigureChanged | trivial | — |
| HandlePause | trivial | — |
| IsRestoreInputBlocked | trivial | — |
| MarkRestoreReady | trivial | — |
| Debug_RequestRoundSave | trivial | — |
| Debug_LoadRoundSave | trivial | — |
| Debug_RestartWithoutLoad | trivial | — |
| Debug_InspectRoundSave | sin comentarios | Si existe el guardado de la ronda, lo carga como BP_SaveGame_Round (debug) |
| PreviewPath | sin comentarios | Borra los puntos anteriores y ubica PathCursor en la figura seleccionada · Recorre el camino más corto hasta Space y une con puntos cada casilla con la anterior |
| EnsureDots | sin comentarios | Si Dots ya existe, lo devuelve · Si no, crea un actor con un InstancedStaticMesh para los puntos · Lo configura como esferas M_PathDot sin colisión ni sombra y lo guarda en Dots |
| ClearPathDots | trivial | — |
| DotSegment | sin comentarios | Calcula cuántos puntos entran en el tramo: uno cada 38 unidades · Por cada punto, interpola su posición entre From y To · Agrega una esfera chica a Dots en esa posición, con Z = 10 |
| DotsAlong | sin comentarios | Busca el camino por NavMesh entre From y To y mira si tiene más de un punto · Si no hay camino, une From y To en línea recta · Si hay camino, dibuja puntos en cada tramo, del punto i al i+1 |
| AimHighlights | sin comentarios | Reinicia AimTargets y sigue solo si está en modo Apuntar · Recorre las figuras que están en la casilla de la figura elegida · Resalta como objetivo a cada enemigo vivo y suma 1 a AimTargets |
| CanStartAction | sin comentarios | Si los enemigos todavía están actuando (Resolving), avisa y no deja actuar · Si no quedan acciones, avisa que hay que terminar el turno |
| EventGraph | parcial | Habilita el mouse sobre la UI, carga los controles y escucha los cambios de turno · Cada frame: si cambió lo que está bajo el mouse, lo guarda y actualiza los resaltados · Cada input llama a su acción · Al terminar deja de escuchar los cambios de turno · Tecla 2: entra en modo Apuntar · Marca el controller como listo para restaurar un guardado (RestoreReady) |

### BP_Character

| Grafo | Antes | Secciones |
|---|---|---|
| UserConstructionScript | completo | Setea el radio de la esfera · Activa la configuración de colisión del componente SelectionBounds · Setea la colisión como una de un objeto tipo Figure · Configura cómo responde la colisión al canal indicado · Asigna el material y aplica highlight al personaje · Si UseBodyAsBase, define y aplica una escala personalizada al Body · Establece la posición local del Body mediante un desplazamiento en el eje Z |
| GetHealth | trivial | — |
| GetMaxHealth | trivial | — |
| GetCurrentSpace | trivial | — |
| Die | completo | Avisa la muerte al BP_GameMode (NotifyDeath) y arranca StartDeath. Si falla el Cast, destruye el actor directamente. |
| ApplyDamage | parcial | Solo sigue si Amount es mayor que 0 · Suma el daño a las estadísticas de la ronda en el GameMode · Guarda PendingDamage y le resta Amount a la vida, hasta un mínimo de 0 · Guarda la vida; si llegó a 0 muere, si no programa PlayHit tras IncomingDelay |
| GetBaseColour | trivial | — |
| ApplyHighlight | parcial | Aplica el Highlight a la figura según el estado recibido, modificando el color del material y la intensidad del brillo (Glow) para representar visualmente cada estado. · Define el color mediante valores RGB · Aplica ese color al parámetro Colour del material de la figura para cambiar su apariencia según el estado seleccionado · Usa State como valor de stencil del contorno (color del outline) · Según State pinta Colour y Glow del Body: base, rojo, amarillo o azul |
| SetSpace | completo | Camina hasta el nuevo espacio y actualiza el asignado: sale del anterior, entra al nuevo y reacomoda a los ocupantes. · Si el nuevo espacio no es igual que el actual · Si el espacio actual no está vacío · Si el nuevo espacio no está vacío · Si el espacio actual no está vacío, saca a la figura de ahí y reacomoda a los que quedan · Recalcula las posiciones de los ocupantes para que queden distribuidos correctamente en el espacio |
| FindStartSpace | parcial | Si no se está cargando partida, busca el BP_Space cuya sala (RoomSize) contiene a la figura, se lo asigna y la agrega como ocupante. · Si no hay que saltearlo, recorre todos los BP_Space del nivel · Pasa la posición de la figura al espacio local de la sala · Registra la figura (Self) como ocupante del espacio seleccionado · Chequea si cae dentro de la sala: \|X\| e \|Y\| menores que la mitad de RoomSize · Si está adentro, le asigna esa sala, la agrega como ocupante y reacomoda |
| PlayOneShot | trivial | — |
| PlayHit | trivial | — |
| PlayDeath | sin comentarios | Muestra el impacto y el efecto NS_Death sobre la figura · Corta la animación en curso, marca IsDead y reproduce SC_Death · Programa BeginDissolve al 70% de DeathAnim (mínimo 0.2 s) |
| AttachWeapon | sin comentarios | Si hay WeaponMesh, crea un StaticMeshComponent sin colisión y lo pega a WeaponSocket |
| PlayAttack | sin comentarios | Mira al objetivo y reproduce AttackAnim · Programa el sonido del ataque 0.15 s antes del impacto · Avisa al objetivo cuándo llega el golpe para que se prepare (BraceFor) |
| StartDeath | sin comentarios | Programa PlayDeath para cuando llegue el golpe (IncomingDelay) · Destruye el actor al terminar DeathAnim más 1 s y el retraso del golpe |
| AttachHealthBar | sin comentarios | Crea un WidgetComponent en pantalla de 300x40, ubicado sobre la cabeza · Crea WBP_HealthPips para esta figura, con su nombre, y lo pone en el componente |
| LoadStats | sin comentarios | Lee la fila UnitRow de DT_Units y la guarda en Stats · Copia nombre, vida máxima, acciones, movimiento y dados a sus variables |
| WalkTo | sin comentarios | Encola el destino en WalkQueue y arranca WalkNext; al restaurar o al empezar la partida, se ubica directo |
| WalkPath | sin comentarios | Encola con WalkTo cada casilla intermedia del camino más corto hasta To |
| FaceTowards | sin comentarios | Guarda FaceTarget y gira la figura (solo en Yaw) para mirar a Other |
| PlayAttackSound | trivial | — |
| ShowImpact | sin comentarios | Lanza el efecto NS_Hit y el sonido SC_Hit sobre la figura · Destello (Flash) en el material que se apaga a los 0.12 s con EndFlash · Pide al GameMode la pausa de impacto (HitStop) y el feedback de pantalla · Muestra el número de daño (PendingDamage) arriba de la figura |
| BraceFor | trivial | — |
| BeginDissolve | sin comentarios | Oculta los StaticMesh de la figura (el arma) mientras se disuelve |
| DissolveStep | trivial | — |
| EndFlash | trivial | — |
| MarkRestoreReady | trivial | — |
| ShouldSkipStartSpace | sin comentarios | Saltea la casilla inicial si se está cargando una partida (LoadPhase entre 1 y 4) |
| RestoreAtLocation | sin comentarios | Corta la caminata: frena WalkNext, vacía WalkQueue y WalkSteps y apaga IsMoving · La teletransporta a Location manteniendo su altura actual |
| TryRestoreTeleport | sin comentarios | En la fase 3 de la carga, teletransporta la figura en vez de caminar y devuelve Handled |
| ClearRestorePlacement | sin comentarios | Corta la caminata, vacía WalkQueue y WalkSteps y deja la figura sin casilla |
| ApplySavedRecord | sin comentarios | Arma UnitRow con la fila guardada, carga sus stats e inicializa la figura · Pisa vida, acciones, movimiento y dados con lo guardado y la marca viva · La ubica en su asiento guardado y, si es héroe, restaura Ratchet y BonusDice |
| StartRecoil | sin comentarios | Solo los enemigos retroceden al recibir un golpe · La primera vez guarda la pose original del Mesh; luego arranca RecoilStep cada 0.016 s |
| RecoilStep | sin comentarios | Calcula el avance del retroceso (de 0 a 1 en 0.4 s); al llegar a 1 termina · Al terminar, restaura posición y escala del Mesh y frena el timer · Curva de rebote: sin(t·π)·(1−t), sube rápido y se apaga · Desplaza el Mesh en X hasta −60 según la curva (el retroceso) · Lo aplasta: ensancha X e Y hasta un 12% y achata Z hasta un 18% |
| EventGraph | parcial | Recibe el estado de Highlight que debe aplicarse al personaje · Inicialización: encuentra el espacio inicial y carga las estadísticas · Actualiza el espacio del personaje para desvincularlo del espacio actual · Actualiza el material del personaje según el estado recibido · Al recibir SetHighlight (BPI_Selectable), aplica ese estado con ApplyHighlight · Ejecuta la lógica correspondiente cuando el personaje sale del juego o es destruido · WalkNext: si quedan pasos en WalkSteps, va al siguiente · Llena la vida, agrega arma y barra de vida, ignora la colisión con Pawns y marca RestoreReady · Toma el próximo paso como WalkTarget (a la altura actual) y lo saca de WalkSteps · Si no quedan pasos pero hay destinos en WalkQueue, calcula el camino por NavMesh al próximo · Si no queda nada por caminar, apaga IsMoving y mira a FaceTarget · Marca IsMoving y suena un paso solo si el tramo es largo (más de 150) · Suma el destino final a los pasos, lo saca de WalkQueue y vuelve a llamar a WalkNext · Gira hacia WalkTarget (si está a más de 20) y desliza la cápsula hasta ahí según MoveSpeed |
| CanBeSelected | trivial | — |

### WBP_GameHUD

| Grafo | Antes | Secciones |
|---|---|---|
| ShowRefusal | completo | Muestra el mensaje y anota hasta cuándo se ve |
| UpdateRefusal | completo | Oculta el mensaje cuando pasó el tiempo |
| ResolveRefs | completo | Guarda GameMode, PlayerController y GameState para no buscarlos cada frame |
| ShowEnding | parcial | Muestra VICTORY o DEFEAT con su color · Muestra estadísticas en el panel final, oculta Resume, marca Ended y toca el sonido de fin |
| Refresh | sin comentarios | Actualiza los dados siempre; el resto del HUD, como mucho cada 0.1 s · Con GameState y PlayerController válidos actualiza cada parte del HUD; si faltan, los busca |
| UpdateActions | sin comentarios | Muestra acciones restantes / acciones por turno · Pinta los tres puntos de acción: verdes los disponibles, oscuros los usados |
| UpdateRound | parcial | Muestra la ronda y cuántas figuras faltan jugar · Escribe la ronda en TxtRound · Escribe cuántas figuras faltan actuar en TxtTurnsLeft |
| UpdateSelected | completo | Muestra el panel de la figura activa; si no hay ninguna, lo oculta |
| UpdateTarget | parcial | Muestra la vida del enemigo que está bajo el mouse · Si no hay un enemigo bajo el mouse, oculta el panel del objetivo · Escribe el nombre del enemigo, sus pips y su vida actual / máxima · Si el héroe activo comparte sala, muestra dados y chance de acertar; sin héroe, pide elegir uno · Si el enemigo está en otra sala, avisa que está fuera de alcance |
| FigureName | trivial | — |
| UpdateRoster | sin comentarios | Llena las 4 filas del roster (oculta las que no tienen héroe) y habilita End Turn si hay figura activa |
| ShowUnit | sin comentarios | Si hay figura, muestra el panel con su nombre, pips y vida; si no, lo oculta · Muestra cuántos dados tira la figura · Si es un héroe, muestra su Ratchet actual / máximo; si no, oculta la caja |
| CurrentPrey | sin comentarios | Devuelve el objetivo elegido (PendingTarget) o, si no hay, la figura bajo el mouse |
| RosterLine | sin comentarios | Pinta el nombre en verde si el mouse está sobre el héroe · Muestra la fila con nombre, pips y vida del héroe · Si es la figura activa: etiqueta AWAKE y fila a pleno · Si ya actuó en la ronda (Retired) muestra SPENT y atenúa la fila; si no, DORMANT |
| UpdateConfirm | sin comentarios | Busca el objetivo elegido y la figura activa; si no hay objetivo, oculta el panel · Muestra el panel y prende el brillo de Atacar; sin figura activa, los apaga · Escribe quién ataca a quién y el daño posible (0 a la cantidad de dados) · Muestra el nombre, los pips y la chance de acertar contra el objetivo |
| HeroDice | trivial | — |
| HitText | sin comentarios | Chance de al menos un acierto: 1 - 0.5^dados, en porcentaje |
| RollText | sin comentarios | Arma "Nombre: N DAMAGE" con la última tirada · Si salió algún Toll, agrega "+N MUTATION" |
| UpdateRoll | sin comentarios | Si hubo una tirada nueva, la anuncia con el texto de RollText · Vacía las listas de dados, recorre las caras normales y las bonus y anota la hora · Por cada cara agrega su etiqueta (HIT, MISS, TOLL...) y su color a las listas |
| UpdateActionBar | trivial | — |
| ShowPause | sin comentarios | Usa el panel final como menú de pausa: PAUSED, sin estadísticas y con Resume |
| TogglePause | sin comentarios | Si el juego está pausado, lo despausa y oculta el panel · Si no, y la partida no terminó, pausa y muestra el menú de pausa |
| PlayEndingSound | trivial | — |
| SetDie | sin comentarios | Según Index, muestra u oculta ese dado y le pone etiqueta, color y escala |
| UpdateDice | sin comentarios | Los dados se ven solo durante 3.5 s después de la tirada · Recorre los 6 casilleros de dado y se fija si hay etiqueta para cada uno · Sin etiqueta, oculta ese dado · Cada dado aparece 0.12 s después del anterior y se oculta a los 2.8 s · Al aparecer arranca más grande (x1.5) y vuelve a su tamaño en 0.18 s; lo aplica con SetDie |
| UpdateAnnouncement | sin comentarios | Si el GameMode tiene un anuncio nuevo, lo muestra en el mensaje |
| UpdateButtonLocks | sin comentarios | Move y Attack se habilitan si no se está resolviendo una acción y quedan acciones · End Turn se habilita mientras no se esté resolviendo una acción · Atenúa (35 %) los botones deshabilitados |
| EventGraph | parcial | Al crearse: busca las referencias y escucha el fin de partida · Cada frame: actualiza los textos y el mensaje · Al cerrarse: deja de escuchar el fin de partida · El botón Atacar activa el modo apuntar · El botón Moverse activa el modo mover · El botón Replay recarga el nivel actual · El botón Quit cierra el juego · El botón End Turn termina el turno · El botón Confirm lanza el ataque elegido (ConfirmAttack) · El botón Resume saca la pausa |

### BP_GameInstance

| Grafo | Antes | Secciones |
|---|---|---|
| FailLoad | sin comentarios | Guarda el error; si había una carga en curso, la marca como fallida (LoadPhase 4) |
| ValidateFigureRecord | sin comentarios | Rechaza el registro si le falta el ID, la clase, la fila de unidad o la sala · Rechaza asiento, vida, acciones, movimiento, dados, Ratchet o bonus fuera de rango · Rechaza si la fila de unidad no existe en DT_Units · Carga la clase guardada y es válido solo si hereda de BP_Character |
| ValidateRoundSave | sin comentarios | Reinicia el conteo de héroes vivos y las listas de IDs vistos · Rechaza si la captura no fue válida, la versión no es 1 o la ronda es menor a 1 · Rechaza si el guardado no es del mapa Laboratorio · Recorre las salas; al terminar exige al menos una sala y una figura · Cada sala necesita un ID no vacío y que no se repita · Recorre las figuras; al final es válido si quedó al menos un héroe vivo · Cada figura debe pasar ValidateFigureRecord, estar en una sala conocida y no repetir ID · Cuenta los héroes (clase BP_Player) y exige que sala + asiento no se repita; anota ID y asiento |
| RequestLoadGame | sin comentarios | Ignora el pedido si ya hay una carga en curso (LoadPhase > 0 y distinta de 4) · Si el slot no existe, avisa el error; si existe, lo carga · Si es un BP_SaveGame_Round empieza la carga; si no, falla |
| CompleteLoad | trivial | — |
| RestartWithoutLoad | trivial | — |
| BeginSnapshotLoad | sin comentarios | Valida el guardado; si sirve, lo deja pendiente (LoadPhase 1) y abre su mapa |

### BP_SaveGame_Round

| Grafo | Antes | Secciones |
|---|---|---|
| CaptureFigure | sin comentarios | Las figuras muertas no se guardan; si está moviéndose, la captura no es válida · La figura debe estar en una sala con SaveId y figurar entre sus ocupantes · Si la figura no tiene SaveId le genera un GUID; después recorre las ya guardadas · Rechaza ID o sala + asiento repetidos; al final guarda el registro (con Ratchet si es héroe) |
| AppendFigureRecord | sin comentarios | Arma un ST_SavedFigure con los datos de la figura y lo agrega a Figures |
| CaptureSpace | sin comentarios | Si la sala no tiene SaveId o ya está anotada, la captura no es válida; si no, la anota |

### WBP_HealthPips

| Grafo | Antes | Secciones |
|---|---|---|
| SetPip | sin comentarios | El pip se ve solo si hay figura viva y su índice es menor que la vida máxima · Calcula el daño previsto y el color base: rojo si es enemigo, verde si es héroe · Parpadea si este pip se perdería con el ataque previsto · Color final: blanco al parpadear, oscuro si es vida ya perdida |
| DamagePreview | sin comentarios | Si el mouse no está sobre esta figura, no hay daño previsto (0) · Solo cuenta si la figura activa está en la misma sala que esta · Devuelve los dados de la figura activa, más BonusDice si es héroe |
| UpdateName | sin comentarios | Muestra el nombre en mayúsculas si ShowName está activo y la figura está bajo el mouse o seleccionada |
| EventGraph | sin comentarios | Cada frame pinta los 9 pips (índices 0 a 8) y actualiza el nombre |

### WBP_Feedback

| Grafo | Antes | Secciones |
|---|---|---|
| Flash | trivial | — |
| ShowBanner | trivial | — |
| ShowNote | trivial | — |
| UpdateHint | sin comentarios | Si no está apuntando oculta el aviso; si apunta, se fija si ya eligió objetivo · Con objetivo oculta el aviso; sin objetivo, explica si hay enemigos en la sala o no |
| EventGraph | sin comentarios | Cada frame descuenta el tiempo de la viñeta, el banner y la nota, sin bajar de 0 · La viñeta se apaga en 0.7 s; banner y nota se desvanecen en sus últimos 0.35 y 0.6 s |

### BP_HUD_Level

| Grafo | Antes | Secciones |
|---|---|---|
| Show Refusal | trivial | — |
| TogglePause | trivial | — |
| EventGraph | parcial | Crea el HUD, lo guarda y lo muestra en pantalla · Pone la música de la ronda, prepara el feedback y arranca el sonido ambiente |

### BP_DamageNumber

| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | sin comentarios | Al aparecer: vive 1.6 s y pinta el número en rojo · Cada frame suma el tiempo vivido · A los 0.35 s muestra "-N", sacude la cámara y hace HitStop; ya visible, sube mirando a cámara |

58 comentarios nuevos y 13 editados. Las ediciones arreglan tipeos o acentos, salvo tres cambios de sentido (ver "Dudosos").

### BP_Space
| Grafo | Antes | Secciones |
|---|---|---|
| UserConstructionScript | sin comentarios | Escala el Tile al tamaño de RoomSize y le pone el perfil de colisión Space · Agranda el marcador de spawn (x1.5) si en la sala aparece el Boss · Oculta el marcador si hay SpawnSource; si no, lo muestra solo si SpawnRound > 0 |
| ApplyHighlight | parcial | Setea los materiales de las Tiles · Pinta el Tile según el estado: gris, verde, amarillo o azul, con más brillo cada vez |
| GetSlotLocation | completo | Centra la fila de ocupantes: resta la mitad de la cantidad y multiplica por Slot Spacing · Posiciona al personaje |
| GetOccupants | trivial | |
| RefreshOccupantPlacement | parcial | Recorre todos los ocupantes · Encuentra la nueva locación donde tiene que estar el ocupante · Formatea el nuevo vector de posición · Setea la nueva locación · Calcula fila y columna del asiento y el desplazamiento en X según RoomSize · Calcula el desplazamiento en Y y arma el vector del asiento · Lleva ese desplazamiento al mundo con la posición y la rotación de la sala · Si el ocupante está a más de 20 de su lugar, camina hasta ahí (WalkTo) |
| AddOccupant | sin comentarios | Solo si el ocupante todavía no está en la sala · Reinicia TakenSeats con los asientos bloqueados (BlockedSeats) · Si el marcador de spawn está visible, su asiento también cuenta como tomado · Pasa la posición del marcador a un índice de asiento y lo suma a TakenSeats · Suma a TakenSeats el asiento de cada ocupante actual · Mientras el asiento esté tomado, prueba el siguiente (NextSeat + 1) |
| RemoveOccupant | trivial | |
| GetSpawnLocation | trivial | |
| ResetSavedOccupants | trivial | |
| RestoreOccupant | trivial | |
| EventGraph | parcial | Se registra en el grafo con sus vecinos y bloqueos · Al pedir resaltado (BPI_Selectable), pinta la casilla con ApplyHighlight |

### BP_CameraPawn
| Grafo | Antes | Secciones |
|---|---|---|
| HandlePan | completo | Mueve la cámara en el plano, más rápido cuanto más lejos está |
| HandleZoom | completo | Acerca o aleja la cámara dentro de los límites. Le resta "largo" al brazo de la cámara |
| ApplyToCamera | completo | Solo si el ArmLength actual es diferente del deseado · Setea el nuevo "largo" del brazo sobre un tiempo. Suaviza el zoom hacia el largo objetivo |
| InitialiseCamera | completo | Inicializa el largo inicial · Inicializa el ángulo inicial |
| HandleOrbit | completo | Gira la cámara alrededor del centro |
| FocusOn | sin comentarios | Guarda en FocusTarget el punto pedido, a la altura actual de la cámara · Activa Focusing y llama a ApplyFocus en loop con un timer |
| ApplyFocus | sin comentarios | Acerca la cámara de a poco hacia FocusTarget · Al quedar a menos de 15 del objetivo, apaga Focusing y frena el timer |
| EventGraph | parcial | Las acciones de input panean, acercan y giran la cámara · Al empezar inicializa la cámara; cada frame suaviza el zoom · Al poseer: carga los controles de cámara |

### BP_GameState
| Grafo | Antes | Secciones |
|---|---|---|
| BeginRound | completo | Vacía la lista de los que ya jugaron · Suma 1 a la ronda |
| RegisterFigure | completo | Revisa que la figura exista y que todavía no esté en la lista Party. · Si es verdad, suma la figura a la Party |
| CanActThisRound | completo | Si no está en la lista Party, no puede actuar · Si está en la lista Retired, ya usó su turno · Actúa si la figura misma dice que se puede elegir (por ejemplo, si está viva) |
| GetFiguresLeftThisRound | completo | Recorre las figuras del jugador y guarda en FiguresLeft las que todavía pueden jugar esta ronda |
| RetireActiveFigure | completo | Valida si hay una figura jugando ahora mismo · La suma a Retired: ya jugó esta ronda · Deja de haber figura activa y avisa para que apague los resaltados del tablero. |
| SetActiveFigure | completo | Si ya era la activa, no cambia nada · Solo la activa si puede actuar y no hay otra activa · Guarda la figura activa, reinicia sus acciones y avisa el cambio |
| UnregisterFigure | completo | Saca la figura de las listas cuando muere. Si era la que estaba jugando, termina su turno para que el juego no quede trabado |
| ResetActions | sin comentarios | Copia ActionsPerTurn de la figura activa al GameMode y llena ActionsRemaining |
| RestoreRoundBoundary | sin comentarios | Al cargar partida, vacía Party y Retired, quita la figura activa y fija la ronda |
| EventGraph | trivial | |

### BP_Player
| Grafo | Antes | Secciones |
|---|---|---|
| CanBeSelected | trivial | |
| NextThreshold | sin comentarios | Devuelve el primer umbral de Ratchet por encima del actual; si no hay, RatchetMax |
| AddToll | sin comentarios | Solo si suma Toll y la figura sigue viva · Suena el Toll, busca el próximo umbral y se fija si Ratchet + Amount lo alcanza · Si lo alcanza, fija el Ratchet en el umbral; en RatchetMax la figura muere · Si no es el tope, gana un dado extra (BonusDice) · Si no alcanza el umbral, solo suma Amount al Ratchet |
| EventGraph | parcial | Al aparecer, copia RatchetMax y RatchetThresholds de Stats · Se registra en el GameState para entrar en las rondas · Se da de baja del GameState |

### BP_PlayerState
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | trivial | |

### BP_MainMenu
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | parcial | Arranca con el fondo, el botón Back y la selección de episodio ocultos · Set Event Start clicked · Para que se revelen los botones 1 a la vez · Set Event Start clicked · Episode Select Menu · Btn Start · Btn Options · Btn Exit · Background border · Btn Back · Episode Select · Btn Back · Btn Start · Btn Options · Btn Exit · Background border |

### BP_Episode
| Grafo | Antes | Secciones |
|---|---|---|
| SetEpisodeImage | sin comentarios | Con IsClicked, elige el sprite Clicked según EpisodeNum · Sin click, elige el sprite normal según EpisodeNum · Arma el estilo del botón con ButtonSprite (250x400) · Aplica el estilo a Button_0 |
| EventGraph | sin comentarios | Fija el tamaño del episodio (250x400), pone su imagen y oculta Image_353 · Al pasar el mouse reproduce OnHover; al salir, la reproduce al revés · Al clickear marca IsClicked y anima según si el episodio es seleccionable · A los 0.5 s vuelve la imagen normal; si es el episodio 1, avisa con EDEpiOnclick |

### BP_EpisodeSelect
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | sin comentarios | Escucha el click del episodio (EDEpiOnclick) para entrar al nivel · Reproduce la animación de entrada del selector · Abre el nivel Laboratorio |

### BP_ButtonMenu
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | completo | Size box · Button Sprite · Button Text · Enter btn Anim · Btn 0 Anim · OnClicked Anim |

### BP_ButtonBackMenu
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | completo | Size box · Button Sprite · Button Text · Enter btn Anim · Btn 0 Anim · OnClicked Anim |

### BP_EnemyAIController
| Grafo | Antes | Secciones |
|---|---|---|
| GiveTurn | sin comentarios | Carga en el Blackboard el héroe en turno y si se movió, y activa IsMyTurn |
| StartDecisionTree | sin comentarios | Arranca BaseTree y mete en el slot AI.Decide el subárbol de la araña (DecideTree) |
| ResumeAfterRestore | trivial | |
| InitializeForPossession | sin comentarios | Arranca el árbol si no hay una carga de partida en curso (LoadPhase 0 o 5) |
| EventGraph | sin comentarios | Al poseer la araña, prepara su IA con InitializeForPossession |

### BTT_Attack
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | sin comentarios | Toma la araña y el héroe en Target; si falta alguno, la tarea falla · El GameMode resuelve el ataque (EnemyAttack) y la tarea termina con su resultado |

### BTT_Follow
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | sin comentarios | Toma la araña y el héroe en Target; si falta alguno, la tarea falla · El GameMode la mueve hacia el héroe (EnemyFollow); si no puede, la tarea falla · Cada tick: cuando la araña deja de moverse, la tarea termina con éxito |

### BTT_FinishTurn
| Grafo | Antes | Secciones |
|---|---|---|
| EventGraph | sin comentarios | Apaga el flag de turno (TurnFlag) en el Blackboard · Avisa al GameMode que la araña terminó su turno (FinishEnemyTurn) |

### BTD_InRange
| Grafo | Antes | Secciones |
|---|---|---|
| PerformConditionCheckAI | sin comentarios | Toma la araña y el héroe en Target; si falta alguno, no está en rango · Está en rango si la distancia en salas entre los dos va de 0 a AttackRange |

### BTD_OnHeroPath
| Grafo | Antes | Secciones |
|---|---|---|
| PerformConditionCheckAI | sin comentarios | Pregunta al GameMode si la araña está en el camino que hizo el héroe (IsOnHeroPath) |

### ABP_Character
| Grafo | Antes | Secciones |
|---|---|---|
| AnimGraph (y estados Idle/Walk/Death, transiciones) | trivial | |
| EventGraph | sin comentarios | Cada frame copia IsMoving e IsDead del personaje para la máquina de estados · Al iniciar guarda el personaje dueño y sus animaciones Idle, Walk y Death |

### BP_Enemy
| Grafo | Antes | Secciones |
|---|---|---|
| UserConstructionScript | trivial | |
| GetBaseColour | completo | rojo oscuro |
| EventGraph | trivial | |

### BP_Boss / BP_Broodling
| Grafo | Antes | Secciones |
|---|---|---|
| UserConstructionScript | trivial | |
| EventGraph | trivial | |

### BPI_Selectable, BP_Brace, BP_Maul, BP_Shiv, BP_Spike
| Grafo | Antes | Secciones |
|---|---|---|
| (vacío) | trivial | |

### Dudosos
- **Huecos de 20 px entre cajas vecinas**: en los grafos acomodados en una sola fila (con nodos cada 280 px), la caja estimada de un grupo (pos + 300) llega al primer nodo del grupo siguiente. Esto pasa en BP_Space, CameraPawn FocusOn/ApplyFocus, AddToll y StartDecisionTree. Al aplicar las cajas conviene usar el ancho real de cada nodo.
- **BP_Space ApplyHighlight**: `K2Node_SwitchInteger_1`, `K2Node_CallFunction_16` a `_33` (menos `_34`) y `Knot_0` no tienen ningún cable de ejecución que les llegue: son código muerto. La caja "Setea los materiales de las Tiles" rodea esos nodos y, además, los nodos que sí se usan para el contorno (`SetRenderCustomDepth`). Si se borra el código muerto, el texto de la caja pasaría a "Activa el contorno (Custom Depth) solo en el estado 2".
- **BP_Space AddOccupant**: entre x=3608 y x≈6990 hay dos ramas encimadas en la fila y=0 (la rama del marcador y la del ForEach/While). Hay que reacomodarlas antes de poner cajas ahí. Textos propuestos para esa zona: "Lleva la posición del marcador al espacio local de la sala" y "Le asigna NextSeat al ocupante y lo suma a Occupants".
- **BP_Space RefreshOccupantPlacement**: las 4 cajas existentes quedaron corridas respecto de sus nodos, porque el grafo se reacomodó en una fila. Solo se corrigieron los acentos. Las cajas nuevas cubren el tramo que había quedado sin comentar (x ≥ 3104).
- **BP_Player EventGraph** (cambia el sentido): la caja "Se registra en el GameState…" hoy rodea solo la copia de Stats. Esa frase pasó a una caja nueva sobre RegisterFigure, y la caja vieja dice ahora lo que hay adentro.
- **BP_GameState SetActiveFigure** (cambia el sentido): se agregó "reinicia sus acciones", porque ahora llama a ResetActions.
- **BP_MainMenu**: las cadenas EventConstruct (los binds de EXIT/OPTIONS), OnExitClicked y OnContinueClicked están encima de las cajas "Btn Options", "Btn Exit" y "Background border". No se comentaron. Textos propuestos para cuando se muevan: "Conecta el click de EXIT y OPTIONS" · "Cierra el juego" · "Si hay guardado (RoundCheckpoint), pide cargarlo al GameInstance".
- **BP_EnemyAIController EventGraph**: OnPossess está encima de BeginPlay en (0,0). Además, `VariableGet_0`, `CallFunction_11`, `DynamicCast_0/1`, `CallFunction_12/13` y `VariableGet_1/2` son código muerto: repiten lo que hace StartDecisionTree y no tienen ningún cable de ejecución que les llegue.
