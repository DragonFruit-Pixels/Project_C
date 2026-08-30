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
	// The GameMode is wired up from the Blueprint subclass, not from here. These defaults exist
	// so the module is coherent if someone uses it directly.
	GameStateClass = AMissionGameState::StaticClass();
	PlayerControllerClass = AMissionPlayerController::StaticClass();
	PlayerStateClass = AMissionPlayerState::StaticClass();

	// The player possesses a camera and never a character
	// (design/architecture/04-mapa-de-clases.md). The camera itself is BP_CameraPawn, and it is the
	// Blueprint subclass that sets it here -- the whole camera lives in Blueprint because that is
	// where classes 1-13 of the syllabus are taught.
	//
	// A bodiless APawn is the coherent default for anyone using this class directly. Deliberately
	// not ADefaultPawn: that one brings its own camera, its own movement component and its own
	// input bindings, and all three would fight the Blueprint that is supposed to be in charge.
	DefaultPawnClass = APawn::StaticClass();

	bStartPlayersAsSpectators = false;
	PrimaryActorTick.bCanEverTick = false;
}

void AMissionGameMode::StartPlay()
{
	// Super::StartPlay() is what fires every actor's BeginPlay, and that is where each ASpace
	// registers itself. Sealing earlier would give an incomplete graph, which is worse than none.
	Super::StartPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGraphSubsystem* Graph = World->GetSubsystem<UGraphSubsystem>())
		{
			Graph->SealGraph();
		}
	}

	// The graph is complete, so the turn can start. What starts it lives in
	// BP_GameMode_Mission: the turn machine is Gameplay Framework, which is class 3 and therefore
	// Blueprint. All this owes the graph is an entry point that runs after the seal.
	OnMissionReady();
}

// Note: `Figure` is not `const AActor*` even though it is not modified. Blueprint does not
// support const object pointer parameters, and this function has to be callable from the HUD.
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
		// A figure with no UOccupancyComponent is a setup error, not a game state: it is a
		// Blueprint someone built without the component that puts it on the board. Saying so here
		// rather than in the mover catches it when the figure is selected, which is the first
		// moment anything asks the question.
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

	// The range rule is already written and tested in FGraphMath; here it is only queried.
	Destinations = Graph->GetReachable(From, SpacesPerMove);

	// `Reachable` includes the origin because d(a,a)=0 <= MaxSteps. As a destination it is useless.
	Destinations.Remove(From);

	return Destinations;
}

bool AMissionGameMode::TryMoveFigure_Implementation(AActor* Figure, ASpace* To)
{
	// Reached only when BP_GameMode_Mission does not override this, which for a working project
	// means the Blueprint is missing, broken or was reparented away. It refuses instead of moving
	// the figure: charging the action would mean keeping a second `ActionsRemaining` down here,
	// and two counters that nothing keeps in step is a worse failure than not moving at all.
	UE_LOG(LogProjectCMission, Error,
		TEXT("TryMoveFigure has no Blueprint implementation: %s cannot move to %s. ")
		TEXT("BP_GameMode_Mission is what implements it."),
		Figure ? *Figure->GetName() : TEXT("nothing"),
		To ? *To->GetName() : TEXT("nowhere"));

	return false;
}
