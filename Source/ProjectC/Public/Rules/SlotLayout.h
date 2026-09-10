#pragma once

#include "CoreMinimal.h"

struct PROJECTC_API FSlotLayout
{
	static FVector2D Offset(int32 SlotIndex, int32 SlotCount, float Spacing);

	static int32 ColumnsFor(int32 SlotCount);

	static float Span(int32 SlotCount, float Spacing);
};
