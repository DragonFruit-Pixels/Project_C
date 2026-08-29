// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Selectable.h"
#include "Space.generated.h"

class UBoxComponent;

/**
 * A node of the graph: a `Space`.
 *
 * **Why it is an Actor and not just data.** It has to exist in the world so the mouse raycast can
 * select it (class 4), so a figure has somewhere to walk to, and so rooms can be edited in
 * separate sublevels by separate people. The *logical* graph still lives in UGraphSubsystem, not
 * scattered across the actors.
 *
 * **Walls do not exist.** A wall is the absence of an edge: if two `Space` actors do not list each
 * other as neighbours, they are not adjacent, geometry between them or not.
 *
 * Design: design/gdd/01-fundamentos/mapa-y-espacios.md
 */
UCLASS()
class PROJECTC_API ASpace : public AActor, public ISelectable
{
	GENERATED_BODY()

public:
	ASpace();

	/**
	 * The `Space` actors adjacent to this one.
	 *
	 * **Adjacency is bidirectional or it does not exist** (F1): if A lists B, B has to list A.
	 * UGraphSubsystem validates the symmetry when sealing and fails loudly if one side is
	 * missing -- a one-way arrow is a badly built map, not a directed edge.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Space")
	TArray<TObjectPtr<ASpace>> Neighbours;

	/**
	 * Passages blocked right now, as a subset of `Neighbours`.
	 *
	 * A block does not delete the edge: it leaves it impassable for whoever cannot ignore it, and
	 * a figure with that permission crosses it and does not count it when measuring distances.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Space")
	TArray<TObjectPtr<ASpace>> BlockedTowards;

	/** The volume the selection raycast hits. Collision profile `Space`: query only. */
	UFUNCTION(BlueprintPure, Category = "Space")
	UBoxComponent* GetBounds() const { return Bounds; }

	/**
	 * Where a figure standing here rests its feet: the centre of the base of `Bounds`.
	 *
	 * It exists so whoever moves a figure does not need to know how the `Space` is built. Without
	 * it, every caller rebuilds the offset by hand, and the day `BP_Space` changes the height of
	 * the box the figures sink silently.
	 */
	UFUNCTION(BlueprintPure, Category = "Space")
	FVector GetFigureAnchorLocation() const;

	// --- ISelectable ---

	virtual bool CanBeSelected_Implementation() const override { return true; }
	virtual FText GetSelectableName_Implementation() const override;

protected:
	virtual void BeginPlay() override;

	/**
	 * A selection volume, not a physics one.
	 *
	 * With the `Space` profile it is QueryOnly: it does not simulate, does not block movement and
	 * does not collide with figures -- there is no per-space figure limit, so nothing should be
	 * pushing anything.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Space")
	TObjectPtr<UBoxComponent> Bounds;
};
