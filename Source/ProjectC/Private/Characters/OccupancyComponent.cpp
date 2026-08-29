// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Characters/OccupancyComponent.h"
#include "Map/Space.h"

UOccupancyComponent::UOccupancyComponent()
{
	// Sin Tick: la ocupacion cambia porque una regla la cambio, nunca por el paso del tiempo.
	PrimaryComponentTick.bCanEverTick = false;
}

void UOccupancyComponent::SetSpace(ASpace* NewSpace)
{
	if (CurrentSpace == NewSpace)
	{
		return;
	}

	ASpace* const OldSpace = CurrentSpace;
	CurrentSpace = NewSpace;

	// El aviso va despues de escribir, no antes: quien escuche tiene que ver el estado nuevo.
	OnSpaceChanged.Broadcast(OldSpace, NewSpace);
}
