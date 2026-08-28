// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OccupancyComponent.generated.h"

class ASpace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpaceChanged, ASpace*, OldSpace, ASpace*, NewSpace);

/**
 * En que `Space` esta parada esta figura.
 *
 * Es un componente y no un campo de la clase base porque lo necesitan tres jerarquias que no
 * comparten padre —`Character`, `Enemy`, `Ally`—, que es exactamente el criterio del proyecto
 * para componer en vez de heredar (design/architecture/04-mapa-de-clases.md).
 *
 * **Guarda la posicion, no la valida.** Si un movimiento es legal lo decide `AMissionGameMode`
 * consultando `UGraphSubsystem`; este componente obedece. Poner la validacion aca la duplicaria
 * en cada figura y la volveria imposible de testear sin mundo.
 *
 * **Primer corte deliberado.** Falta la consulta inversa —quien esta en un `Space`—, que es lo
 * que necesita el estado `Clear` (design/gdd/01-fundamentos/mapa-y-espacios.md) y la
 * pegajosidad de enemigos. Las dos son clase 5 y no entran todavia: la relacion inversa se
 * mantiene sola o se desincroniza, y sostenerla sin nadie que la use es donde aparecen los bugs.
 */
UCLASS(ClassGroup = (ProjectC), meta = (BlueprintSpawnableComponent))
class PROJECTC_API UOccupancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOccupancyComponent();

	UFUNCTION(BlueprintPure, Category = "Occupancy")
	ASpace* GetSpace() const { return CurrentSpace; }

	/** Mover no es teletransportar el Actor: eso lo hace quien llama, con la posicion del `Space`. */
	UFUNCTION(BlueprintCallable, Category = "Occupancy")
	void SetSpace(ASpace* NewSpace);

	UPROPERTY(BlueprintAssignable, Category = "Occupancy")
	FOnSpaceChanged OnSpaceChanged;

protected:
	/**
	 * `EditInstanceOnly` porque es la posicion de **esta** figura en **este** nivel: el default
	 * de la clase no tiene sentido, y dejarlo editable en el Blueprint invitaria a que las cuatro
	 * figuras arranquen en el mismo lado.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Occupancy")
	TObjectPtr<ASpace> CurrentSpace;
};
