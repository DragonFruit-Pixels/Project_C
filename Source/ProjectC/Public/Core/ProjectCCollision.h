// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

/**
 * The project's own collision channels and profiles, given names.
 *
 * The channels are declared in Config/DefaultEngine.ini as `ECC_GameTraceChannel1..3`, which are
 * positional slots: if someone inserts a new channel, the numbers shift and every hand-written
 * trace starts hitting something else **with no compile error**. This header is the only place
 * where that correspondence lives.
 *
 * The profiles are FName and are returned from a function, not a global constant: building an
 * FName during static initialisation runs before the engine's name table exists. It is the same
 * reason the tests build their TArray inside a function.
 */
namespace ProjectCCollision
{
	/** A graph node. Object, not trace: `bTraceType=False` in the .ini. */
	inline constexpr ECollisionChannel Space = ECC_GameTraceChannel1;

	/** Character, enemy or Ally. Object. */
	inline constexpr ECollisionChannel Figure = ECC_GameTraceChannel2;

	/**
	 * The channel the mouse traces on. It is a **trace channel** (`bTraceType=True`), not an
	 * object one: it does not describe what a thing is, it describes a question asked of the world.
	 */
	inline constexpr ECollisionChannel Selectable = ECC_GameTraceChannel3;

	/** Profile for `ASpace::Bounds`: QueryOnly, blocks `Selectable` only. */
	inline FName SpaceProfile() { return FName(TEXT("Space")); }

	/** Profile for a figure's selection volume. Not the profile of its capsule. */
	inline FName FigureProfile() { return FName(TEXT("Figure")); }
}
