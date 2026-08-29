// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectCGameInstance.generated.h"

/**
 * Lives for the whole process and survives level changes.
 *
 * Its business: the RNG seed, the run in flight and progression across missions.
 * Not its business: any mission rule -- AMissionGameMode handles those.
 *
 * The services at this scope (RNG, save) will be UGameInstanceSubsystem, not methods here.
 * See design/architecture/02-managers-y-subsystems.md.
 */
UCLASS()
class PROJECTC_API UProjectCGameInstance : public UGameInstance
{
	GENERATED_BODY()
};
