// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Selectable.generated.h"

UENUM(BlueprintType)
enum class ESelectionHighlight : uint8
{
	None = 0,
	Legal = 1,
	Hovered = 2,
	Selected = 3
};

UINTERFACE(BlueprintType, MinimalAPI)
class USelectable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTC_API ISelectable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	bool CanBeSelected() const;
	virtual bool CanBeSelected_Implementation() const { return true; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	void SetHighlight(ESelectionHighlight Highlight);
	virtual void SetHighlight_Implementation(ESelectionHighlight /*Highlight*/) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	FText GetSelectableName() const;
	virtual FText GetSelectableName_Implementation() const { return FText::GetEmpty(); }
};
