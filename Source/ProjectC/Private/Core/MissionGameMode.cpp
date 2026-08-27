// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/MissionGameMode.h"
#include "Core/MissionGameState.h"
#include "Core/MissionPlayerController.h"
#include "Core/MissionPlayerState.h"
#include "Map/GraphSubsystem.h"

AMissionGameMode::AMissionGameMode()
{
	// El GameMode se enchufa desde la subclase Blueprint, no desde acá. Estos defaults son para
	// que el módulo sea coherente si alguien lo usa directo.
	GameStateClass = AMissionGameState::StaticClass();
	PlayerControllerClass = AMissionPlayerController::StaticClass();
	PlayerStateClass = AMissionPlayerState::StaticClass();

	bStartPlayersAsSpectators = false;
	PrimaryActorTick.bCanEverTick = false;
}

void AMissionGameMode::StartPlay()
{
	// Super::StartPlay() es lo que dispara los BeginPlay de todos los actores, y ahi es donde
	// cada ASpace se registra. Sellar antes daria un grafo incompleto, que es peor que ninguno.
	Super::StartPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGraphSubsystem* Graph = World->GetSubsystem<UGraphSubsystem>())
		{
			Graph->SealGraph();
		}
	}
}

void AMissionGameMode::SetPhase(EMissionTurnPhase NewPhase)
{
	if (Phase == NewPhase)
	{
		return;
	}

	Phase = NewPhase;

	// Entrar al turno de un personaje es lo único que recarga las acciones. Ponerlo acá y no en el
	// llamador es lo que evita que un camino nuevo se olvide de recargarlas.
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
