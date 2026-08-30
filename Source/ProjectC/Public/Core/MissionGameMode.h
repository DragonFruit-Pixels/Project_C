// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MissionGameMode.generated.h"

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

class ASpace;

UCLASS()
class PROJECTC_API AMissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMissionGameMode();

	virtual void StartPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Mission")
	void OnMissionReady();

	UFUNCTION(BlueprintPure, Category = "Mission")
	TArray<ASpace*> GetLegalDestinations(AActor* Figure) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mission")
	bool TryMoveFigure(AActor* Figure, ASpace* To);
	virtual bool TryMoveFigure_Implementation(AActor* Figure, ASpace* To);

	UPROPERTY(BlueprintReadWrite, Category = "Mission")
	EMissionTurnPhase Phase = EMissionTurnPhase::NotStarted;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 ActionsPerTurn = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 SpacesPerMove = 3;
};
