#pragma once

#include "CoreMinimal.h"
#include "ProjectCTypes.generated.h"

UENUM(BlueprintType)
enum class EMissionTurnPhase : uint8
{
	NotStarted,
	StartOfRound,
	CharacterTurn,
	Actions,
	PressureCard,
	Reckoning,
	EndOfTurnEffects,
	Hazard,
	ClockCheck,
	ManifestationCheck,
	AdversaryEndOfTurn,
	EndOfRound,
	Finished
};

UENUM(BlueprintType)
enum class EInteractionMode : uint8
{
	SelectFigure,
	Move,
	Attack
};
