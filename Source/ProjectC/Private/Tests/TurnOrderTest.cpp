#include "Core/MissionGameState.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
	AActor* MakeFigure(const TCHAR* Name)
	{
		return NewObject<AActor>(GetTransientPackage(), AActor::StaticClass(), FName(Name));
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTurnOrderTest,
	"ProjectC.Rules.Turn.Order",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FTurnOrderTest::RunTest(const FString& Parameters)
{
	AMissionGameState* const State =
		NewObject<AMissionGameState>(GetTransientPackage(), AMissionGameState::StaticClass());

	TArray<AActor*> Party;
	for (int32 Index = 0; Index < 4; ++Index)
	{
		AActor* const Figure = MakeFigure(*FString::Printf(TEXT("Figure_%02d"), Index + 1));
		Party.Add(Figure);
		State->RegisterFigure(Figure);
	}

	State->RegisterFigure(Party[0]);
	TestEqual(TEXT("registering twice does not double the party"), State->GetParty().Num(), 4);

	State->BeginRound();
	TestEqual(TEXT("the round starts at one"), State->Round, 1);
	TestEqual(TEXT("all four can act"), State->GetFiguresLeftCount(), 4);

	TestTrue(TEXT("the first pick is free"), State->SetActiveFigure(Party[2]));
	TestEqual(TEXT("that figure is active"), State->GetActiveFigure(), Party[2]);

	TestFalse(TEXT("an active turn cannot be handed to somebody else"),
		State->SetActiveFigure(Party[0]));
	TestEqual(TEXT("the active figure did not change"), State->GetActiveFigure(), Party[2]);

	TestTrue(TEXT("re-picking the figure already acting is a no-op that succeeds"),
		State->SetActiveFigure(Party[2]));

	State->RetireActiveFigure();
	TestNull(TEXT("nobody is active once the turn ends"), State->GetActiveFigure());
	TestTrue(TEXT("that figure has acted"), State->HasActedThisRound(Party[2]));
	TestEqual(TEXT("three are left"), State->GetFiguresLeftCount(), 3);

	TestFalse(TEXT("a figure that already went cannot act again this round"),
		State->SetActiveFigure(Party[2]));

	TestTrue(TEXT("the order is free: any of the rest can go next"), State->SetActiveFigure(Party[0]));
	State->RetireActiveFigure();
	TestTrue(TEXT("and the next, in any order"), State->SetActiveFigure(Party[3]));
	State->RetireActiveFigure();
	TestEqual(TEXT("one is left"), State->GetFiguresLeftCount(), 1);
	TestEqual(TEXT("the round has not turned over yet"), State->Round, 1);

	TestTrue(TEXT("the last one goes"), State->SetActiveFigure(Party[1]));
	State->RetireActiveFigure();
	TestEqual(TEXT("nobody is left"), State->GetFiguresLeftCount(), 0);

	State->BeginRound();
	TestEqual(TEXT("the next round begins"), State->Round, 2);
	TestEqual(TEXT("everyone is available again"), State->GetFiguresLeftCount(), 4);
	TestFalse(TEXT("nobody carries a spent turn into it"), State->HasActedThisRound(Party[2]));

	TestFalse(TEXT("a stranger cannot be activated"), State->SetActiveFigure(MakeFigure(TEXT("Intruder"))));
	TestNull(TEXT("and nothing became active"), State->GetActiveFigure());

	State->SetActiveFigure(Party[1]);
	State->UnregisterFigure(Party[1]);
	TestNull(TEXT("losing the active figure ends its turn"), State->GetActiveFigure());
	TestEqual(TEXT("and takes it out of the party"), State->GetParty().Num(), 3);
	TestEqual(TEXT("three can still act"), State->GetFiguresLeftCount(), 3);

	return true;
}

#endif
