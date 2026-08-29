// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Rules/RatchetRules.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

/**
 * Tests for the `Ratchet` track.
 *
 * They verify the confirmed numbers from design/gdd/07-balance/perillas-y-constantes.md: 20
 * spaces, 6 thresholds at 4/8/12/15/18/19 with gaps of 4-4-4-3-3-1, and death on reaching the end.
 *
 * This is what Blueprint could not give: the GDD's acceptance criteria stop being paragraphs.
 */

namespace
{
	/**
	 * A function and not a global constant: a file-scope TArray allocates during static
	 * initialisation, before GMalloc exists. It is one of the classic ways to break engine
	 * startup.
	 */
	TArray<int32> DefaultThresholds()
	{
		return { 4, 8, 12, 15, 18, 19 };
	}

	constexpr int32 DefaultTrackLength = 20;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRatchetThresholdsTest,
	"ProjectC.Rules.Ratchet.Thresholds",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FRatchetThresholdsTest::RunTest(const FString& Parameters)
{
	const TArray<int32> Thresholds = DefaultThresholds();

	// Advancing one at a time from 0 to the end crosses exactly 6 thresholds, in order.
	TArray<int32> Seen;
	for (int32 Position = 0; Position < DefaultTrackLength; ++Position)
	{
		Seen.Append(FRatchetRules::ThresholdsCrossed(Position, Position + 1, Thresholds));
	}

	TestEqual(TEXT("6 thresholds are crossed advancing one at a time"), Seen.Num(), 6);
	if (Seen.Num() == 6)
	{
		TestEqual(TEXT("threshold 1"), Seen[0], 4);
		TestEqual(TEXT("threshold 2"), Seen[1], 8);
		TestEqual(TEXT("threshold 3"), Seen[2], 12);
		TestEqual(TEXT("threshold 4"), Seen[3], 15);
		TestEqual(TEXT("threshold 5"), Seen[4], 18);
		TestEqual(TEXT("threshold 6"), Seen[5], 19);
	}

	// The spacing ACCELERATES: 4-4-4-3-3-1. That is what makes the end of the track hand over two
	// skill levels almost back to back and then kill.
	const int32 ExpectedGaps[] = { 4, 4, 4, 3, 3, 1 };
	int32 Previous = 0;
	for (int32 Index = 0; Index < Thresholds.Num(); ++Index)
	{
		TestEqual(
			*FString::Printf(TEXT("gap up to threshold %d"), Index + 1),
			Thresholds[Index] - Previous,
			ExpectedGaps[Index]);
		Previous = Thresholds[Index];
	}

	// The last threshold sits ONE tile from death.
	TestEqual(TEXT("the last threshold is 1 from the end"), DefaultTrackLength - Thresholds.Last(), 1);

	// A single large advance crosses every threshold it skipped over.
	const TArray<int32> AllAtOnce = FRatchetRules::ThresholdsCrossed(0, DefaultTrackLength, Thresholds);
	TestEqual(TEXT("an advance from 0 to the end crosses all 6"), AllAtOnce.Num(), 6);

	// The interval is open at From: evaluating again while standing on a threshold does not cross
	// it a second time. Without this, the same threshold would fire twice.
	TestEqual(TEXT("the threshold you are standing on is not re-crossed"),
		FRatchetRules::ThresholdsCrossed(4, 4, Thresholds).Num(), 0);

	// And it is closed at To: landing exactly on a threshold crosses it.
	TestEqual(TEXT("landing exactly on a threshold crosses it"),
		FRatchetRules::ThresholdsCrossed(3, 4, Thresholds).Num(), 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRatchetAdvanceAndLossTest,
	"ProjectC.Rules.Ratchet.AdvanceAndLoss",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FRatchetAdvanceAndLossTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("a normal advance"), FRatchetRules::Advance(0, 3, DefaultTrackLength), 3);

	// Overshooting the end means nothing: it clamps, because reaching the end already loses.
	TestEqual(TEXT("the advance clamps to the track length"),
		FRatchetRules::Advance(18, 9, DefaultTrackLength), DefaultTrackLength);

	// The character is lost ON REACHING the end, not on passing it.
	TestFalse(TEXT("tile 19 does not kill"), FRatchetRules::IsLost(19, DefaultTrackLength));
	TestTrue(TEXT("tile 20 kills"), FRatchetRules::IsLost(20, DefaultTrackLength));

	// The ratchet ONLY GOES UP: it is the property that defines the core mechanic.
	TestEqual(TEXT("a negative Toll moves nothing"), FRatchetRules::Advance(5, -3, DefaultTrackLength), 5);
	TestEqual(TEXT("no thresholds are crossed backwards"),
		FRatchetRules::ThresholdsCrossed(12, 4, DefaultThresholds()).Num(), 0);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
