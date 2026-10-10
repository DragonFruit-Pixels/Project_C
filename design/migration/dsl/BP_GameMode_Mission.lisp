; BP_GameMode_Mission
; asset:  /Game/Project_C/Core/BP_GameMode_Mission
; parent: /Script/ProjectC.MissionGameMode
; vars:   ActionsRemaining, OnPhaseChanged (dispatcher), OnFigureMoved (dispatcher)
; capturado con BlueprintTools.read_graph_dsl antes del reparent.

;; ---- graph: UserConstructionScript ----
(fn ConstructionScript ())


;; ---- graph: TryMoveFigure ----
(fn TryMoveFigure (Figure To)
  (if (not (Mission|CanMoveFigure Figure To))
    (return false)
    (else
      (bind _success (CallFunction|SpendAction))
      (if (not _success)
        (return false)
        (else
          (bind _returnvalue (Occupancy|SetSpace (Actor|GetComponentByClass Figure "/Script/ProjectC.OccupancyComponent") To))
          (Transformation|SetActorLocation Figure (Mission|GetFigurePlacement Figure To))
          (Default|CallOnFigureMoved Figure _returnvalue To)
          (return true))))))


;; ---- graph: SpendAction ----
(fn SpendAction ()
  (if (<= (Variables|Turn|GetActionsRemaining) 0)
    (return false)
    (else
      (Variables|Turn|SetActionsRemaining (- (Variables|Turn|GetActionsRemaining) 1))
      (if (<= (Variables|Turn|GetActionsRemaining) 0)
        (Mission|EndTurn))
      (return true))))


;; ---- graph: EnterPhase ----
(fn EnterPhase (NewPhase)
  (if (not (Utilities|Enum|Equal(Enum) (Variables|Mission|GetPhase) NewPhase))
    (Variables|Mission|SetPhase NewPhase)
    (if (Utilities|Enum|Equal(Enum) NewPhase (Utilities|Enum|LiteralenumEMissionTurnPhase "CharacterTurn"))
      (Variables|Turn|SetActionsRemaining (Variables|Mission|GetActionsPerTurn)))
    (Default|CallOnPhaseChanged NewPhase)))


;; ---- graph: EventGraph ----
(event EventBeginPlay)

(event Mission|EventEndTurn
  (Mission|AdvanceTurn)
  (CallFunction|EnterPhase "CharacterTurn")
  (CallFunction|EnterPhase "Actions"))

(event Mission|EventOnMissionReady
  (Mission|EndTurn))

(event EventTick (DeltaSeconds))


;; ---- graph: OnPhaseChanged ----
(fn OnPhaseChanged (NewPhase))


;; ---- graph: OnFigureMoved ----
(fn OnFigureMoved (Figure From To))

