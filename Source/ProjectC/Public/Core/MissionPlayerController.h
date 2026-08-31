// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EngineTypes.h"
#include "Core/Selectable.h"
#include "MissionPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ASpace;

UENUM(BlueprintType)
enum class EInteractionMode : uint8
{
	SelectFigure,
	Move,
	Attack
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredChanged, AActor*, HoveredActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, AActor*, SelectedActor);

UCLASS()
class PROJECTC_API AMissionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMissionPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SelectActor(AActor* NewSelection);

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void ClearSelection();

	UFUNCTION(BlueprintPure, Category = "Selection")
	AActor* GetSelectedActor() const { return SelectedActor; }

	UFUNCTION(BlueprintPure, Category = "Selection")
	AActor* GetHoveredActor() const { return HoveredActor; }

	UPROPERTY(BlueprintReadWrite, Category = "Selection")
	EInteractionMode InteractionMode = EInteractionMode::SelectFigure;

	UFUNCTION(BlueprintPure, Category = "Selection")
	static TArray<TEnumAsByte<EObjectTypeQuery>> GetSpaceObjectTypes();

	UFUNCTION(BlueprintPure, Category = "Selection")
	static TArray<TEnumAsByte<EObjectTypeQuery>> GetFigureObjectTypes();

	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnHoveredChanged OnHoveredChanged;

	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnSelectionChanged OnSelectionChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MissionContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	int32 MissionContextPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> EndTurnAction;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
	AActor* TraceSelectableUnderCursor();
	virtual AActor* TraceSelectableUnderCursor_Implementation();

	void HandleSelect();
	void HandleCancel();
	void HandleEndTurn();

private:
	void RefreshLegalDestinations();

	void RefreshHighlights();

	UPROPERTY(Transient)
	TObjectPtr<AActor> HoveredActor;

	UPROPERTY(Transient)
	TObjectPtr<AActor> SelectedActor;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ASpace>> LegalDestinations;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> HighlightedActors;
};
