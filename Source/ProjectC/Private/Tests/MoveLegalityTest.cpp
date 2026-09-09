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

	{
		const TArray<int32> OneStep = FGraphMath::Reachable(NodeCount, Edges, S1, 1, Movement);

		TestTrue(TEXT("at 1 step S6 is reachable"), OneStep.Contains(S6));
		TestTrue(TEXT("at 1 step S4 is reachable via the stairway"), OneStep.Contains(S4));
		TestFalse(TEXT("at 1 step S2 is NOT reachable: the passage is blocked"), OneStep.Contains(S2));
		TestFalse(TEXT("at 1 step S5 is NOT reachable: it is at 2"), OneStep.Contains(S5));

		TestTrue(TEXT("Reachable includes the origin"), OneStep.Contains(S1));

		TestEqual(TEXT("at 1 step from S1 there are 3 nodes counting the origin"), OneStep.Num(), 3);
	}

	{
		FGraphQuery Ignoring = FGraphQuery::ForMovement();
		Ignoring.bIgnoresBlocked = true;

		const TArray<int32> OneStep = FGraphMath::Reachable(NodeCount, Edges, S1, 1, Ignoring);

		TestTrue(TEXT("ignoring blocks, at 1 step S2 is reachable"), OneStep.Contains(S2));
	}

	{
		const TArray<int32> NoSteps = FGraphMath::Reachable(NodeCount, Edges, S1, 0, Movement);

		TestEqual(TEXT("with 0 steps only the origin is left"), NoSteps.Num(), 1);
		TestTrue(TEXT("and that origin is S1"), NoSteps.Contains(S1));
	}

	{
		const TArray<int32> ThreeSteps = FGraphMath::Reachable(NodeCount, Edges, S1, SpacesPerMove, Movement);

		TestEqual(TEXT("at 3 steps from S1 the whole board is reachable"), ThreeSteps.Num(), NodeCount);
		TestTrue(TEXT("S2 is reached by going round the blocked passage"), ThreeSteps.Contains(S2));
	}

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

#endif
