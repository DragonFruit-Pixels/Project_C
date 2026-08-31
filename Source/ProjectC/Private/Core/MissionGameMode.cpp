// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/MissionGameMode.h"
#include "Core/MissionGameState.h"
#include "Core/MissionPlayerController.h"
#include "Core/MissionPlayerState.h"
#include "GameFramework/Pawn.h"
#include "Characters/OccupancyComponent.h"
#include "Map/GraphSubsystem.h"
#include "Map/Space.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectCMission, Log, All);

AMissionGameMode::AMissionGameMode()
{
	GameStateClass = AMissionGameState::StaticClass();
	PlayerControllerClass = AMissionPlayerController::StaticClass();
	PlayerStateClass = AMissionPlayerState::StaticClass();

	DefaultPawnClass = APawn::StaticClass();

	bStartPlayersAsSpectators = false;
	PrimaryActorTick.bCanEverTick = false;
}

void AMissionGameMode::StartPlay()
{
	Super::StartPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGraphSubsystem* Graph = World->GetSubsystem<UGraphSubsystem>())
		{
			Graph->SealGraph();
		}
	}

	OnMissionReady();
}

TArray<ASpace*> AMissionGameMode::GetLegalDestinations(AActor* Figure) const
{
	TArray<ASpace*> Destinations;

	if (Figure == nullptr)
	{
		return Destinations;
	}

	const UOccupancyComponent* const Occupancy = Figure->FindComponentByClass<UOccupancyComponent>();
	if (Occupancy == nullptr)
	{
		UE_LOG(LogProjectCMission, Warning,
			TEXT("%s has no UOccupancyComponent, so it can reach nothing."), *Figure->GetName());
		return Destinations;
	}

	if (Occupancy->GetSpace() == nullptr)
	{
		return Destinations;
	}

	const UWorld* const World = GetWorld();
	const UGraphSubsystem* const Graph = World ? World->GetSubsystem<UGraphSubsystem>() : nullptr;
	if (Graph == nullptr)
	{
		return Destinations;
	}

	ASpace* const From = Occupancy->GetSpace();

	Destinations = Graph->GetReachable(From, SpacesPerMove);

	Destinations.Remove(From);

	return Destinations;
}

bool AMissionGameMode::CanMoveFigure(AActor* Figure, ASpace* To) const
{
	return To != nullptr && GetLegalDestinations(Figure).Contains(To);
}

FVector AMissionGameMode::GetFigurePlacement(AActor* Figure, ASpace* To) const
{
	if (Figure == nullptr || To == nullptr)
	{
		return FVector::ZeroVector;
	}

	FVector Origin = FVector::ZeroVector;
	FVector BoxExtent = FVector::ZeroVector;
	Figure->GetActorBounds(true, Origin, BoxExtent);

	return To->GetFigureAnchorLocation() + FVector(0.0f, 0.0f, BoxExtent.Z);
}

void AMissionGameMode::EndTurn_Implementation()
{
	UE_LOG(LogProjectCMission, Error,
		TEXT("EndTurn has no Blueprint implementation, so the turn cannot end and the actions ")
		TEXT("will never refill. BP_GameMode_Mission is what implements it."));
}

bool AMissionGameMode::TryMoveFigure_Implementation(AActor* Figure, ASpace* To)
{
	UE_LOG(LogProjectCMission, Error,
		TEXT("TryMoveFigure has no Blueprint implementation: %s cannot move to %s. ")
		TEXT("BP_GameMode_Mission is what implements it."),
		Figure ? *Figure->GetName() : TEXT("nothing"),
		To ? *To->GetName() : TEXT("nowhere"));

	return false;
}
