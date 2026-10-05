#include "Rules/ProjectCRulesLibrary.h"
#include "Rules/RatchetRules.h"

int32 UProjectCRulesLibrary::RatchetAdvance(int32 From, int32 Amount, int32 TrackLength)
{
	return FRatchetRules::Advance(From, Amount, TrackLength);
}

TArray<int32> UProjectCRulesLibrary::RatchetThresholdsCrossed(int32 From, int32 To, const TArray<int32>& Thresholds)
{
	return FRatchetRules::ThresholdsCrossed(From, To, Thresholds);
}

bool UProjectCRulesLibrary::RatchetIsLost(int32 Position, int32 TrackLength)
{
	return FRatchetRules::IsLost(Position, TrackLength);
}
