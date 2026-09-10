#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MissionGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveFigureChanged, AActor*, ActiveFigure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundBegan, int32, Round);

UCLASS()
class PROJECTC_API AMissionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 DoomTrackPosition = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 Round = 0;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void RegisterFigure(AActor* Figure);

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void UnregisterFigure(AActor* Figure);

	UFUNCTION(BlueprintPure, Category = "Mission")
	TArray<AActor*> GetParty() const;

	UFUNCTION(BlueprintPure, Category = "Mission")
	AActor* GetActiveFigure() const { return ActiveFigure; }

	UFUNCTION(BlueprintPure, Category = "Mission")
	bool HasActedThisRound(AActor* Figure) const;

	UFUNCTION(BlueprintPure, Category = "Mission")
	bool CanActThisRound(AActor* Figure) const;

	UFUNCTION(BlueprintPure, Category = "Mission")
	TArray<AActor*> GetFiguresLeftThisRound() const;

	UFUNCTION(BlueprintPure, Category = "Mission")
	int32 GetFiguresLeftCount() const;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	bool SetActiveFigure(AActor* Figure);

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void RetireActiveFigure();

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void BeginRound();

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnActiveFigureChanged OnActiveFigureChanged;

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnRoundBegan OnRoundBegan;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	TArray<TObjectPtr<AActor>> Party;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	TArray<TObjectPtr<AActor>> Retired;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	TObjectPtr<AActor> ActiveFigure;
};
