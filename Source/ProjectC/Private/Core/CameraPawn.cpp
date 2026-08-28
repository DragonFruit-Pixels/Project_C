// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/CameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACameraPawn::ACameraPawn()
{
	// La unica excepcion de Tick del lado de la presentacion. Ver el comentario de la clase.
	PrimaryActorTick.bCanEverTick = true;

	// Un SceneComponent pelado como raiz: lo que se mueve al panear es el punto que la camara
	// mira, no la camara. Con la camara como raiz, orbitar la desplazaria en vez de girarla.
	USceneComponent* const Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	SetRootComponent(Pivot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Pivot);
	SpringArm->TargetArmLength = DefaultArmLength;
	SpringArm->SetRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));

	// El brazo no colisiona: un tablero se mira desde arriba y no hay nada entre la camara y el
	// piso que deba acercarla. Con colision, pasar sobre una pared alta pegaria un salto.
	SpringArm->bDoCollisionTest = false;

	// La interpolacion la hace Tick sobre los objetivos, no el brazo: asi el pan, el zoom y el
	// orbit comparten una sola constante de suavizado y no tres que se pelean.
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// El controller no rota este pawn: la camara la maneja el input de orbit y nada mas.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

	// Los objetivos arrancan en donde el pawn quedo colocado en el nivel, no en el origen: si no,
	// el primer frame lo arrastra desde (0,0,0) y se ve un barrido.
	TargetLocation = GetActorLocation();
	TargetArmLength = FMath::Clamp(DefaultArmLength, MinArmLength, MaxArmLength);
	TargetYaw = SpringArm->GetRelativeRotation().Yaw;

	SpringArm->TargetArmLength = TargetArmLength;
	SpringArm->SetRelativeRotation(FRotator(Pitch, TargetYaw, 0.0f));
}

void ACameraPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Panear mas rapido cuanto mas lejos esta la camara. Sin esto, alejado el mapa se recorre a
	// paso de hormiga y de cerca se pasa de largo: es la misma velocidad angular percibida.
	const float ZoomScale = TargetArmLength / FMath::Max(DefaultArmLength, 1.0f);

	if (!PendingPan.IsNearlyZero())
	{
		// El eje llega relativo a la camara: se rota por el yaw actual para que "adelante" siga
		// siendo hacia donde se mira despues de orbitar.
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
	// Positivo acerca, asi que resta largo de brazo.
	TargetArmLength = FMath::Clamp(TargetArmLength - Axis * ZoomStep, MinArmLength, MaxArmLength);
}

void ACameraPawn::AddOrbitInput(float Axis)
{
	PendingOrbit += Axis;
}
