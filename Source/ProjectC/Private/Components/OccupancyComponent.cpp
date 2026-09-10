#include "Components/OccupancyComponent.h"
#include "Map/Space.h"

UOccupancyComponent::UOccupancyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOccupancyComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentSpace != nullptr)
	{
		CurrentSpace->AddOccupant(GetOwner());
	}
}

void UOccupancyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentSpace != nullptr)
	{
		CurrentSpace->RemoveOccupant(GetOwner());
	}

	Super::EndPlay(EndPlayReason);
}

ASpace* UOccupancyComponent::SetSpace(ASpace* NewSpace)
{
	if (CurrentSpace == NewSpace)
	{
		return CurrentSpace;
	}

	ASpace* const OldSpace = CurrentSpace;
	CurrentSpace = NewSpace;

	if (OldSpace != nullptr)
	{
		OldSpace->RemoveOccupant(GetOwner());
	}

	if (NewSpace != nullptr)
	{
		NewSpace->AddOccupant(GetOwner());
	}

	OnSpaceChanged.Broadcast(OldSpace, NewSpace);

	return OldSpace;
}
