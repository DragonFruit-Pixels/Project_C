#pragma once

#include "CoreMinimal.h"

struct PROJECTC_API FRatchetRules
{
	static int32 Advance(int32 From, int32 Amount, int32 TrackLength);

	static TArray<int32> ThresholdsCrossed(int32 From, int32 To, const TArray<int32>& Thresholds);

	static bool IsLost(int32 Position, int32 TrackLength);
};
