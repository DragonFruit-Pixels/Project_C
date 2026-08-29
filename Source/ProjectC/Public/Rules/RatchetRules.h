// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Pure rules for the `Ratchet` track.
 *
 * It does not inherit from UObject and does not touch the world on purpose: it is the part of the
 * core mechanic that can be tested without the engine. The tests live in
 * Tests/RatchetRulesTest.cpp.
 *
 * Design: see design/gdd/02-personaje/trinquete.md
 * Numbers: design/gdd/07-balance/perillas-y-constantes.md declares them, and nobody else.
 */
struct PROJECTC_API FRatchetRules
{
	/**
	 * Advances the position and clamps it to the track length.
	 *
	 * The clamp exists because overshooting the end means nothing: reaching the end already loses
	 * the character, and a value of 23 on a track of 20 would be a state the UI cannot display.
	 */
	static int32 Advance(int32 From, int32 Amount, int32 TrackLength);

	/**
	 * The thresholds crossed on the way from `From` to `To`.
	 *
	 * The interval is **open at From and closed at To**: landing exactly on a threshold crosses
	 * it, and evaluating again from that same position does not cross it a second time. That is
	 * what prevents the bug of firing the same threshold twice, the kind of error that in a
	 * Blueprint graph only turns up while playing.
	 *
	 * Returns the thresholds in ascending order. A single advance can cross several.
	 */
	static TArray<int32> ThresholdsCrossed(int32 From, int32 To, const TArray<int32>& Thresholds);

	/** The character is lost on reaching the end of the track, not on passing it. */
	static bool IsLost(int32 Position, int32 TrackLength);
};
