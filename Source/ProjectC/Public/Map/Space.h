#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Selectable.h"
#include "Space.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTC_API ASpace : public AActor, public ISelectable
{
	GENERATED_BODY()

public:
	ASpace();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Space")
	TArray<TObjectPtr<ASpace>> Neighbours;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Space")
	TArray<TObjectPtr<ASpace>> BlockedTowards;

	UFUNCTION(BlueprintPure, Category = "Space")
	UBoxComponent* GetBounds() const { return Bounds; }

	UFUNCTION(BlueprintPure, Category = "Space")
	FVector GetFigureAnchorLocation() const;

	UFUNCTION(BlueprintPure, Category = "Space")
	FVector GetSlotLocation(int32 SlotIndex, int32 SlotCount) const;

	UFUNCTION(BlueprintPure, Category = "Space")
	FVector GetOccupantLocation(AActor* Occupant) const;

	UFUNCTION(BlueprintPure, Category = "Space")
	TArray<AActor*> GetOccupants() const;

	UFUNCTION(BlueprintCallable, Category = "Space")
	void AddOccupant(AActor* Occupant);

	UFUNCTION(BlueprintCallable, Category = "Space")
	void RemoveOccupant(AActor* Occupant);

	UFUNCTION(BlueprintCallable, Category = "Space")
	void RefreshOccupantPlacement();

	virtual bool CanBeSelected_Implementation() const override { return true; }
	virtual FText GetSelectableName_Implementation() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Space")
	TObjectPtr<UBoxComponent> Bounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Space", meta = (ClampMin = "1"))
	float SlotSpacing = 150.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Space")
	TArray<TObjectPtr<AActor>> Occupants;
};
