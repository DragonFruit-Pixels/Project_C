// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/GraphMath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

/**
 * The legality rule of the `Move` action.
 *
 * `AMissionGameMode::GetLegalDestinations` is what decides where a figure may go, and it decides
 * by calling `FGraphMath::Reachable` with the knobs from
 * design/gdd/07-balance/perillas-y-constantes.md: 3 spaces per `Move`.
 *
 * The GameMode needs a world and cannot be instantiated in a unit test. What is tested is **the
 * rule it queries**, which is where the logic lives: if the range is right, all that is left on
 * the GameMode's side is charging the action and dropping the origin from the list.
 *
 * It reuses the example graph from design/gdd/01-fundamentos/mapa-y-espacios.md, the same one
 * GraphMathTest.cpp already uses. No case was invented.
 */

namespace
{
	/**
	 *   S1 - S2 - S3 - S4 - S5          stairway: S1 <-> S4
	 *    |                              block:    passage S1 - S2
	 *   S6 - S7
	 */
	enum : int32 { S1 = 0, S2, S3, S4, S5, S6, S7, NodeCount };

	TArray<FGraphEdge> ExampleGraph()
	{
		return {
			FGraphEdge(S1, S2, /*bBlocked*/ true),
			FGraphEdge(S2, S3),
			FGraphEdge(S3, S4),
			FGraphEdge(S4, S5),
			FGraphEdge(S1, S6),
			FGraphEdge(S6, S7),
			FGraphEdge(S1, S4), // stairway: adjacent "for all purposes"
		};
	}

	/** The real GDD knob: 3 spaces per `Move` action (rulebook p. 13). */
	constexpr int32 SpacesPerMove = 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FMoveLegalityTest,
	"ProjectC.Rules.Move.Legality",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FMoveLegalityTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();
	const FGraphQuery Movement = FGraphQuery::ForMovement();

	// --- One step from S1 ---
	//
	// A single case verifies both of the map's odd rules at once: the S1-S2 passage is blocked, so
	// S2 does **not** make it in even though it is a neighbour; and the S1-S4 stairway is an edge
	// like any other, so S4 **does** make it in even though it is across the board.
	{
		const TArray<int32> OneStep = FGraphMath::Reachable(NodeCount, Edges, S1, 1, Movement);

		TestTrue(TEXT("at 1 step S6 is reachable"), OneStep.Contains(S6));
		TestTrue(TEXT("at 1 step S4 is reachable via the stairway"), OneStep.Contains(S4));
		TestFalse(TEXT("at 1 step S2 is NOT reachable: the passage is blocked"), OneStep.Contains(S2));
		TestFalse(TEXT("at 1 step S5 is NOT reachable: it is at 2"), OneStep.Contains(S5));

		// The origin is included because d(a,a)=0 <= MaxSteps. That is correct for "which spaces
		// can I reach", and it is exactly why GetLegalDestinations drops it afterwards: standing
		// still is not a destination, it is not spending the action.
		TestTrue(TEXT("Reachable includes the origin"), OneStep.Contains(S1));
		// {S1, S4, S6} and nothing else. The count is kept apart from the Contains on purpose: the
		// Contains verify that what should be there is there, the count verifies nothing else is.
		TestEqual(TEXT("at 1 step from S1 there are 3 nodes counting the origin"), OneStep.Num(), 3);
	}

	// --- The permission to ignore blocks belongs to the query, not to the map ---
	//
	// Same board, same tile, two different figures and two different destination sets.
	{
		FGraphQuery Ignoring = FGraphQuery::ForMovement();
		Ignoring.bIgnoresBlocked = true;

		const TArray<int32> OneStep = FGraphMath::Reachable(NodeCount, Edges, S1, 1, Ignoring);

		TestTrue(TEXT("ignoring blocks, at 1 step S2 is reachable"), OneStep.Contains(S2));
	}

	// --- With no steps there is no movement ---
	{
		const TArray<int32> NoSteps = FGraphMath::Reachable(NodeCount, Edges, S1, 0, Movement);

		TestEqual(TEXT("with 0 steps only the origin is left"), NoSteps.Num(), 1);
		TestTrue(TEXT("and that origin is S1"), NoSteps.Contains(S1));
	}

	// --- With the real knob, the BFS does not cut off early ---
	//
	// At 3 steps from S1 the whole board is reachable, S2 included: blocked head-on, but reachable
	// by going round via the stairway (S1 -> S4 -> S3 -> S2). That a block makes a space more
	// expensive rather than isolating it is the property that makes walls interesting.
	{
		const TArray<int32> ThreeSteps = FGraphMath::Reachable(NodeCount, Edges, S1, SpacesPerMove, Movement);

		TestEqual(TEXT("at 3 steps from S1 the whole board is reachable"), ThreeSteps.Num(), NodeCount);
		TestTrue(TEXT("S2 is reached by going round the blocked passage"), ThreeSteps.Contains(S2));
	}

	// --- Range grows with the steps, it never shrinks ---
	//
	// This is the property that makes raising the knob from 3 to 4 unable to remove destinations.
	// Cheap to verify, and the one that breaks if someone "optimises" the BFS with a misplaced
	// prune.
	{
		int32 Previous = 0;
		for (int32 Steps = 0; Steps <= NodeCount; ++Steps)
		{
			const int32 Count = FGraphMath::Reachable(NodeCount, Edges, S1, Steps, Movement).Num();
			TestTrue(
				FString::Printf(TEXT("the range at %d steps is not smaller than at %d"), Steps, Steps - 1),
				Count >= Previous);
			Previous = Count;
		}
	}

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
