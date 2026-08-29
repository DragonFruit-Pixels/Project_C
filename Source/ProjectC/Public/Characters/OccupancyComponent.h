// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OccupancyComponent.generated.h"

class ASpace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpaceChanged, ASpace*, OldSpace, ASpace*, NewSpace);

/**
 * Which `Space` this figure is standing on.
 *
 * It is a component and not a field on the base class because three hierarchies that share no
 * parent need it -- `Character`, `Enemy`, `Ally` -- which is exactly the project's criterion for
 * composing instead of inheriting (design/architecture/04-mapa-de-clases.md).
 *
 * **It stores the position, it does not validate it.** Whether a move is legal is decided by
 * `AMissionGameMode` querying `UGraphSubsystem`; this component obeys. Putting the validation here
 * would duplicate it in every figure and make it impossible to test without a world.
 *
 * **A deliberate first cut.** The reverse query -- who is on a `Space` -- is missing, and it is
 * what the `Clear` state (design/gdd/01-fundamentos/mapa-y-espacios.md) and enemy stickiness need.
 * Both are class 5 and do not come in yet: the reverse relation either maintains itself or goes
 * out of sync, and holding it up with nobody using it is where the bugs come from.
 */
UCLASS(ClassGroup = (ProjectC), meta = (BlueprintSpawnableComponent))
class PROJECTC_API UOccupancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOccupancyComponent();

	UFUNCTION(BlueprintPure, Category = "Occupancy")
	ASpace* GetSpace() const { return CurrentSpace; }

	/** Moving is not teleporting the Actor: the caller does that, using the `Space` position. */
	UFUNCTION(BlueprintCallable, Category = "Occupancy")
	void SetSpace(ASpace* NewSpace);

	UPROPERTY(BlueprintAssignable, Category = "Occupancy")
	FOnSpaceChanged OnSpaceChanged;

protected:
	/**
	 * `EditInstanceOnly` because it is **this** figure's position in **this** level: a class
	 * default makes no sense, and leaving it editable in the Blueprint would invite all four
	 * figures to start in the same place.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Occupancy")
	TObjectPtr<ASpace> CurrentSpace;
};
