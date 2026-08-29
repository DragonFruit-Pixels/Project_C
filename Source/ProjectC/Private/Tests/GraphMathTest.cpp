// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/GraphMath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

/**
 * Tests for the space graph.
 *
 * They verify the acceptance criteria of design/gdd/01-fundamentos/mapa-y-espacios.md, which
 * already spells out the example graph and the two expected distance tables. No case was
 * invented: every test targets a numbered criterion of the GDD.
 */

namespace
{
	/**
	 * The F3 example graph, which the GDD reuses in F4, F5 and F6:
	 *
	 *   S1 -- S2 -- S3 -- S4 -- S5      stairway: S1 <-> S4
	 *    |                              block:    passage S1 -- S2
	 *   S6 -- S7
	 *
	 * Indices 0..6 = S1..S7.
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
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGraphDistancesTest,
	"ProjectC.Rules.Graph.Distances",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FGraphDistancesTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();

	// Criterion 3: respecting the block, S2 is at 3 because you have to go round via the stairway.
	const TArray<int32> Respecting = FGraphMath::Distances(NodeCount, Edges, S1, FGraphQuery::ForDistance());
	const TArray<int32> ExpectedRespecting = { 0, 3, 2, 1, 2, 1, 2 };
	for (int32 Node = 0; Node < NodeCount; ++Node)
	{
		TestEqual(FString::Printf(TEXT("respecting the block, d(S1,S%d)"), Node + 1), Respecting[Node], ExpectedRespecting[Node]);
	}

	// Criterion 4: the same query for a figure that ignores blocks, without mutating the graph.
	FGraphQuery Ignoring = FGraphQuery::ForDistance();
	Ignoring.bIgnoresBlocked = true;

	const TArray<int32> WithIgnore = FGraphMath::Distances(NodeCount, Edges, S1, Ignoring);
	const TArray<int32> ExpectedIgnoring = { 0, 1, 2, 1, 2, 1, 2 };
	for (int32 Node = 0; Node < NodeCount; ++Node)
	{
		TestEqual(FString::Printf(TEXT("ignoring the block, d(S1,S%d)"), Node + 1), WithIgnore[Node], ExpectedIgnoring[Node]);
	}

	// Criterion 2: the distance from a space to itself is 0.
	TestEqual(TEXT("d(s,s) = 0"), FGraphMath::Distance(NodeCount, Edges, S3, S3, FGraphQuery::ForDistance()), 0);

	// Criterion 5: removing one token of the stairway pair leaves S4 at 3 steps, not 1.
	//
	// It is measured with the S1-S2 passage open, which is the criterion's assumption: with the
	// block in place AND no stairway, S1 is shut in with S6 and S7, and S4 becomes unreachable.
	// Those are two different scenarios, and mixing them was the bug in the first version of this
	// test.
	TArray<FGraphEdge> NoStair = Edges;
	NoStair.RemoveAt(NoStair.Num() - 1);

	TestEqual(TEXT("with no stairway and the passage open, d(S1,S4) = 3"),
		FGraphMath::Distance(NodeCount, NoStair, S1, S4, Ignoring), 3);

	TestEqual(TEXT("with no stairway and the block in place, S4 is unreachable"),
		FGraphMath::Distance(NodeCount, NoStair, S1, S4, FGraphQuery::ForDistance()), FGraphMath::Unreachable);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGraphUnreachableAndDegreeTest,
	"ProjectC.Rules.Graph.UnreachableAndDegree",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FGraphUnreachableAndDegreeTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();

	// Degree is a value the cards query (Claustrophobia), not just layout.
	TestEqual(TEXT("degree(S5) = 1"), FGraphMath::Degree(Edges, S5, FGraphQuery::ForDistance()), 1);

	// **A blocked passage does not count as adjacent.** It follows from F2: Adjacent(a,b,f) is
	// defined over E_mov(f), which excludes the blocks that figure cannot cross. Gameplay
	// consequence: blocking a passage lowers the space's degree, and Claustrophobia reads it
	// differently.
	FGraphQuery Ignoring = FGraphQuery::ForDistance();
	Ignoring.bIgnoresBlocked = true;

	TestEqual(TEXT("degree(S1) with the block in place = 2"), FGraphMath::Degree(Edges, S1, FGraphQuery::ForDistance()), 2);
	TestEqual(TEXT("degree(S1) for whoever ignores it = 3"), FGraphMath::Degree(Edges, S1, Ignoring), 3);
	TestEqual(TEXT("degree(S2) with the block in place = 1"), FGraphMath::Degree(Edges, S2, FGraphQuery::ForDistance()), 1);

	// An isolated node comes back unreachable, not at distance 0 nor negative.
	TArray<FGraphEdge> WithIsland = Edges;
	const int32 IslandCount = NodeCount + 1;
	const int32 Island = NodeCount;
	TestEqual(TEXT("a node with no edges is Unreachable"),
		FGraphMath::Distance(IslandCount, WithIsland, S1, Island, FGraphQuery::ForDistance()), FGraphMath::Unreachable);

	// A Trinket edge enables movement but does NOT shorten the measured distance (F2).
	TArray<FGraphEdge> WithTrinket = Edges;
	WithTrinket.Add(FGraphEdge(S1, S7, /*bBlocked*/ false, /*bMovementOnly*/ true));

	TestEqual(TEXT("with a Trinket, moving S1->S7 costs 1"),
		FGraphMath::Distance(NodeCount, WithTrinket, S1, S7, FGraphQuery::ForMovement()), 1);
	TestEqual(TEXT("but the reported distance is still 2"),
		FGraphMath::Distance(NodeCount, WithTrinket, S1, S7, FGraphQuery::ForDistance()), 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGraphPushAndTiesTest,
	"ProjectC.Rules.Graph.PushAndTies",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FGraphPushAndTiesTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();
	const FGraphQuery Q = FGraphQuery::ForDistance();

	// F6 example A: E2 on S3, target S1, d=2, the card pushes 2 -> it arrives.
	TestEqual(TEXT("a push of 2 with d=2 reaches the target"), FGraphMath::PushTowards(NodeCount, Edges, S3, S1, 2, Q), (int32)S1);

	// F6 example B: E1 on S2, target S1, d=3 respecting the block, push 2 -> it ends at 1.
	const int32 Landed = FGraphMath::PushTowards(NodeCount, Edges, S2, S1, 2, Q);
	TestEqual(TEXT("a push of 2 with d=3 leaves the figure at distance 1"),
		FGraphMath::Distance(NodeCount, Edges, Landed, S1, Q), 1);

	// Criterion 7 / F6 example C: it has a step to spare and does NOT use it. It never overshoots.
	TestEqual(TEXT("a push of 2 with d=1 ends on the target, not beyond it"),
		FGraphMath::PushTowards(NodeCount, Edges, S4, S1, 2, Q), (int32)S1);

	// F6 example D: with no path, the step is skipped. It is not an error.
	TArray<FGraphEdge> WithIsland = Edges;
	const int32 IslandCount = NodeCount + 1;
	TestEqual(TEXT("with no path, the push returns INDEX_NONE and the card carries on"),
		FGraphMath::PushTowards(IslandCount, WithIsland, NodeCount, S1, 2, Q), (int32)INDEX_NONE);

	// Criterion 8: two candidates at the same distance return both, so the game can ask.
	const TArray<int32> Tied = FGraphMath::Nearest(NodeCount, Edges, S1, { S3, S5, S2 }, Q);
	TestEqual(TEXT("a tie at 2 returns both candidates"), Tied.Num(), 2);
	TestTrue(TEXT("the tie includes S3"), Tied.Contains(S3));
	TestTrue(TEXT("the tie includes S5"), Tied.Contains(S5));

	// A single nearest candidate does not open a selection.
	const TArray<int32> Single = FGraphMath::Nearest(NodeCount, Edges, S1, { S4, S3 }, Q);
	TestEqual(TEXT("with no tie it returns exactly one"), Single.Num(), 1);
	TestEqual(TEXT("and it is S4, at 1 via the stairway"), Single[0], (int32)S4);

	// F4: with Move = 3 from S1, even S2 falls inside the reachable set.
	const TArray<int32> Within3 = FGraphMath::Reachable(NodeCount, Edges, S1, 3, Q);
	TestEqual(TEXT("with Move=3 all 7 spaces are reachable"), Within3.Num(), 7);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
