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

	virtual bool CanBeSelected_Implementation() const override { return true; }
	virtual FText GetSelectableName_Implementation() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Space")
	TObjectPtr<UBoxComponent> Bounds;
};
