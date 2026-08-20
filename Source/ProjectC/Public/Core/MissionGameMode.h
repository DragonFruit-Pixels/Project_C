// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MissionGameMode.generated.h"

/**
 * Las fases del turno, explícitas.
 *
 * Están declaradas como dato y no implícitas en el orden de los nodos porque la secuencia del GDD
 * (design/gdd/03-resolucion/turno.md) tiene pasos obligatorios que se saltean fácil. Y porque un
 * juego por turnos con fases explícitas no necesita Tick: todo pasa porque una fase lo disparó.
 */
UENUM(BlueprintType)
enum class EMissionTurnPhase : uint8
{
	NotStarted,
	StartOfRound,
	CharacterTurn,
	Actions,
	PressureCard,
	EndOfTurn,
	EndOfRound,
	ClockCheck,
	Finished
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionPhaseChanged, EMissionTurnPhase, NewPhase);

/**
 * El árbitro de la misión: reglas, secuencia de turno, victoria y derrota.
 *
 * **Decide, no guarda.** El estado que otros necesitan leer vive en AMissionGameState. Si algo de
 * acá tiene que ser leído por la UI o por un personaje, está en el lugar equivocado.
 *
 * Ver design/architecture/01-por-donde-se-empieza.md, paso 3.
 */
UCLASS()
class PROJECTC_API AMissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMissionGameMode();

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SetPhase(EMissionTurnPhase NewPhase);

	UFUNCTION(BlueprintPure, Category = "Mission")
	EMissionTurnPhase GetPhase() const { return Phase; }

	/** Consume una acción del turno. Devuelve false si no quedaban. */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	bool SpendAction();

	UFUNCTION(BlueprintPure, Category = "Mission")
	int32 GetActionsRemaining() const { return ActionsRemaining; }

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnMissionPhaseChanged OnPhaseChanged;

protected:
	/**
	 * 3 acciones por turno.
	 *
	 * El dueño de este número es design/gdd/07-balance/perillas-y-constantes.md. Está acá como
	 * default editable y va a salir de DA_MissionConfig cuando exista (clase 12) — la costura ya
	 * está puesta para que eso sea cambiar de dónde viene el valor, no reescribir las reglas.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 ActionsPerTurn = 3;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	EMissionTurnPhase Phase = EMissionTurnPhase::NotStarted;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 ActionsRemaining = 0;
};
