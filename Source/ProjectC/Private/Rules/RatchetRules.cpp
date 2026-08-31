#include "Rules/RatchetRules.h"

int32 FRatchetRules::Advance(int32 From, int32 Amount, int32 TrackLength)
{
	const int32 Clamped = FMath::Max(0, From) + FMath::Max(0, Amount);
	return FMath::Min(Clamped, FMath::Max(0, TrackLength));
}

TArray<int32> FRatchetRules::ThresholdsCrossed(int32 From, int32 To, const TArray<int32>& Thresholds)
{
	TArray<int32> Crossed;
	if (To <= From)
	{
		return Crossed;
	}

	for (const int32 Threshold : Thresholds)
	{
		if (Threshold > From && Threshold <= To)
		{
			Crossed.Add(Threshold);
		}
	}

	Crossed.Sort();
	return Crossed;
}

bool FRatchetRules::IsLost(int32 Position, int32 TrackLength)
{
	return Position >= TrackLength;
}
