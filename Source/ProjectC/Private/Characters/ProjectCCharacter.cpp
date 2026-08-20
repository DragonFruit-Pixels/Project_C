// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Characters/ProjectCCharacter.h"
#include "Characters/RatchetComponent.h"

AProjectCCharacter::AProjectCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Ratchet = CreateDefaultSubobject<URatchetComponent>(TEXT("Ratchet"));
}
