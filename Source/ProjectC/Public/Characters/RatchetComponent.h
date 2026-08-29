// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RatchetComponent.generated.h"

/**
 * Track configuration. The defaults are the original game's, verified.
 *
 * The owner of these numbers is design/gdd/07-balance/perillas-y-constantes.md. They sit here as
 * editable defaults so the child Blueprint can tune them without recompiling; once
 * DA_MissionConfig exists (class 12), this struct is filled from there.
 */
USTRUCT(BlueprintType)
struct FRatchetTrackConfig
{
	GENERATED_BODY()

	/** 20 spaces. Reaching the last one loses the character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet", meta = (ClampMin = "4"))
	int32 TrackLength = 20;

	/** 6 thresholds. The spacing accelerates: 4-4-4-3-3-1, and the last sits one tile from death. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet")
	TArray<int32> Thresholds = { 4, 8, 12, 15, 18, 19 };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRatchetAdvanced, int32, NewPosition, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThresholdCrossed, int32, ThresholdPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLost);

/**
 * A character's ratchet: a bar that **only goes up**.
 *
 * It is a component and not part of the base class because the mechanic belongs to the figure, not
 * to the hierarchy -- see design/architecture/04-mapa-de-clases.md.
 *
 * The rules live in FRatchetRules, which is pure and testable. This component is the one that
 * holds the state and announces through delegates; it reimplements no rule.
 */
UCLASS(ClassGroup = (ProjectC), meta = (BlueprintSpawnableComponent))
class PROJECTC_API URatchetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URatchetComponent();

	/** Adds `Toll`. It is the only route by which the ratchet moves. */
	UFUNCTION(BlueprintCallable, Category = "Ratchet")
	void AddToll(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetPosition() const { return Position; }

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetThresholdsCrossedCount() const { return ThresholdsCrossedCount; }

	/** True once the character has reached the end of the track. */
	UFUNCTION(BlueprintPure, Category = "Ratchet")
	bool IsLost() const;

	/** How far the next threshold is, or -1 if there is none left. */
	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetDistanceToNextThreshold() const;

	UPROPERTY(BlueprintAssignable, Category = "Ratchet")
	FOnRatchetAdvanced OnRatchetAdvanced;

	UPROPERTY(BlueprintAssignable, Category = "Ratchet")
	FOnThresholdCrossed OnThresholdCrossed;

	UPROPERTY(BlueprintAssignable, Category = "Ratchet")
	FOnCharacterLost OnCharacterLost;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet")
	FRatchetTrackConfig Config;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ratchet")
	int32 Position = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ratchet")
	int32 ThresholdsCrossedCount = 0;
};
