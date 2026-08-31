#include "Characters/OccupancyComponent.h"
#include "Map/Space.h"

UOccupancyComponent::UOccupancyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ASpace* UOccupancyComponent::SetSpace(ASpace* NewSpace)
{
	if (CurrentSpace == NewSpace)
	{
		return CurrentSpace;
	}

	ASpace* const OldSpace = CurrentSpace;
	CurrentSpace = NewSpace;

	OnSpaceChanged.Broadcast(OldSpace, NewSpace);

	return OldSpace;
}
