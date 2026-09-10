#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OccupancyComponent.generated.h"

class ASpace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpaceChanged, ASpace*, OldSpace, ASpace*, NewSpace);

UCLASS(ClassGroup = (ProjectC), meta = (BlueprintSpawnableComponent))
class PROJECTC_API UOccupancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOccupancyComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure, Category = "Occupancy")
	ASpace* GetSpace() const { return CurrentSpace; }

	UFUNCTION(BlueprintCallable, Category = "Occupancy")
	ASpace* SetSpace(ASpace* NewSpace);

	UPROPERTY(BlueprintAssignable, Category = "Occupancy")
	FOnSpaceChanged OnSpaceChanged;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Occupancy")
	TObjectPtr<ASpace> CurrentSpace;
};
