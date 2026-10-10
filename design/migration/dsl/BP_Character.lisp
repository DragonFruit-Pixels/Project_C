; BP_Character
; asset:  /Game/Project_C/Characters/Player/BP_Character
; parent: /Script/Engine.Character          (antes /Script/ProjectC.ProjectCCharacter)
; interfaz: Selectable  -- agregada a mano DESPUES del reparent, ver trap 21
; componentes: Body (StaticMesh, del Blueprint), Ratchet (BPC_Ratchet),
;              Occupancy (BPC_Occupancy), SelectionBounds (Sphere r=60, perfil Figure)
; capturado con BlueprintTools.read_graph_dsl despues de la migracion.

;; ---- graph: UserConstructionScript ----
(fn ConstructionScript ())


;; ---- graph: ApplyHighlight ----   (sin cambios respecto al pre-reparent)
(fn ApplyHighlight (State)
  (bind _body (Variables|Default|GetBody))
  (switch Utilities|FlowControl|Switch|SwitchonInt (Math|Conversions|ToInteger(Byte) State)
    (:0
      (Rendering|Material|SetVectorParameterValueOnMaterials _body "Colour" (Math|Vector|MakeVector 0.8 0.8 0.85))
      (Rendering|Material|SetScalarParameterValueOnMaterials _body "Glow"))
    (:1
      (Rendering|Material|SetVectorParameterValueOnMaterials _body "Colour" (Math|Vector|MakeVector 0.8 0.8 0.85))
      (Rendering|Material|SetScalarParameterValueOnMaterials _body "Glow"))
    (:2
      (Rendering|Material|SetVectorParameterValueOnMaterials _body "Colour" (Math|Vector|MakeVector 0.85 0.8 0.35))
      (Rendering|Material|SetScalarParameterValueOnMaterials _body "Glow" 1.2))
    (:3
      (Rendering|Material|SetVectorParameterValueOnMaterials _body "Colour" (Math|Vector|MakeVector 0.2 0.55 0.9))
      (Rendering|Material|SetScalarParameterValueOnMaterials _body "Glow" 2.0))))


;; ---- graph: CanBeSelected ----
;; el bind explicito NO es estilo: Class|BPCRatchet|IsLost no es pure y usada
;; inline se poda, se lee como false, y la figura perdida sigue seleccionable.
(fn CanBeSelected ()
  (bind _lost (Class|BPCRatchet|IsLost :self (Variables|Default|GetRatchet)))
  (return (not _lost)))


;; ---- graph: GetSelectableName ----
(fn GetSelectableName ()
  (return (Utilities|Text|ToText(String) (Utilities|GetDisplayName self))))


;; ---- graph: EventGraph ----
;; BeginPlay/EndPlay hacen el registro que antes hacia Super::BeginPlay() en C++.
(event EventBeginPlay
  (Rendering|Material|SetMaterial (Variables|Default|GetBody) 0 "/Game/Project_C/Map/Space/M_Space.M_Space")
  (bind _state (Utilities|Casting|CastToBP_GameState_Mission (Game|GetGameState))
    (:then
      (Class|BPGameStateMission|RegisterFigure :self _state :Figure self))))

(event EventEndPlay (EndPlayReason)
  (bind _state (Utilities|Casting|CastToBP_GameState_Mission (Game|GetGameState))
    (:then
      (Class|BPGameStateMission|UnregisterFigure :self _state :Figure self))))

(event Selectable|EventSetHighlight (Highlight)
  (CallFunction|ApplyHighlight :self self :State Highlight))
