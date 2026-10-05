; BP_Space
; asset:  /Game/Project_C/Map/Space/BP_Space  (identico a /Game/Project_C/Placeables/BP_Space)
; parent: /Script/ProjectC.Space
; vars:   (ninguna)
; capturado con BlueprintTools.read_graph_dsl antes del reparent.

;; ---- graph: UserConstructionScript ----
(fn ConstructionScript ())


;; ---- graph: ApplyHighlight ----
(fn ApplyHighlight (State)
  (bind _tile (Variables|Default|GetTile))
  (switch Utilities|FlowControl|Switch|SwitchonInt (Math|Conversions|ToInteger(Byte) State)
    (:0
      (Rendering|Material|SetVectorParameterValueOnMaterials _tile "Colour" (Math|Vector|MakeVector 0.22 0.23 0.26))
      (Rendering|Material|SetScalarParameterValueOnMaterials _tile "Glow"))
    (:1
      (Rendering|Material|SetVectorParameterValueOnMaterials _tile "Colour" (Math|Vector|MakeVector 0.1 0.45 0.3))
      (Rendering|Material|SetScalarParameterValueOnMaterials _tile "Glow" 0.6))
    (:2
      (Rendering|Material|SetVectorParameterValueOnMaterials _tile "Colour" (Math|Vector|MakeVector 0.85 0.8 0.35))
      (Rendering|Material|SetScalarParameterValueOnMaterials _tile "Glow" 1.2))
    (:3
      (Rendering|Material|SetVectorParameterValueOnMaterials _tile "Colour" (Math|Vector|MakeVector 0.2 0.55 0.9))
      (Rendering|Material|SetScalarParameterValueOnMaterials _tile "Glow" 2.0))))


;; ---- graph: EventGraph ----
(event Selectable|EventSetHighlight (Highlight)
  (CallFunction|ApplyHighlight Highlight))

(event EventBeginPlay)

(event Collision|EventActorBeginOverlap (OtherActor))

(event EventTick (DeltaSeconds))

