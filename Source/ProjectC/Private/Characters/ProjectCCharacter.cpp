#include "Characters/ProjectCCharacter.h"
#include "Components/RatchetComponent.h"
#include "Components/OccupancyComponent.h"
#include "Core/ProjectCCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Core/MissionGameState.h"

AProjectCCharacter::AProjectCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Ratchet = CreateDefaultSubobject<URatchetComponent>(TEXT("Ratchet"));
	Occupancy = CreateDefaultSubobject<UOccupancyComponent>(TEXT("Occupancy"));

	SelectionBounds = CreateDefaultSubobject<USphereComponent>(TEXT("SelectionBounds"));
	SelectionBounds->SetupAttachment(GetCapsuleComponent());
	SelectionBounds->SetCollisionProfileName(ProjectCCollision::FigureProfile());

	SelectionBounds->SetSphereRadius(60.0f);
}

void AProjectCCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AMissionGameState* const State = GetWorld() ? GetWorld()->GetGameState<AMissionGameState>() : nullptr)
	{
		State->RegisterFigure(this);
	}
}

void AProjectCCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AMissionGameState* const State = GetWorld() ? GetWorld()->GetGameState<AMissionGameState>() : nullptr)
	{
		State->UnregisterFigure(this);
	}

	Super::EndPlay(EndPlayReason);
}

bool AProjectCCharacter::CanBeSelected_Implementation() const
{
	return Ratchet == nullptr || !Ratchet->IsLost();
}

FText AProjectCCharacter::GetSelectableName_Implementation() const
{
	return FText::FromString(GetActorNameOrLabel());
}
