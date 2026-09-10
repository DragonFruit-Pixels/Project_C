#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RatchetComponent.generated.h"

USTRUCT(BlueprintType)
struct FRatchetTrackConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet", meta = (ClampMin = "4"))
	int32 TrackLength = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet")
	TArray<int32> Thresholds = { 4, 8, 12, 15, 18, 19 };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRatchetAdvanced, int32, NewPosition, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThresholdCrossed, int32, ThresholdPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLost);

UCLASS(ClassGroup = (ProjectC), meta = (BlueprintSpawnableComponent))
class PROJECTC_API URatchetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URatchetComponent();

	UFUNCTION(BlueprintCallable, Category = "Ratchet")
	void AddToll(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetPosition() const { return Position; }

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetThresholdsCrossedCount() const { return ThresholdsCrossedCount; }

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	bool IsLost() const;

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
