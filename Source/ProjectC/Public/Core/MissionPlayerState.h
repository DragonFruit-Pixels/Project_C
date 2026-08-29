// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MissionPlayerState.generated.h"

/**
 * What happened to the **player** during this mission.
 *
 * The defining property of APlayerState is that it **outlives the Pawn's death**, and here that
 * describes something concrete: these counters stay alive when a figure is lost on the track.
 *
 * It is the right home by elimination, not as filler:
 *   - they are not mission rules      -> that is AMissionGameMode
 *   - they are not board state        -> that is AMissionGameState
 *   - they do not cross missions      -> that is UProjectCGameInstance
 * They belong to the player, and they last as long as the run does.
 *
 * They are needed regardless: the end-of-mission screen has nowhere else to get these numbers.
 *
 * See design/gdd/06-decisiones/registro.md, D-23.
 */
UCLASS()
class PROJECTC_API AMissionPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/**
	 * `Ratchet` accumulated by every figure across the whole mission.
	 *
	 * It is the metric that best sums up a run: what acting cost. It rises on `Toll` in any roll,
	 * so it grows even on the turns that went well.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 TollTaken = 0;

	/** `Reserve` spent on rerolls. Measures how hard the player fought variance. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 RerollsSpent = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 EnemiesKilled = 0;

	/** Figures that reached the end of the track. They do not come back. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 CharactersLost = 0;

	/** Edges travelled. It counts every step, so a `Move` of 3 adds 3. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 SpacesMoved = 0;
};
