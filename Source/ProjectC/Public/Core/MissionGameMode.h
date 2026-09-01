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
class AMissionGameState;

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

	UFUNCTION(BlueprintPure, Category = "Mission")
	bool CanMoveFigure(AActor* Figure, ASpace* To) const;

	UFUNCTION(BlueprintPure, Category = "Mission")
	FVector GetFigurePlacement(AActor* Figure, ASpace* To) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mission")
	bool TryMoveFigure(AActor* Figure, ASpace* To);
	virtual bool TryMoveFigure_Implementation(AActor* Figure, ASpace* To);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mission")
	void EndTurn();
	virtual void EndTurn_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void AdvanceTurn();

	UFUNCTION(BlueprintCallable, Category = "Mission")
	bool TryActivateFigure(AActor* Figure);

	UFUNCTION(BlueprintPure, Category = "Mission")
	AActor* GetActiveFigure() const;

	UFUNCTION(BlueprintPure, Category = "Mission")
	int32 GetFiguresLeftThisRound() const;

	UFUNCTION(BlueprintPure, Category = "Mission")
	int32 GetRound() const;

	UPROPERTY(BlueprintReadWrite, Category = "Mission")
	EMissionTurnPhase Phase = EMissionTurnPhase::NotStarted;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 ActionsPerTurn = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 SpacesPerMove = 3;
};
