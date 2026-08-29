// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Characters/RatchetComponent.h"
#include "Rules/RatchetRules.h"

URatchetComponent::URatchetComponent()
{
	// A turn-based game has nothing to tick: everything happens because a phase fired it.
	PrimaryComponentTick.bCanEverTick = false;
}

void URatchetComponent::AddToll(int32 Amount)
{
	if (Amount <= 0 || IsLost())
	{
		return;
	}

	const int32 Previous = Position;
	Position = FRatchetRules::Advance(Previous, Amount, Config.TrackLength);

	const int32 Delta = Position - Previous;
	if (Delta <= 0)
	{
		return;
	}

	OnRatchetAdvanced.Broadcast(Position, Delta);

	// A single advance can cross several thresholds, and each one is a build decision of its own.
	for (const int32 Threshold : FRatchetRules::ThresholdsCrossed(Previous, Position, Config.Thresholds))
	{
		++ThresholdsCrossedCount;
		OnThresholdCrossed.Broadcast(Threshold);
	}

	if (IsLost())
	{
		OnCharacterLost.Broadcast();
	}
}

bool URatchetComponent::IsLost() const
{
	return FRatchetRules::IsLost(Position, Config.TrackLength);
}

int32 URatchetComponent::GetDistanceToNextThreshold() const
{
	int32 Best = -1;
	for (const int32 Threshold : Config.Thresholds)
	{
		if (Threshold > Position && (Best == -1 || Threshold - Position < Best))
		{
			Best = Threshold - Position;
		}
	}
	return Best;
}
