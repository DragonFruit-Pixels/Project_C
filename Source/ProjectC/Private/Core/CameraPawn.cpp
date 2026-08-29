// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/CameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACameraPawn::ACameraPawn()
{
	// The only Tick exception on the presentation side. See the class comment.
	PrimaryActorTick.bCanEverTick = true;

	// A bare SceneComponent as the root: panning moves the point the camera looks at, not the
	// camera. With the camera as root, orbiting would translate it instead of turning it.
	USceneComponent* const Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	SetRootComponent(Pivot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Pivot);
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));

	// The arm does not collide: a board is viewed from above and there is nothing between the
	// camera and the floor that should pull it in. With collision, passing over a tall wall would
	// snap it.
	SpringArm->bDoCollisionTest = false;

	// Tick interpolates the targets, not the arm: that way pan, zoom and orbit share a single
	// smoothing constant instead of three that fight each other.
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// The controller does not rotate this pawn: the orbit input drives the camera and nothing else.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

	// The targets start wherever the pawn was placed in the level, not at the origin: otherwise
	// the first frame drags it from (0,0,0) and you see it sweep across.
	TargetLocation = GetActorLocation();
	TargetArmLength = FMath::Clamp(DefaultArmLength, MinArmLength, MaxArmLength);
	TargetYaw = SpringArm->GetRelativeRotation().Yaw;

	SpringArm->TargetArmLength = TargetArmLength;
	SpringArm->SetRelativeRotation(FRotator(Pitch, TargetYaw, 0.0f));
}

void ACameraPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Pan faster the further the camera is. Without this, zoomed out you crawl across the map and
	// zoomed in you overshoot: this keeps the perceived angular speed constant.
	const float ZoomScale = TargetArmLength / FMath::Max(DefaultArmLength, 1.0f);

	if (!PendingPan.IsNearlyZero())
	{
		// The axis arrives relative to the camera: it is rotated by the current yaw so "forward"
		// stays wherever you are looking after orbiting.
		const FRotator YawOnly(0.0f, TargetYaw, 0.0f);
		const FVector Forward = YawOnly.RotateVector(FVector::ForwardVector);
		const FVector Right = YawOnly.RotateVector(FVector::RightVector);

		TargetLocation += (Forward * PendingPan.Y + Right * PendingPan.X) * PanSpeed * ZoomScale * DeltaSeconds;
		PendingPan = FVector2D::ZeroVector;
	}

	if (!FMath::IsNearlyZero(PendingOrbit))
	{
		TargetYaw += PendingOrbit * OrbitSpeed * DeltaSeconds;
		PendingOrbit = 0.0f;
	}

	SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, InterpSpeed));

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaSeconds, InterpSpeed);

	const FRotator Current = SpringArm->GetRelativeRotation();
	const float NewYaw = FMath::FInterpTo(Current.Yaw, TargetYaw, DeltaSeconds, InterpSpeed);
	SpringArm->SetRelativeRotation(FRotator(Pitch, NewYaw, 0.0f));
}

void ACameraPawn::AddPanInput(const FVector2D& Axis)
{
	PendingPan += Axis;
}

void ACameraPawn::AddZoomInput(float Axis)
{
	// Positive means closer, so it subtracts arm length.
	TargetArmLength = FMath::Clamp(TargetArmLength - Axis * ZoomStep, MinArmLength, MaxArmLength);
}

void ACameraPawn::AddOrbitInput(float Axis)
{
	PendingOrbit += Axis;
}
