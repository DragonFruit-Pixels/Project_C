// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/Selectable.h"
#include "MissionPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ASpace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredChanged, AActor*, HoveredActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, AActor*, SelectedActor);

/**
 * The player's will: input, camera and owner of the UI.
 *
 * **The player has no body.** Conceptually it is the camera and whoever gives the orders, not a
 * figure on the board: it possesses `BP_CameraPawn` and **never** possesses the characters.
 * Figures are picked by raycast against `ISelectable`, which is a single input route for figures,
 * `Space` actors and dice.
 *
 * Consequence: every figure in the game -- characters and enemies alike -- moves the same way,
 * each through its own AIController. There is one movement path, not two.
 *
 * **It decides no rules.** It asks `AMissionGameMode` what is legal and asks it to move; it never
 * computes range on its own. If a widget needs a value it reads it through an interface or
 * listens for a delegate; it does not cast to a concrete character.
 */
UCLASS()
class PROJECTC_API AMissionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMissionPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/**
	 * Hover is resolved here, one trace per frame.
	 *
	 * This is the project's second and last exception to "there is no Tick", and like the one in
	 * `BP_CameraPawn` it sits on the presentation side: hover touches no game state. The alternative
	 * -- tracing only when the mouse moved -- saves one trace against ~10 boxes and adds the bug
	 * that moving the camera with the mouse still leaves the highlight stuck on the wrong space.
	 */
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SelectActor(AActor* NewSelection);

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void ClearSelection();

	UFUNCTION(BlueprintPure, Category = "Selection")
	AActor* GetSelectedActor() const { return SelectedActor; }

	UFUNCTION(BlueprintPure, Category = "Selection")
	AActor* GetHoveredActor() const { return HoveredActor; }

	/**
	 * Announces that the hover or the selection changed.
	 *
	 * They are dispatchers and not direct calls because the speaker does not know who listens:
	 * today nobody, tomorrow the HUD, a sound and a debug log. See
	 * 03-comunicacion-y-referencias.md.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnHoveredChanged OnHoveredChanged;

	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnSelectionChanged OnSelectionChanged;

protected:
	// --- Enhanced Input assets. BP_PlayerController_Mission wires them up. ---

	/**
	 * `EditDefaultsOnly` and not `EditAnywhere`: the key mapping belongs to the class, not to the
	 * instance. Two controllers of the same type with different keys is a bug, not a feature.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MissionContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	int32 MissionContextPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CancelAction;

	/**
	 * The actor under the cursor on the `Selectable` channel, or null.
	 *
	 * **This is the game's only raycast**, and both routes consume it: the per-frame hover and the
	 * click. Having exactly one is what makes it impossible for what lights up and what can be
	 * clicked to diverge.
	 *
	 * `BlueprintNativeEvent` on purpose. The raycast is presentation, not rule
	 * (08-presentacion-y-reglas.md), so it lives on the Blueprint side without violating anything:
	 * `BP_PlayerController_Mission` overrides it with a `Line Trace By Channel`, which leaves the
	 * ray, its reach, the channel and the ignore list in plain sight -- and drawable with
	 * `DrawDebugType` -- instead of buried inside a C++ helper.
	 *
	 * C++ keeps the default, equivalent implementation. If the graph is deleted or breaks, the
	 * game keeps responding: that safety net is what makes experimenting in the Blueprint cheap.
	 *
	 * The ray's length is `MouseInterface > Trace Distance` (`HitResultTraceDistance`, 100000 cm),
	 * and it is the engine's on purpose: the graph reads it and so does
	 * `GetHitResultUnderCursorByChannel`, the fallback just below. A single knob is what stops the
	 * two routes from drifting apart. It is more than enough for a board: the camera never pulls
	 * back further than 4500.
	 *
	 * It is called by its bare name: inside a `UCLASS` it is UHT that writes the body of
	 * `TraceSelectableUnderCursor()`, and that body dispatches to the graph if there is an
	 * override and to `_Implementation` otherwise. (The `Execute_` prefix is only for interfaces,
	 * such as `ISelectable`.)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
	AActor* TraceSelectableUnderCursor();
	virtual AActor* TraceSelectableUnderCursor_Implementation();

	// --- Handlers ---

	void HandleSelect();
	void HandleCancel();

private:
	/** Asks the referee again which destinations are legal for the current selection. */
	void RefreshLegalDestinations();

	/**
	 * Pushes the visual state out to everything that needs it.
	 *
	 * It turns off everything that was on and turns it back on from scratch. It is O(n) over a
	 * handful of actors, and it is what makes the classic highlight bug impossible: a `Space` left
	 * lit because the event that would have cleared it got lost down a branch.
	 *
	 * The precedence falls out of the order it is applied in: `Legal`, then `Hovered`, then
	 * `Selected`. The last one wins.
	 */
	void RefreshHighlights();

	UPROPERTY(Transient)
	TObjectPtr<AActor> HoveredActor;

	UPROPERTY(Transient)
	TObjectPtr<AActor> SelectedActor;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ASpace>> LegalDestinations;

	/** Whatever currently has a highlight other than `None`, so it can be cleared. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> HighlightedActors;
};
