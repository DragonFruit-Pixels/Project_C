// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/GraphMath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
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
			FGraphEdge(S1, S4),
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

	const TArray<int32> Respecting = FGraphMath::Distances(NodeCount, Edges, S1, FGraphQuery::ForDistance());
	const TArray<int32> ExpectedRespecting = { 0, 3, 2, 1, 2, 1, 2 };
	for (int32 Node = 0; Node < NodeCount; ++Node)
	{
		TestEqual(FString::Printf(TEXT("respecting the block, d(S1,S%d)"), Node + 1), Respecting[Node], ExpectedRespecting[Node]);
	}

	FGraphQuery Ignoring = FGraphQuery::ForDistance();
	Ignoring.bIgnoresBlocked = true;

	const TArray<int32> WithIgnore = FGraphMath::Distances(NodeCount, Edges, S1, Ignoring);
	const TArray<int32> ExpectedIgnoring = { 0, 1, 2, 1, 2, 1, 2 };
	for (int32 Node = 0; Node < NodeCount; ++Node)
	{
		TestEqual(FString::Printf(TEXT("ignoring the block, d(S1,S%d)"), Node + 1), WithIgnore[Node], ExpectedIgnoring[Node]);
	}

	TestEqual(TEXT("d(s,s) = 0"), FGraphMath::Distance(NodeCount, Edges, S3, S3, FGraphQuery::ForDistance()), 0);

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

	TestEqual(TEXT("degree(S5) = 1"), FGraphMath::Degree(Edges, S5, FGraphQuery::ForDistance()), 1);

	FGraphQuery Ignoring = FGraphQuery::ForDistance();
	Ignoring.bIgnoresBlocked = true;

	TestEqual(TEXT("degree(S1) with the block in place = 2"), FGraphMath::Degree(Edges, S1, FGraphQuery::ForDistance()), 2);
	TestEqual(TEXT("degree(S1) for whoever ignores it = 3"), FGraphMath::Degree(Edges, S1, Ignoring), 3);
	TestEqual(TEXT("degree(S2) with the block in place = 1"), FGraphMath::Degree(Edges, S2, FGraphQuery::ForDistance()), 1);

	TArray<FGraphEdge> WithIsland = Edges;
	const int32 IslandCount = NodeCount + 1;
	const int32 Island = NodeCount;
	TestEqual(TEXT("a node with no edges is Unreachable"),
		FGraphMath::Distance(IslandCount, WithIsland, S1, Island, FGraphQuery::ForDistance()), FGraphMath::Unreachable);

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

	TestEqual(TEXT("a push of 2 with d=2 reaches the target"), FGraphMath::PushTowards(NodeCount, Edges, S3, S1, 2, Q), (int32)S1);

	const int32 Landed = FGraphMath::PushTowards(NodeCount, Edges, S2, S1, 2, Q);
	TestEqual(TEXT("a push of 2 with d=3 leaves the figure at distance 1"),
		FGraphMath::Distance(NodeCount, Edges, Landed, S1, Q), 1);

	TestEqual(TEXT("a push of 2 with d=1 ends on the target, not beyond it"),
		FGraphMath::PushTowards(NodeCount, Edges, S4, S1, 2, Q), (int32)S1);

	TArray<FGraphEdge> WithIsland = Edges;
	const int32 IslandCount = NodeCount + 1;
	TestEqual(TEXT("with no path, the push returns INDEX_NONE and the card carries on"),
		FGraphMath::PushTowards(IslandCount, WithIsland, NodeCount, S1, 2, Q), (int32)INDEX_NONE);

	const TArray<int32> Tied = FGraphMath::Nearest(NodeCount, Edges, S1, { S3, S5, S2 }, Q);
	TestEqual(TEXT("a tie at 2 returns both candidates"), Tied.Num(), 2);
	TestTrue(TEXT("the tie includes S3"), Tied.Contains(S3));
	TestTrue(TEXT("the tie includes S5"), Tied.Contains(S5));

	const TArray<int32> Single = FGraphMath::Nearest(NodeCount, Edges, S1, { S4, S3 }, Q);
	TestEqual(TEXT("with no tie it returns exactly one"), Single.Num(), 1);
	TestEqual(TEXT("and it is S4, at 1 via the stairway"), Single[0], (int32)S4);

	const TArray<int32> Within3 = FGraphMath::Reachable(NodeCount, Edges, S1, 3, Q);
	TestEqual(TEXT("with Move=3 all 7 spaces are reachable"), Within3.Num(), 7);

	return true;
}

#endif
