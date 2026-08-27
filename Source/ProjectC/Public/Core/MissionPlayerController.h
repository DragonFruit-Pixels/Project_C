// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MissionPlayerController.generated.h"

/**
 * La voluntad del jugador: input, cámara y dueño de la UI.
 *
 * **El jugador no tiene cuerpo.** Conceptualmente es la cámara y quien da las órdenes, no una
 * figura del tablero: posee un pawn-cámara y **nunca** posee a los personajes. Las figuras se
 * eligen por raycast contra ISelectable, que es una sola ruta de input para figuras, `Space` y
 * dados.
 *
 * Consecuencia: todas las figuras del juego —personajes y enemigos— se mueven igual, cada una
 * por su propio AIController. Hay un solo camino de movimiento, no dos.
 *
 * No le tocan las reglas del juego. Si un widget necesita un dato, lo lee por interfaz o lo
 * escucha por delegate; no castea a un personaje concreto.
 */
UCLASS()
class PROJECTC_API AMissionPlayerController : public APlayerController
{
	GENERATED_BODY()
};
