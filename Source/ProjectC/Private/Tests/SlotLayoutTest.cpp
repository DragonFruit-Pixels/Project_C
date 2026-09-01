#include "Rules/SlotLayout.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
	constexpr float Spacing = 150.0f;
	constexpr float Tolerance = 0.01f;

	FVector2D BoundsCentre(int32 SlotCount)
	{
		FVector2D Min(TNumericLimits<float>::Max(), TNumericLimits<float>::Max());
		FVector2D Max(-TNumericLimits<float>::Max(), -TNumericLimits<float>::Max());

		for (int32 Index = 0; Index < SlotCount; ++Index)
		{
			const FVector2D Slot = FSlotLayout::Offset(Index, SlotCount, Spacing);
			Min = FVector2D::Min(Min, Slot);
			Max = FVector2D::Max(Max, Slot);
		}

		return (Min + Max) * 0.5f;
	}

	float ClosestPair(int32 SlotCount)
	{
		float Closest = TNumericLimits<float>::Max();
		for (int32 A = 0; A < SlotCount; ++A)
		{
			for (int32 B = A + 1; B < SlotCount; ++B)
			{
				const float Distance = FVector2D::Distance(
					FSlotLayout::Offset(A, SlotCount, Spacing),
					FSlotLayout::Offset(B, SlotCount, Spacing));
				Closest = FMath::Min(Closest, Distance);
			}
		}
		return Closest;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FSlotLayoutTest,
	"ProjectC.Rules.Slots.Layout",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FSlotLayoutTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("a lone figure stands in the middle"),
		FSlotLayout::Offset(0, 1, Spacing), FVector2D::ZeroVector);

	TestEqual(TEXT("an index outside the count falls back to the middle"),
		FSlotLayout::Offset(4, 2, Spacing), FVector2D::ZeroVector);

	TestEqual(TEXT("two figures make one row"), FSlotLayout::ColumnsFor(2), 2);
	TestEqual(TEXT("four figures make a square"), FSlotLayout::ColumnsFor(4), 2);
	TestEqual(TEXT("nine figures make a 3x3"), FSlotLayout::ColumnsFor(9), 3);

	for (int32 Count = 1; Count <= 9; ++Count)
	{
		TestTrue(FString::Printf(TEXT("%d figures stay centred on the space"), Count),
			BoundsCentre(Count).IsNearlyZero(Tolerance));

		TArray<FVector2D> Seen;
		for (int32 Index = 0; Index < Count; ++Index)
		{
			const FVector2D Slot = FSlotLayout::Offset(Index, Count, Spacing);
			TestFalse(FString::Printf(TEXT("with %d figures, slot %d is its own"), Count, Index),
				Seen.ContainsByPredicate([&Slot](const FVector2D& Other)
					{ return Other.Equals(Slot, Tolerance); }));
			Seen.Add(Slot);
		}
	}

	for (int32 Count = 2; Count <= 9; ++Count)
	{
		TestTrue(FString::Printf(TEXT("with %d figures nobody is closer than the spacing"), Count),
			ClosestPair(Count) >= Spacing - Tolerance);
	}

	TestTrue(TEXT("four figures fit inside a 500 wide space"),
		FSlotLayout::Span(4, Spacing) + 120.0f < 500.0f);

	TestTrue(TEXT("nine figures still fit inside a 500 wide space"),
		FSlotLayout::Span(9, Spacing) + 120.0f < 500.0f);

	return true;
}

#endif
