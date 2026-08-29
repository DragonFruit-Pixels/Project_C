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
 * The body of a playable character.
 *
 * It inherits from ACharacter and not APawn on purpose: that brings CharacterMovementComponent and
 * a capsule, which is what feeds the locomotion Blendspace (class 6). A bare Pawn would force the
 * movement to be written by hand.
 *
 * **The player never possesses a character.** They are picked with the mouse, like any other
 * figure on the board; the `PlayerController` possesses a camera pawn. See
 * design/architecture/04-mapa-de-clases.md, "El jugador no tiene cuerpo".
 *
 * Capabilities are components, not inheritance -- the `Ratchet` belongs to the figure, not to the
 * hierarchy. BP_Character_01..04 inherit from here and bring their mesh, values and art.
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
	 * The volume the mouse traces against, **separate from the capsule**.
	 *
	 * The capsule cannot carry the `Figure` profile: that profile ignores everything but
	 * `Selectable`, and CharacterMovementComponent sweeps the capsule against the world in order to
	 * walk. Overwriting it would leave the character with no floor, and the symptom would show up
	 * in class 6 with locomotion, a long way from the cause.
	 *
	 * It is the same pattern ASpace already uses with its `Bounds`: a component whose only job is
	 * to be traceable.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectC|Character")
	TObjectPtr<USphereComponent> SelectionBounds;
};
