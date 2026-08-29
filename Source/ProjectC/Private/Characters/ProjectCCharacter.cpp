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

	// A little wider than the default capsule (34 uu) so clicking does not demand precision.
	SelectionBounds->SetSphereRadius(60.0f);
}

bool AProjectCCharacter::CanBeSelected_Implementation() const
{
	// A lost character stays in the world until a rule removes it, but can no longer be picked.
	// See design/gdd/02-personaje/trinquete.md.
	return Ratchet == nullptr || !Ratchet->IsLost();
}

FText AProjectCCharacter::GetSelectableName_Implementation() const
{
	return FText::FromString(GetActorNameOrLabel());
}
