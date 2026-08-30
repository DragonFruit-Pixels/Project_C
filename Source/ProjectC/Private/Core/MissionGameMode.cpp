// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/MissionGameMode.h"
#include "Core/MissionGameState.h"
#include "Core/MissionPlayerController.h"
#include "Core/MissionPlayerState.h"
#include "GameFramework/Pawn.h"
#include "Characters/OccupancyComponent.h"
#include "Map/GraphSubsystem.h"
#include "Map/Space.h"

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

	// A minimal turn bootstrap, and nothing more than that.
	//
	// The real turn machine -- rounds, character rotation, the 4 phases -- does not exist yet:
	// that is class 5. Without this `ActionsRemaining` stays at 0 and no action can be paid for,
	// so there would be nothing to test. Entering `CharacterTurn` is the only thing that refills
	// the actions (see SetPhase), and from there it moves on to `Actions`.
	SetPhase(EMissionTurnPhase::CharacterTurn);
	SetPhase(EMissionTurnPhase::Actions);
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
	if (Occupancy == nullptr || Occupancy->GetSpace() == nullptr)
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

bool AMissionGameMode::TryMoveFigure(AActor* Figure, ASpace* To)
{
	if (Figure == nullptr || To == nullptr)
	{
		return false;
	}

	UOccupancyComponent* const Occupancy = Figure->FindComponentByClass<UOccupancyComponent>();
	if (Occupancy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryMoveFigure: %s has no UOccupancyComponent"), *Figure->GetName());
		return false;
	}

	// Legality is asked of the same function that paints the highlight. That is what guarantees
	// what is lit and what is allowed cannot diverge.
	if (!GetLegalDestinations(Figure).Contains(To))
	{
		return false;
	}

	// The action is charged after validating and before moving: if `SpendAction` fails there are
	// no actions left, and the figure has not moved.
	if (!SpendAction())
	{
		return false;
	}

	ASpace* const From = Occupancy->GetSpace();
	Occupancy->SetSpace(To);

	// Teleport, not a walk. Pathfinding is class 9; until then board movement is instantaneous
	// and the presentation layer still owes the animation.
	const FVector Anchor = To->GetFigureAnchorLocation();
	Figure->SetActorLocation(Anchor + FVector(0.0f, 0.0f, Figure->GetSimpleCollisionHalfHeight()));

	OnFigureMoved.Broadcast(Figure, From, To);

	return true;
}

void AMissionGameMode::SetPhase(EMissionTurnPhase NewPhase)
{
	if (Phase == NewPhase)
	{
		return;
	}

	Phase = NewPhase;

	// Entering a character's turn is the only thing that refills the actions. Putting it here and
	// not in the caller is what stops a new code path from forgetting to refill them.
	if (Phase == EMissionTurnPhase::CharacterTurn)
	{
		ActionsRemaining = ActionsPerTurn;
	}

	OnPhaseChanged.Broadcast(Phase);
}

bool AMissionGameMode::SpendAction()
{
	if (ActionsRemaining <= 0)
	{
		return false;
	}

	--ActionsRemaining;
	return true;
}
