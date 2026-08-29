// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Selectable.h"
#include "ProjectCCharacter.generated.h"

class URatchetComponent;
class UOccupancyComponent;
class USphereComponent;

/**
 * El cuerpo de un personaje jugable.
 *
 * Hereda de ACharacter y no de APawn a propósito: trae CharacterMovementComponent y cápsula, que
 * es lo que alimenta el Blendspace de locomoción (clase 6). Un Pawn pelado obligaría a escribir
 * el movimiento a mano.
 *
 * **El jugador nunca posee a un personaje.** Se los selecciona con el mouse, como a cualquier
 * otra figura del tablero; el `PlayerController` posee un pawn-cámara. Ver
 * design/architecture/04-mapa-de-clases.md, "El jugador no tiene cuerpo".
 *
 * Las capacidades son componentes, no herencia — el `Ratchet` es de la figura, no de la jerarquía.
 * Los BP_Character_01..04 heredan de acá y traen su mesh, sus valores y su arte.
 */
UCLASS()
class PROJECTC_API AProjectCCharacter : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	AProjectCCharacter();

	UFUNCTION(BlueprintPure, Category = "ProjectC|Character")
	URatchetComponent* GetRatchet() const { return Ratchet; }

	UFUNCTION(BlueprintPure, Category = "ProjectC|Character")
	UOccupancyComponent* GetOccupancy() const { return Occupancy; }

	// --- ISelectable ---

	virtual bool CanBeSelected_Implementation() const override;
	virtual FText GetSelectableName_Implementation() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<URatchetComponent> Ratchet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<UOccupancyComponent> Occupancy;

	/**
	 * El volumen que traza el mouse, **separado de la cápsula**.
	 *
	 * La cápsula no puede llevar el perfil `Figure`: ese perfil ignora todo salvo `Selectable`, y
	 * el CharacterMovementComponent barre la cápsula contra el mundo para caminar. Pisárselo
	 * dejaría al personaje sin piso, y el síntoma aparecería en la clase 6 con la locomoción, muy
	 * lejos de la causa.
	 *
	 * Es el mismo patrón que ASpace ya usa con su `Bounds`: un componente cuyo único trabajo es
	 * ser trazable.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<USphereComponent> SelectionBounds;
};
