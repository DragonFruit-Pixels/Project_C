; BP_PlayerController_Mission
; asset:  /Game/Project_C/Core/BP_PlayerController_Mission
; parent: /Script/ProjectC.MissionPlayerController
; vars:   MissionHud
; capturado con BlueprintTools.read_graph_dsl antes del reparent.

;; ---- graph: UserConstructionScript ----
(fn ConstructionScript ())


;; ---- graph: TraceSelectableUnderCursor ----
(fn TraceSelectableUnderCursor ()
  (bind _returnvalue (Selection|GetSpaceObjectTypes))
  (bind _returnvalue_1 (Selection|GetFigureObjectTypes))
  (bind _returnvalue_2 (Utilities|Enum|LiteralenumEInteractionMode "Move"))
  (bind _interactionmode (Variables|Selection|GetInteractionMode))
  (bind _returnvalue_3 (Utilities|Enum|Equal(Enum) _interactionmode _returnvalue_2))
  (bind _returnvalue_4 (select _returnvalue_3 _returnvalue _returnvalue_1))
  (bind _self self)
  (bind _hitresult (Game|Player|GetHitResultUnderCursorForObjects _self _returnvalue_4 false))
  (bind _hitactor (Collision|BreakHitResult _hitresult))
  (Utilities|Casting|CastToSelectable _hitactor
    (:then
      (return _hitactor))
    (:CastFailed
      (return 0))))


;; ---- graph: EventGraph ----
(event Custom|OnOrderRefused_Event_0 (Reason))

(event Custom|OnOrderRefused_Event_1 (Reason))

(event EventBeginPlay
  (bind _returnvalue (UserInterface|CreateWidget "/Game/Project_C/UI/WBP_MissionHUD.WBP_MissionHUD_C" self))
  (Variables|Default|SetMissionHud _returnvalue)
  (UserInterface|Viewport|AddToViewport _returnvalue)
  (Selection|AssignOnOrderRefused (AddEvent|Custom|OnOrderRefused_Event)))

(event Custom|OnOrderRefused_Event_2 (Reason))

(event Custom|OnOrderRefused_Event (Reason)
  (Class|WBPMissionHUD|ShowRefusal (Variables|Default|GetMissionHud) Reason))

