#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MissionGameState.generated.h"

UCLASS()
class PROJECTC_API AMissionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 DoomTrackPosition = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 Round = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 ActiveCharacterIndex = 0;
};
