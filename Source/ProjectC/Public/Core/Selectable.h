// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Selectable.generated.h"

/**
 * How a selectable thing is being displayed right now.
 *
 * This is **one state, not four events**. The alternative -- `OnHoverBegin`, `OnHoverEnd`,
 * `OnSelected`, `OnDeselected` -- forces every implementer to rebuild the state from the sequence
 * of notifications, and the first odd case breaks it: hovering something already selected fires
 * `OnHoverBegin`, and the object cannot tell whether leaving should return it to "normal" or to
 * "selected".
 *
 * With a state, exactly one party decides -- `AMissionPlayerController`, the only one that knows
 * all three things at once -- and the Blueprint just draws what it is told. The precedence
 * (`Selected` > `Hovered` > `Legal` > `None`) is resolved there and never duplicated.
 *
 * The values map 1 to 1 onto the material's `HighlightState` scalar parameter.
 */
UENUM(BlueprintType)
enum class ESelectionHighlight : uint8
{
	/** Not highlighted. */
	None = 0,

	/** A legal destination of the action in progress. Painted as a set, not one by one. */
	Legal = 1,

	/** The mouse is over it. */
	Hovered = 2,

	/** It is the player's current selection. */
	Selected = 3
};

UINTERFACE(BlueprintType, MinimalAPI)
class USelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * What the mouse can pick: a `Space`, a figure, and later on a die.
 *
 * It exists so that selection **is not resolved by casting**. The `PlayerController` traces
 * against the `Selectable` channel and talks to whatever actor it hit without knowing its class:
 * that is lateral traffic, and the project's mechanism for it is the interface
 * (design/architecture/03-comunicacion-y-referencias.md).
 *
 * Everything is a `BlueprintNativeEvent`: C++ supplies a sane default and the child Blueprint
 * decides **how it looks**, which is presentation and therefore does not belong in C++
 * (design/architecture/08-presentacion-y-reglas.md).
 */
class PROJECTC_API ISelectable
{
	GENERATED_BODY()

public:
	/**
	 * Whether this can be picked right now.
	 *
	 * It does not mean "exists": a `Space` out of range is still traceable and still shows its
	 * name, but it does not get selected. Defaults to true.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	bool CanBeSelected() const;
	virtual bool CanBeSelected_Implementation() const { return true; }

	/**
	 * Display this state. The C++ default does nothing on purpose: with no Blueprint subclass
	 * there is no material to touch, and a base that assumes a mesh exists breaks on the first
	 * selectable that has none.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	void SetHighlight(ESelectionHighlight Highlight);
	virtual void SetHighlight_Implementation(ESelectionHighlight /*Highlight*/) {}

	/** For debugging and, later on, for the HUD tooltip. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	FText GetSelectableName() const;
	virtual FText GetSelectableName_Implementation() const { return FText::GetEmpty(); }
};
