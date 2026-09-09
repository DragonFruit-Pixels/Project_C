// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MissionPlayerState.generated.h"

UCLASS()
class PROJECTC_API AMissionPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 TollTaken = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 RerollsSpent = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 EnemiesKilled = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 CharactersLost = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 SpacesMoved = 0;
};
