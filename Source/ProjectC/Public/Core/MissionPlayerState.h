// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MissionPlayerState.generated.h"

/**
 * Lo que le pasó al **jugador** durante esta misión.
 *
 * La propiedad que define a APlayerState es que **sobrevive a la muerte del Pawn**, y acá eso
 * describe algo concreto: estos contadores siguen vivos cuando una figura se pierde en el track.
 *
 * Es el hogar correcto por descarte, no por relleno:
 *   - no son reglas de la misión      -> eso es AMissionGameMode
 *   - no son estado del tablero       -> eso es AMissionGameState
 *   - no cruzan de misión             -> eso es UProjectCGameInstance
 * Son del jugador, y duran lo que dura la partida.
 *
 * Se necesitan igual: la pantalla de fin de misión no tiene de dónde sacar estos números.
 *
 * Ver design/gdd/06-decisiones/registro.md, D-23.
 */
UCLASS()
class PROJECTC_API AMissionPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/**
	 * `Ratchet` acumulado por todas las figuras en toda la misión.
	 *
	 * Es la métrica que mejor resume una partida: cuánto costó actuar. Sube por `Toll` en
	 * cualquier tirada, así que crece incluso en los turnos que salieron bien.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 TollTaken = 0;

	/** `Reserve` gastado en rerolls. Mide cuánto se peleó contra la varianza. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 RerollsSpent = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 EnemiesKilled = 0;

	/** Figuras que llegaron al final del track. No se recuperan. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 CharactersLost = 0;

	/** Aristas recorridas. Cuenta cada paso, así que un `Move` de 3 suma 3. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission|Stats")
	int32 SpacesMoved = 0;
};
