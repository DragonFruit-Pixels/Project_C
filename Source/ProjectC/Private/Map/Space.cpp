#include "Map/Space.h"
#include "Map/GraphSubsystem.h"
#include "Core/ProjectCCollision.h"
#include "Components/BoxComponent.h"

ASpace::ASpace()
{
	PrimaryActorTick.bCanEverTick = false;

	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	SetRootComponent(Bounds);

	Bounds->SetCollisionProfileName(ProjectCCollision::SpaceProfile());

	Bounds->SetBoxExtent(FVector(250.0f, 250.0f, 100.0f));
}

void ASpace::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGraphSubsystem* Graph = World->GetSubsystem<UGraphSubsystem>())
		{
			Graph->RegisterSpace(this);
		}
	}
}

FVector ASpace::GetFigureAnchorLocation() const
{
	const FVector Centre = Bounds->GetComponentLocation();
	return FVector(Centre.X, Centre.Y, Centre.Z - Bounds->GetScaledBoxExtent().Z);
}

FVector ASpace::GetSlotLocation(int32 SlotIndex, int32 SlotCount) const
{
	const FVector Anchor = GetFigureAnchorLocation();

	if (SlotCount <= 1 || SlotIndex < 0)
	{
		return Anchor;
	}

	const int32 Columns = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(SlotCount)));
	const int32 Rows = FMath::DivideAndRoundUp(SlotCount, Columns);

	const int32 Column = SlotIndex % Columns;
	const int32 Row = SlotIndex / Columns;

	const float X = (static_cast<float>(Row) - 0.5f * static_cast<float>(Rows - 1)) * SlotSpacing;
	const float Y = (static_cast<float>(Column) - 0.5f * static_cast<float>(Columns - 1)) * SlotSpacing;

	return Anchor + Bounds->GetComponentRotation().RotateVector(FVector(X, Y, 0.0f));
}

FVector ASpace::GetOccupantLocation(AActor* Occupant) const
{
	const int32 Index = Occupants.IndexOfByKey(Occupant);

	if (Index == INDEX_NONE)
	{
		return GetSlotLocation(Occupants.Num(), Occupants.Num() + 1);
	}

	return GetSlotLocation(Index, Occupants.Num());
}

TArray<AActor*> ASpace::GetOccupants() const
{
	TArray<AActor*> Actors;
	Actors.Reserve(Occupants.Num());

	for (const TObjectPtr<AActor>& Occupant : Occupants)
	{
		if (IsValid(Occupant))
		{
			Actors.Add(Occupant);
		}
	}

	return Actors;
}

void ASpace::AddOccupant(AActor* Occupant)
{
	if (Occupant == nullptr || Occupants.Contains(Occupant))
	{
		return;
	}

	Occupants.Add(Occupant);
	RefreshOccupantPlacement();
}

void ASpace::RemoveOccupant(AActor* Occupant)
{
	if (Occupants.Remove(Occupant) > 0)
	{
		RefreshOccupantPlacement();
	}
}

void ASpace::RefreshOccupantPlacement()
{
	Occupants.RemoveAll([](const TObjectPtr<AActor>& Occupant) { return !IsValid(Occupant); });

	const int32 Count = Occupants.Num();

	for (int32 Index = 0; Index < Count; ++Index)
	{
		AActor* const Occupant = Occupants[Index];

		FVector Origin = FVector::ZeroVector;
		FVector BoxExtent = FVector::ZeroVector;
		Occupant->GetActorBounds(true, Origin, BoxExtent);

		Occupant->SetActorLocation(GetSlotLocation(Index, Count) + FVector(0.0f, 0.0f, BoxExtent.Z));
	}
}

FText ASpace::GetSelectableName_Implementation() const
{
	return FText::FromString(GetActorNameOrLabel());
}
