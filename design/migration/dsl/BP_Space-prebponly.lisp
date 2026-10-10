; BP_Space
; asset:  /Game/Project_C/Map/Space/BP_Space
; parent: /Script/ProjectC.Space        (se va a /Script/Engine.Actor)
; vars:   ninguna propia -- Tile y Bounds son componentes, Neighbours/BlockedTowards
;         vienen del padre C++ y son EditInstanceOnly (ver space-graph-capture.md)
; capturado con BlueprintTools.read_graph_dsl ANTES de sacar el modulo de C++.
;
; componentes, leidos de la instancia BP_Space_C_0 del nivel:
;   Bounds  BoxComponent        root, BoxExtent (250, 250, 100), profile "Space"
;                               creado por ASpace::ASpace(), hay que recrearlo a mano
;   Tile    StaticMeshComponent /Engine/BasicShapes/Cube.Cube
;                               RelativeLocation (0, 0, -110)
;                               RelativeScale3D  (4.8, 3.8, 0.2)
;                               OverrideMaterials[0] = /Game/Project_C/Map/Space/M_Space.M_Space
;                               agregado en el Blueprint, sobrevive al reparent
;
; lo que el padre C++ daba y hay que rehacer en nodos (de Space.cpp):
;   GetFigureAnchorLocation, GetSlotLocation, GetOccupantLocation, GetOccupants,
;   AddOccupant, RemoveOccupant, RefreshOccupantPlacement, GetBounds
;   + BeginPlay registrandose en el grafo
;   + GetSelectableName_Implementation -> FText::FromString(GetActorNameOrLabel())
;   + CanBeSelected_Implementation     -> true

;; ---- graph: UserConstructionScript ----
(fn ConstructionScript ())


;; ---- graph: ApplyHighlight ----
;; el switch es sobre ESelectionHighlight convertido a int: 0 None, 1 Legal,
;; 2 Hovered, 3 Selected. Pasa a E_SelectionHighlight (enum de Blueprint).
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
;; BeginPlay, ActorBeginOverlap y Tick estan VACIOS: son stubs de eventos que alguien
;; agrego y nunca se usaron. El registro en el grafo hoy lo hace ASpace::BeginPlay en C++,
;; no este grafo -- por eso BeginPlay aparece vacio y el tablero igual funciona.
(event Selectable|EventSetHighlight (Highlight)
  (CallFunction|ApplyHighlight Highlight))

(event EventBeginPlay)

(event Collision|EventActorBeginOverlap (OtherActor))

(event EventTick (DeltaSeconds))
