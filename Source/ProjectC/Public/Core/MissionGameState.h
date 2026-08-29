// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MissionGameState.generated.h"

/**
 * El estado compartido de la misión. **Guarda, no decide.**
 *
 * Existe igual en single player: es el lugar correcto para lo que todos leen, y usarlo bien es lo
 * que deja la puerta abierta si algún día hay red. Nadie castea al GameMode para leer un dato.
 *
 * Le toca: el Doom Track, la ronda, el personaje activo, los mazos y las figuras vivas.
 * No le toca: el grafo de espacios — eso es UGraphSubsystem, un UWorldSubsystem.
 */
UCLASS()
class PROJECTC_API AMissionGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	/** Posición en el Doom Track. Sube cuando se acumulan suficientes Doom Symbol. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 DoomTrackPosition = 0;

	/** Ronda actual. Una ronda son los turnos de todos los personajes vivos. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 Round = 0;

	/** Índice del personaje que está jugando. El jugador controla los 4. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 ActiveCharacterIndex = 0;
};
