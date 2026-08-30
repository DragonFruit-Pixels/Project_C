// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Characters/ProjectCCharacter.h"
#include "Characters/RatchetComponent.h"
#include "Characters/OccupancyComponent.h"
#include "Core/ProjectCCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

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

bool AProjectCCharacter::CanBeSelected_Implementation() const
{

	return Ratchet == nullptr || !Ratchet->IsLost();
}

FText AProjectCCharacter::GetSelectableName_Implementation() const
{
	return FText::FromString(GetActorNameOrLabel());
}
