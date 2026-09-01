#include "Core/MissionGameMode.h"
#include "Core/MissionGameState.h"
#include "Controllers/MissionPlayerController.h"
#include "Core/MissionPlayerState.h"
#include "GameFramework/Pawn.h"
#include "Components/OccupancyComponent.h"
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

	const AMissionGameState* const State = GetGameState<AMissionGameState>();
	if (State != nullptr && !State->GetParty().IsEmpty() && State->GetActiveFigure() != Figure)
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

	return To->GetOccupantLocation(Figure) + FVector(0.0f, 0.0f, BoxExtent.Z);
}

void AMissionGameMode::AdvanceTurn()
{
	AMissionGameState* const State = GetGameState<AMissionGameState>();
	if (State == nullptr)
	{
		UE_LOG(LogProjectCMission, Error,
			TEXT("There is no AMissionGameState, so the round cannot advance."));
		return;
	}

	State->RetireActiveFigure();

	if (!State->GetParty().IsEmpty() && State->GetFiguresLeftThisRound().IsEmpty())
	{
		State->BeginRound();
	}
}

bool AMissionGameMode::TryActivateFigure(AActor* Figure)
{
	AMissionGameState* const State = GetGameState<AMissionGameState>();
	if (State == nullptr || Figure == nullptr)
	{
		return false;
	}

	if (State->Round == 0)
	{
		State->BeginRound();
	}

	return State->SetActiveFigure(Figure);
}

AActor* AMissionGameMode::GetActiveFigure() const
{
	const AMissionGameState* const State = GetGameState<AMissionGameState>();
	return State ? State->GetActiveFigure() : nullptr;
}

int32 AMissionGameMode::GetFiguresLeftThisRound() const
{
	const AMissionGameState* const State = GetGameState<AMissionGameState>();
	return State ? State->GetFiguresLeftCount() : 0;
}

int32 AMissionGameMode::GetRound() const
{
	const AMissionGameState* const State = GetGameState<AMissionGameState>();
	return State ? State->Round : 0;
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
