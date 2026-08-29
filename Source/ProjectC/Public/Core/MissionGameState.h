// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MissionGameState.generated.h"

/**
 * The mission's shared state. **It stores, it does not decide.**
 *
 * It exists in single player all the same: it is the right home for what everyone reads, and
 * using it properly is what leaves the door open if networking ever arrives. Nobody casts to the
 * GameMode just to read a value.
 *
 * Its business: the Doom Track, the round, the active character, the decks and the live figures.
 * Not its business: the space graph -- that is UGraphSubsystem, a UWorldSubsystem.
 */
UCLASS()
class PROJECTC_API AMissionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	/** Position on the Doom Track. It rises once enough Doom Symbol have piled up. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 DoomTrackPosition = 0;

	/** Current round. A round is the turns of every living character. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 Round = 0;

	/** Index of the character currently playing. The player controls all 4. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 ActiveCharacterIndex = 0;
};
