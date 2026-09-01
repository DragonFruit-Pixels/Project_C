#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Selectable.h"
#include "ProjectCCharacter.generated.h"

class URatchetComponent;
class UOccupancyComponent;
class USphereComponent;

UCLASS()
class PROJECTC_API AProjectCCharacter : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	AProjectCCharacter();

	UFUNCTION(BlueprintPure, Category = "ProjectC|Character")
	URatchetComponent* GetRatchet() const { return Ratchet; }

	UFUNCTION(BlueprintPure, Category = "ProjectC|Character")
	UOccupancyComponent* GetOccupancy() const { return Occupancy; }

	virtual bool CanBeSelected_Implementation() const override;
	virtual FText GetSelectableName_Implementation() const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<URatchetComponent> Ratchet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<UOccupancyComponent> Occupancy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<USphereComponent> SelectionBounds;
};
