// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MissionPlayerController.generated.h"

/**
 * La voluntad del jugador: input, cámara y dueño de la UI.
 *
 * Este juego es single player y el jugador controla los 4 personajes, así que la posesión rota
 * desde acá — es el encaje directo con el tema de Posesión (clase 4 del temario).
 *
 * No le tocan las reglas del juego. Si un widget necesita un dato, lo lee por interfaz o lo
 * escucha por delegate; no castea a un personaje concreto.
 */
UCLASS()
class PROJECTC_API AMissionPlayerController : public APlayerController
{
	GENERATED_BODY()
};
