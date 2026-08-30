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

class ASpace;

/**
 * The mission's referee: rules, turn sequence, victory and defeat.
 *
 * **It decides, it does not store.** The state others need to read lives in AMissionGameState. If
 * something here has to be read by the UI or by a character, it is in the wrong place.
 *
 * **The turn itself lives in BP_GameMode_Mission**, not here. Gameplay Framework is class 3 of the
 * syllabus and C++ is class 14, so a turn machine written in C++ is written where nobody is going
 * to look for it (see design/course-alignment.md, and D-30 for the same move on the camera). What
 * stays below is the part a graph cannot express safely.
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
	 * Seals the graph once every `ASpace` has finished its `BeginPlay`, then hands the turn over
	 * to the Blueprint.
	 *
	 * **This is the one thing that cannot move to the graph.** A Blueprint's `BeginPlay` fires
	 * from inside `Super::StartPlay()`, together with every other actor's and in no guaranteed
	 * order, so sealing from there would seal a graph that some `ASpace` had not registered with
	 * yet. Sealing after `Super::StartPlay()` returns is what makes the graph complete, and
	 * deciding that the world is ready is the referee's call rather than the subsystem's -- the
	 * subsystem answers questions. See D-21.
	 */
	virtual void StartPlay() override;

	/**
	 * The graph is sealed and the mission can begin. `BP_GameMode_Mission` starts the turn here.
	 *
	 * It exists so the Blueprint gets an entry point with the ordering guarantee above already
	 * kept. Using `BeginPlay` for this instead would be the bug the guarantee prevents.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Mission")
	void OnMissionReady();

	/**
	 * The `Space` actors `Figure` can reach by spending one `Move` action.
	 *
	 * Stays in C++ because it is a **rule**, not framework: it delegates to `FGraphMath::Reachable`,
	 * which is what `ProjectC.Rules.Move.Legality` covers. Presentation paints destinations by
	 * calling this instead of reimplementing the range, so there is never a second version of
	 * "what is legal" for a highlighted destination to disagree with.
	 *
	 * It excludes the origin space: standing still is not a move, it is not spending the action.
	 */
	UFUNCTION(BlueprintPure, Category = "Mission")
	TArray<ASpace*> GetLegalDestinations(AActor* Figure) const;

	/**
	 * Moves a figure and charges the action. Returns false and **changes nothing** if it was not
	 * legal.
	 *
	 * It is the only route by which a figure changes `Space`. It belongs to the referee and not to
	 * the `PlayerController` because when the enemies move (class 9) they will come through this
	 * same door, not a second one.
	 *
	 * `BlueprintNativeEvent`, and `BP_GameMode_Mission` is what implements it. Unlike
	 * `TraceSelectableUnderCursor`, whose C++ body is a **working** fallback, the body below only
	 * logs and refuses: the action counter it would have to charge now lives in the Blueprint, and
	 * a C++ copy of that counter would be a second source of truth that nothing refills. A seam
	 * that fails loudly beats a safety net that quietly disagrees with the game.
	 *
	 * The seam is temporary. It exists so `AMissionPlayerController` keeps working while it is
	 * still C++, and goes away with it.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mission")
	bool TryMoveFigure(AActor* Figure, ASpace* To);
	virtual bool TryMoveFigure_Implementation(AActor* Figure, ASpace* To);

	/**
	 * The turn phase. **Storage, not machinery**: what drives it is `SetPhase` in
	 * BP_GameMode_Mission, together with the action counter and the dispatchers.
	 *
	 * It stays here for the same reason `EMissionTurnPhase` does -- the 13 phases carry the GDD's
	 * sequence in their own documentation, and the value belongs next to the type that explains
	 * it. `BlueprintReadWrite` because the graph is the only writer: there is no setter here to
	 * enforce anything, since what has to be enforced on a phase change -- refilling the actions,
	 * firing `OnPhaseChanged` -- is the graph's job now.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Mission")
	EMissionTurnPhase Phase = EMissionTurnPhase::NotStarted;

protected:
	/**
	 * 3 actions per turn.
	 *
	 * Here and not in the Blueprint because it is one of a pair: this and `SpacesPerMove` are the
	 * two balance knobs, they share an owner in design/gdd/07-balance/perillas-y-constantes.md and
	 * they share a destination in DA_MissionConfig (class 12). Splitting them across two languages
	 * bought nothing -- `EditDefaultsOnly` is edited in the details panel exactly like a Blueprint
	 * variable -- and cost the default: a Blueprint variable is born at 0, and a turn with 0
	 * actions is a turn where nothing can move, with nothing in the log to say why.
	 *
	 * The counter it refills, `ActionsRemaining`, does live in the graph: that one is the turn
	 * machine's state, and 0 is the right value for it to start at.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 ActionsPerTurn = 3;

	/**
	 * 3 spaces per `Move` action (rulebook p. 13).
	 *
	 * Safe range 2-4: at 4 kiting becomes viable and sticky enemies stop being a threat. It sits
	 * here rather than in the Blueprint because `GetLegalDestinations` is its only reader. The
	 * owner of the number is design/gdd/07-balance/perillas-y-constantes.md, and it will come from
	 * DA_MissionConfig once that exists (class 12).
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 SpacesPerMove = 3;
};
