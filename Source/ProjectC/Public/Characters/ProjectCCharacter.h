// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectCCharacter.generated.h"

class URatchetComponent;

/**
 * El cuerpo de un personaje jugable.
 *
 * Hereda de ACharacter y no de APawn a propósito: trae CharacterMovementComponent y cápsula, que
 * es lo que alimenta el Blendspace de locomoción (clase 6). Un Pawn pelado obligaría a escribir
 * el movimiento a mano.
 *
 * Las capacidades son componentes, no herencia — el `Ratchet` es de la figura, no de la jerarquía.
 * Ver design/architecture/04-mapa-de-clases.md. Los BP_Character_01..04 heredan de acá y traen su
 * mesh, sus valores y su arte.
 */
UCLASS()
class PROJECTC_API AProjectCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectCCharacter();

	UFUNCTION(BlueprintPure, Category = "ProjectC|Character")
	URatchetComponent* GetRatchet() const { return Ratchet; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<URatchetComponent> Ratchet;
};
