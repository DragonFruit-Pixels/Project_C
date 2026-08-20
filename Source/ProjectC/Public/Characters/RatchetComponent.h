// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RatchetComponent.generated.h"

/**
 * Configuración del track. Los defaults son los del juego original, verificados.
 *
 * El dueño de estos números es design/gdd/07-balance/perillas-y-constantes.md. Están acá como
 * default editable para que el Blueprint hijo pueda tunearlos sin recompilar; cuando exista
 * DA_MissionConfig (clase 12), esta struct se llena desde ahí.
 */
USTRUCT(BlueprintType)
struct FRatchetTrackConfig
{
	GENERATED_BODY()

	/** 20 espacios. Llegar al último es perder el personaje. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet", meta = (ClampMin = "4"))
	int32 TrackLength = 20;

	/** 6 umbrales. El espaciado acelera: 4-4-4-3-3-1, y el último queda a una casilla de la muerte. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet")
	TArray<int32> Thresholds = { 4, 8, 12, 15, 18, 19 };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRatchetAdvanced, int32, NewPosition, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThresholdCrossed, int32, ThresholdPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLost);

/**
 * El trinquete de un personaje: una barra que **solo sube**.
 *
 * Es un componente y no parte de la clase base porque la mecánica es de la figura, no de la
 * jerarquía — ver design/architecture/04-mapa-de-clases.md.
 *
 * Las reglas viven en FRatchetRules, que es puro y testeable. Este componente es el que tiene el
 * estado y avisa por delegate; no reimplementa ninguna regla.
 */
UCLASS(ClassGroup = (ProjectC), meta = (BlueprintSpawnableComponent))
class PROJECTC_API URatchetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URatchetComponent();

	/** Suma `Toll`. Es el único camino por el que el trinquete se mueve. */
	UFUNCTION(BlueprintCallable, Category = "Ratchet")
	void AddToll(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetPosition() const { return Position; }

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetThresholdsCrossedCount() const { return ThresholdsCrossedCount; }

	/** True cuando el personaje llegó al final del track. */
	UFUNCTION(BlueprintPure, Category = "Ratchet")
	bool IsLost() const;

	/** Cuánto falta para el próximo umbral, o -1 si no queda ninguno. */
	UFUNCTION(BlueprintPure, Category = "Ratchet")
	int32 GetDistanceToNextThreshold() const;

	UPROPERTY(BlueprintAssignable, Category = "Ratchet")
	FOnRatchetAdvanced OnRatchetAdvanced;

	UPROPERTY(BlueprintAssignable, Category = "Ratchet")
	FOnThresholdCrossed OnThresholdCrossed;

	UPROPERTY(BlueprintAssignable, Category = "Ratchet")
	FOnCharacterLost OnCharacterLost;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ratchet")
	FRatchetTrackConfig Config;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ratchet")
	int32 Position = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ratchet")
	int32 ThresholdsCrossedCount = 0;
};
