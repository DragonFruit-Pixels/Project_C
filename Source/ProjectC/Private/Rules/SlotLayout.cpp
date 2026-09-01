#include "Rules/SlotLayout.h"

int32 FSlotLayout::ColumnsFor(int32 SlotCount)
{
	return SlotCount <= 1 ? 1 : FMath::CeilToInt(FMath::Sqrt(static_cast<float>(SlotCount)));
}

FVector2D FSlotLayout::Offset(int32 SlotIndex, int32 SlotCount, float Spacing)
{
	if (SlotCount <= 1 || SlotIndex < 0 || SlotIndex >= SlotCount)
	{
		return FVector2D::ZeroVector;
	}

	const int32 Columns = ColumnsFor(SlotCount);
	const int32 Rows = FMath::DivideAndRoundUp(SlotCount, Columns);

	const int32 Row = SlotIndex / Columns;
	const int32 Column = SlotIndex % Columns;
	const int32 ColumnsInRow = FMath::Min(Columns, SlotCount - Row * Columns);

	return FVector2D(
		(static_cast<float>(Row) - 0.5f * static_cast<float>(Rows - 1)) * Spacing,
		(static_cast<float>(Column) - 0.5f * static_cast<float>(ColumnsInRow - 1)) * Spacing);
}

float FSlotLayout::Span(int32 SlotCount, float Spacing)
{
	if (SlotCount <= 1)
	{
		return 0.0f;
	}

	FVector2D Min(TNumericLimits<float>::Max(), TNumericLimits<float>::Max());
	FVector2D Max(-TNumericLimits<float>::Max(), -TNumericLimits<float>::Max());

	for (int32 Index = 0; Index < SlotCount; ++Index)
	{
		const FVector2D Slot = Offset(Index, SlotCount, Spacing);
		Min = FVector2D::Min(Min, Slot);
		Max = FVector2D::Max(Max, Slot);
	}

	return FMath::Max(Max.X - Min.X, Max.Y - Min.Y);
}
