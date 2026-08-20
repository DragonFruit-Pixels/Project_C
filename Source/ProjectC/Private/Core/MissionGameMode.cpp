// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/MissionGameMode.h"
#include "Core/MissionGameState.h"
#include "Core/MissionPlayerController.h"

AMissionGameMode::AMissionGameMode()
{
	// El GameMode se enchufa desde la subclase Blueprint, no desde acá. Estos defaults son para
	// que el módulo sea coherente si alguien lo usa directo.
	GameStateClass = AMissionGameState::StaticClass();
	PlayerControllerClass = AMissionPlayerController::StaticClass();

	bStartPlayersAsSpectators = false;
	PrimaryActorTick.bCanEverTick = false;
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
