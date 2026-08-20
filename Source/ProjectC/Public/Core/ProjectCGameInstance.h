// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectCGameInstance.generated.h"

/**
 * Vive todo el proceso y sobrevive los cambios de nivel.
 *
 * Le toca: la semilla del RNG, la partida en vuelo y la progresión entre misiones.
 * No le toca: ninguna regla de la misión — de eso se ocupa AMissionGameMode.
 *
 * Los servicios de este ámbito (RNG, save) van a ser UGameInstanceSubsystem, no métodos de acá.
 * Ver design/architecture/02-managers-y-subsystems.md.
 */
UCLASS()
class PROJECTC_API UProjectCGameInstance : public UGameInstance
{
	GENERATED_BODY()
};
