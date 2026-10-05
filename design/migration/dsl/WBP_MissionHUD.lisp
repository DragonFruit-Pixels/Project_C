; WBP_MissionHUD
; asset:  /Game/Project_C/UI/WBP_MissionHUD
; parent: /Script/UMG.UserWidget
; vars:   RefusalUntil, Mission, MissionPC
; capturado con BlueprintTools.read_graph_dsl antes del reparent.

;; ---- graph: Refresh ----
(fn Refresh ()
  (Widget|SetText(Text) (Variables|WBP_MissionHUD|GetTxtActions) (Utilities|Text|ToText(String) (Utilities|String|Append "ACCIONES  " (Utilities|String|ToString(Integer) (Class|BPGameModeMission|GetActionsRemaining (Variables|Default|GetMission))))))
  (Widget|SetText(Text) (Variables|WBP_MissionHUD|GetTxtPhase) (Utilities|Text|ToText(String) (Utilities|String|Append "FASE  " (Utilities|String|EnumtoString (Class|RetainerBox|GetPhase (Variables|Default|GetMission))))))
  (Widget|SetText(Text) (Variables|WBP_MissionHUD|GetTxtTurn) (Utilities|Text|ToText(String) (Utilities|String|Append "TURNO  " (Utilities|GetDisplayName (Mission|GetActiveFigure (Variables|Default|GetMission))))))
  (Widget|SetText(Text) (Variables|WBP_MissionHUD|GetTxtRound) (Utilities|Text|ToText(String) (Utilities|String|Append (Utilities|String|Append "RONDA  " (Utilities|String|ToString(Integer) (Mission|GetRound (Variables|Default|GetMission)))) (Utilities|String|Append "    QUEDAN  " (Utilities|String|ToString(Integer) (Mission|GetFiguresLeftThisRound (Variables|Default|GetMission)))))))
  (Widget|SetText(Text) (Variables|WBP_MissionHUD|GetTxtSelected) (Utilities|Text|ToText(String) (Utilities|String|Append "SEL  " (Utilities|GetDisplayName (Selection|GetSelectedActor (Variables|Default|GetMissionPC)))))))


;; ---- graph: ShowRefusal ----
(fn ShowRefusal (Reason)
  (Widget|SetText(Text) (Variables|WBP_MissionHUD|GetTxtRefusal) Reason)
  (Widget|SetVisibility (Variables|WBP_MissionHUD|GetRefusalPanel))
  (Variables|Default|SetRefusalUntil (+ (Utilities|Time|GetGameTimeInSeconds) 3.0)))


;; ---- graph: UpdateRefusal ----
(fn UpdateRefusal ()
  (if (> (Utilities|Time|GetGameTimeInSeconds) (Variables|Default|GetRefusalUntil))
    (Widget|SetVisibility (Variables|WBP_MissionHUD|GetRefusalPanel) "Collapsed")))


;; ---- graph: EventGraph ----
(event UserInterface|EventConstruct
  (bind _asbp_game_mode_mission (Utilities|Casting|CastToBP_GameMode_Mission (Game|GetGameMode)))
  (Variables|Default|SetMission _asbp_game_mode_mission)
  (bind _asmission_player_controller (Utilities|Casting|CastToMissionPlayerController (Game|GetPlayerController 0)))
  (Variables|Default|SetMissionPC _asmission_player_controller))

(event UserInterface|EventTick (MyGeometry InDeltaTime)
  (ControlActor|Refresh)
  (CallFunction|UpdateRefusal))

