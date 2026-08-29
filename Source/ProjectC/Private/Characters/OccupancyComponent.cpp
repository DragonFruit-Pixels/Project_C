// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Characters/OccupancyComponent.h"
#include "Map/Space.h"

UOccupancyComponent::UOccupancyComponent()
{
	// No Tick: occupancy changes because a rule changed it, never because time passed.
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

	// The notification goes out after the write, not before: listeners must see the new state.
	OnSpaceChanged.Broadcast(OldSpace, NewSpace);
}
