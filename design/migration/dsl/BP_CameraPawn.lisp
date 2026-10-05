; BP_CameraPawn
; asset:  /Game/Project_C/Core/BP_CameraPawn
; parent: /Script/Engine.Pawn
; vars:   PanSpeed, ZoomStep, OrbitSpeed, MinArmLength, MaxArmLength, DefaultArmLength, Pitch, InterpSpeed, TargetLocation, TargetArmLength, TargetYaw, CurrentArmLength, CurrentYaw, PendingPan, PendingOrbit
; capturado con BlueprintTools.read_graph_dsl antes del reparent.

;; ---- graph: HandlePan ----
(fn HandlePan (Axis)
  (Variables|Camera|Runtime|SetPendingPan (Math|Vector2D|vector2d+vector2d (Variables|Camera|Runtime|GetPendingPan) Axis)))


;; ---- graph: HandleZoom ----
(fn HandleZoom (Axis)
  (Variables|Camera|Runtime|SetTargetArmLength (Math|Float|Clamp(Float) (- (Variables|Camera|Runtime|GetTargetArmLength) (* Axis (Variables|Camera|Tuning|GetZoomStep))) (Variables|Camera|Tuning|GetMinArmLength) (Variables|Camera|Tuning|GetMaxArmLength))))


;; ---- graph: HandleOrbit ----
(fn HandleOrbit (Axis)
  (Variables|Camera|Runtime|SetPendingOrbit (+ (Variables|Camera|Runtime|GetPendingOrbit) Axis)))


;; ---- graph: InitialiseCamera ----
(fn InitialiseCamera ()
  (bind _maxarmlength (Variables|Camera|Tuning|GetMaxArmLength))
  (bind _minarmlength (Variables|Camera|Tuning|GetMinArmLength))
  (bind _defaultarmlength (Variables|Camera|Tuning|GetDefaultArmLength))
  (bind _returnvalue (Math|Float|Clamp(Float) _defaultarmlength _minarmlength _maxarmlength))
  (bind _springarm (Variables|Default|GetSpringArm))
  (bind _relativerotation (Class|SceneComponent|GetRelativeRotation _springarm))
  (bind _yaw (.yaw _relativerotation))
  (Variables|Camera|Runtime|SetTargetLocation (Transformation|GetActorLocation self))
  (Variables|Camera|Runtime|SetTargetArmLength _returnvalue)
  (Variables|Camera|Runtime|SetCurrentArmLength _returnvalue)
  (Variables|Camera|Runtime|SetTargetYaw _yaw)
  (Variables|Camera|Runtime|SetCurrentYaw _yaw)
  (Variables|Camera|Runtime|SetTargetArmLength _returnvalue _springarm)
  (Transformation|SetRelativeRotation _springarm (Math|Rotator|MakeRotator 0.0 (Variables|Camera|Tuning|GetPitch) _yaw)))


;; ---- graph: ConsumePan ----
(fn ConsumePan (DeltaSeconds)
  (Variables|Camera|Runtime|SetTargetLocation (Math|Vector|vector+vector (Variables|Camera|Runtime|GetTargetLocation) (Math|Vector|vector*vector (Math|Vector|vector+vector (Math|Vector|vector*vector (Math|Vector|GetForwardVector (Math|Rotator|MakeRotator 0.0 0.0 (Variables|Camera|Runtime|GetTargetYaw))) (Math|Vector2D|BreakVector2D 0)) (Math|Vector|vector*vector (Math|Vector|GetRightVector (Math|Rotator|MakeRotator 0.0 0.0 (Variables|Camera|Runtime|GetTargetYaw))) (Math|Vector2D|BreakVector2D 0))) (Math|Vector|vector*vector (* (Variables|Camera|Tuning|GetPanSpeed) (/ (Variables|Camera|Runtime|GetTargetArmLength) (Math|Float|Max(Float) (Variables|Camera|Tuning|GetDefaultArmLength) 1.0))) DeltaSeconds))))
  (Variables|Camera|Runtime|SetPendingPan (Math|Vector2D|MakeVector2D 0.0)))


;; ---- graph: ConsumeOrbit ----
(fn ConsumeOrbit (DeltaSeconds)
  (Variables|Camera|Runtime|SetTargetYaw (+ (Variables|Camera|Runtime|GetTargetYaw) (* (* (Variables|Camera|Runtime|GetPendingOrbit) (Variables|Camera|Tuning|GetOrbitSpeed)) DeltaSeconds)))
  (Variables|Camera|Runtime|SetPendingOrbit 0.0))


;; ---- graph: ApplyToCamera ----
(fn ApplyToCamera (DeltaSeconds)
  (bind _interpspeed (Variables|Camera|Tuning|GetInterpSpeed))
  (bind _targetarmlength (Variables|Camera|Runtime|GetTargetArmLength))
  (bind _currentarmlength (Variables|Camera|Runtime|GetCurrentArmLength))
  (bind _returnvalue (Math|Interpolation|FInterpTo _currentarmlength _targetarmlength DeltaSeconds _interpspeed))
  (bind _targetyaw (Variables|Camera|Runtime|GetTargetYaw))
  (bind _currentyaw (Variables|Camera|Runtime|GetCurrentYaw))
  (bind _returnvalue_1 (Math|Interpolation|FInterpTo _currentyaw _targetyaw DeltaSeconds _interpspeed))
  (bind _springarm (Variables|Default|GetSpringArm))
  (Transformation|SetActorLocation self (Math|Interpolation|VInterpTo (Transformation|GetActorLocation self) (Variables|Camera|Runtime|GetTargetLocation) DeltaSeconds _interpspeed))
  (Variables|Camera|Runtime|SetCurrentArmLength _returnvalue)
  (Variables|Camera|Runtime|SetTargetArmLength _returnvalue _springarm)
  (Variables|Camera|Runtime|SetCurrentYaw _returnvalue_1)
  (Transformation|SetRelativeRotation _springarm (Math|Rotator|MakeRotator 0.0 (Variables|Camera|Tuning|GetPitch) _returnvalue_1)))


;; ---- graph: UserConstructionScript ----
(fn ConstructionScript ())


;; ---- graph: EventGraph ----
(event EnhancedInputActionIA_CameraPan (ActionValue ElapsedSeconds TriggeredSeconds InputAction))

(event EnhancedInputActionIA_CameraZoom (ActionValue ElapsedSeconds TriggeredSeconds InputAction))

(event EventBeginPlay
  (CallFunction|InitialiseCamera))

(event EnhancedInputActionIA_CameraOrbit (ActionValue ElapsedSeconds TriggeredSeconds InputAction))

(event EventTick (DeltaSeconds)
  (CallFunction|ConsumePan DeltaSeconds)
  (CallFunction|ConsumeOrbit DeltaSeconds)
  (CallFunction|ApplytoCamera DeltaSeconds))

