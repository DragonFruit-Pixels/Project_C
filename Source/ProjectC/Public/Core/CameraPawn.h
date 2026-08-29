// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * The player's body: a camera, and nothing else.
 *
 * **This is the game's possession model.** The `PlayerController` possesses this pawn and never
 * possesses a character; figures are picked with the mouse. The decision and its cost are in
 * design/architecture/04-mapa-de-clases.md, "El jugador no tiene cuerpo": a tactics game's input
 * is not "move my pawn", it is clicking a tile, and rotating possession meant two input routes
 * where one is enough.
 *
 * **Tick, on purpose.** The project states that a turn-based game with explicit phases does not
 * need Tick (01-por-donde-se-empieza.md). That rule is about the **rules**: no game state may
 * depend on the passage of time. A camera is pure presentation and touches no state, and without
 * interpolation the zoom snaps and the pan feels broken. This is one of the project's two
 * exceptions, and both sit on this side of the line.
 *
 * A tactical camera, not a free orbit: the pitch is fixed and it only turns in yaw. A board is
 * read from above, and letting the camera tilt only enables angles the map cannot be read from.
 */
UCLASS()
class PROJECTC_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Moves the point the camera looks at, in the plane of the board.
	 *
	 * The axis arrives **relative to the camera**, not to the world: after orbiting, "forward" is
	 * still wherever the camera faces. That is what keeps W from feeling arbitrary.
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddPanInput(const FVector2D& Axis);

	/** Positive moves closer. Applied at once because the wheel is discrete; the arm interpolates. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddZoomInput(float Axis);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddOrbitInput(float Axis);

	UFUNCTION(BlueprintPure, Category = "Camera")
	UCameraComponent* GetCamera() const { return Camera; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	// --- Knobs. The child Blueprint tunes them without recompiling. ---

	/** uu per second with the arm at its default length. Scaled by zoom: see Tick. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "100"))
	float PanSpeed = 1800.0f;

	/** uu of arm length per wheel notch. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "10"))
	float ZoomStep = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "10"))
	float OrbitSpeed = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "100"))
	float MinArmLength = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "200"))
	float MaxArmLength = 4500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "200"))
	float DefaultArmLength = 1800.0f;

	/** Negative looks downwards. -55 shows the figures' faces and the layout at the same time. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "-89", ClampMax = "-10"))
	float Pitch = -55.0f;

	/** How hard the camera chases its target. Higher = stiffer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Tuning", meta = (ClampMin = "1"))
	float InterpSpeed = 10.0f;

private:
	/** Targets that Tick chases. Input writes here; it never moves the actor directly. */
	FVector TargetLocation = FVector::ZeroVector;
	float TargetArmLength = 0.0f;
	float TargetYaw = 0.0f;

	/**
	 * Pan axis accumulated since the last Tick.
	 *
	 * Enhanced Input fires `Triggered` once per frame while the key is held, but does not hand
	 * over DeltaTime. Storing the axis and consuming it in Tick is what keeps the speed from
	 * depending on the framerate.
	 */
	FVector2D PendingPan = FVector2D::ZeroVector;
	float PendingOrbit = 0.0f;
};
