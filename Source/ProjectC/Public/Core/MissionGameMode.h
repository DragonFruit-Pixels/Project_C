// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MissionGameMode.generated.h"

/**
 * The turn phases, made explicit.
 *
 * They are declared as data instead of being implicit in node order because the GDD's sequence
 * (design/gdd/03-resolucion/turno.md) has mandatory steps that are easy to skip. And because a
 * turn-based game with explicit phases does not need Tick: everything happens because a phase
 * fired it.
 *
 * A character's turn is **4 phases** and none of them can be skipped. Phase 4 is opened up here
 * into its 5 steps instead of staying a single value: leaving them implicit already cost a bug --
 * the clock check had ended up at round level, when the GDD puts it in every turn.
 */
UENUM(BlueprintType)
enum class EMissionTurnPhase : uint8
{
	NotStarted,

	StartOfRound,

	/** Entering here refills the actions. A round is as many CharacterTurn as living characters. */
	CharacterTurn,

	/** Phase 1 -- three actions. */
	Actions,

	/** Phase 2 -- draw 1 pressure card and resolve it top to bottom. */
	PressureCard,

	/**
	 * Phase 3 -- `Reckoning`. A `Clear` space forces a reward draw; if there are enemies, all of
	 * them attack the active character. What decides is the state at the **start** of the phase,
	 * so it needs to be a phase of its own rather than a step inside another one: the snapshot
	 * needs an edge.
	 */
	Reckoning,

	// Phase 4 -- end of turn. Five steps, in this order.

	/** 4.1 -- end-of-turn effects, the adversary's aside. The player chooses the order. */
	EndOfTurnEffects,

	/** 4.2 -- one standard die per `Hazard` token, as a single roll. Then all of them are discarded. */
	Hazard,

	/**
	 * 4.3 -- if there are 3 or more `Doom Symbol` in the discard: the adversary advances and
	 * **the whole discard is reshuffled**. It runs every turn, not once per round: reshuffling
	 * resets the counter, so checking per round would also throw away the accumulated surplus.
	 */
	ClockCheck,

	/** 4.4 -- the adversary's manifestation check. */
	ManifestationCheck,

	/** 4.5 -- the adversary's end-of-turn effects, resolving the revealed stages in order. */
	AdversaryEndOfTurn,

	EndOfRound,

	Finished
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionPhaseChanged, EMissionTurnPhase, NewPhase);

class ASpace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFigureMoved, AActor*, Figure, ASpace*, From, ASpace*, To);

/**
 * The mission's referee: rules, turn sequence, victory and defeat.
 *
 * **It decides, it does not store.** The state others need to read lives in AMissionGameState. If
 * something here has to be read by the UI or by a character, it is in the wrong place.
 *
 * See design/architecture/01-por-donde-se-empieza.md, step 3.
 */
UCLASS()
class PROJECTC_API AMissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMissionGameMode();

	/**
	 * Seals the graph as soon as every `ASpace` has finished its `BeginPlay`.
	 *
	 * Deciding that the world is ready is the referee's call, not the subsystem's: the subsystem
	 * answers questions. See D-21.
	 */
	virtual void StartPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SetPhase(EMissionTurnPhase NewPhase);

	UFUNCTION(BlueprintPure, Category = "Mission")
	EMissionTurnPhase GetPhase() const { return Phase; }

	/** Consumes one action of the turn. Returns false if there were none left. */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	bool SpendAction();

	UFUNCTION(BlueprintPure, Category = "Mission")
	int32 GetActionsRemaining() const { return ActionsRemaining; }

	/**
	 * The `Space` actors `Figure` can reach by spending one `Move` action.
	 *
	 * It exists so presentation can paint the destinations **without reimplementing the rule**. If
	 * the HUD computed its own range there would be two versions of "what is legal", and the day
	 * one of them changed the player would see a highlighted destination the game rejects.
	 *
	 * It excludes the origin space: standing still is not a move, it is not spending the action.
	 */
	UFUNCTION(BlueprintPure, Category = "Mission")
	TArray<ASpace*> GetLegalDestinations(AActor* Figure) const;

	/**
	 * Moves a figure and charges the action. Returns false and **changes nothing** if it was not
	 * legal.
	 *
	 * It is the only route by which a figure changes `Space`. It lives here and not in the
	 * `PlayerController` because deciding is the referee's job: when the enemies move it
	 * (class 9) they will come through this same door, not a second one.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	bool TryMoveFigure(AActor* Figure, ASpace* To);

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnMissionPhaseChanged OnPhaseChanged;

	/**
	 * A figure moved. Presentation listens to it in order to animate.
	 *
	 * The rule does not wait for the animation to finish: the move has already happened by the
	 * time this is broadcast. See design/architecture/08-presentacion-y-reglas.md.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnFigureMoved OnFigureMoved;

protected:
	/**
	 * 3 actions per turn.
	 *
	 * The owner of this number is design/gdd/07-balance/perillas-y-constantes.md. It sits here as
	 * an editable default and will come from DA_MissionConfig once that exists (class 12) -- the
	 * seam is already in place so that becomes a change of where the value comes from, not a
	 * rewrite of the rules.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 ActionsPerTurn = 3;

	/**
	 * 3 spaces per `Move` action (rulebook p. 13).
	 *
	 * Safe range 2-4: at 4 kiting becomes viable and sticky enemies stop being a threat. Same
	 * owner and same destination as `ActionsPerTurn`:
	 * design/gdd/07-balance/perillas-y-constantes.md, and DA_MissionConfig once it exists.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 SpacesPerMove = 3;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	EMissionTurnPhase Phase = EMissionTurnPhase::NotStarted;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 ActionsRemaining = 0;
};
