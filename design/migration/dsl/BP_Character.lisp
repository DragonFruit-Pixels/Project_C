; BP_Character
; asset:  /Game/Project_C/Characters/Player/BP_Character  (identico a /Game/Project_C/Core/BP_Character)
; parent: /Script/ProjectC.ProjectCCharacter
; vars:   (ninguna)
; capturado con BlueprintTools.read_graph_dsl antes del reparent.

;; ---- graph: UserConstructionScript ----
(fn ConstructionScript ())


;; ---- graph: ApplyHighlight ----
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


;; ---- graph: EventGraph ----
(event EventBeginPlay
  (Rendering|Material|SetMaterial (Variables|Default|GetBody) 0 "/Game/Project_C/Map/Space/M_Space.M_Space"))

(event Selectable|EventSetHighlight (Highlight)
  (Class|BPSpace|ApplyHighlight Highlight))

