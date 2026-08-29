// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/Space.h"
#include "Map/GraphSubsystem.h"
#include "Core/ProjectCCollision.h"
#include "Components/BoxComponent.h"

ASpace::ASpace()
{
	// A turn-based game with explicit phases does not need Tick: everything happens because a
	// phase fired it. See 01-por-donde-se-empieza.md, step 3.
	PrimaryActorTick.bCanEverTick = false;

	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	SetRootComponent(Bounds);

	// Profile defined in Config/DefaultEngine.ini: QueryOnly, blocks only the Selectable channel.
	Bounds->SetCollisionProfileName(ProjectCCollision::SpaceProfile());

	// 400 uu per side = 4 m. A Space has to hold 4 Character, 6 Creature and the Adversary at
	// once: there is no occupancy limit, and the problem is layout, not rules.
	Bounds->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
}

void ASpace::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGraphSubsystem* Graph = World->GetSubsystem<UGraphSubsystem>())
		{
			Graph->RegisterSpace(this);
		}
	}
}

FVector ASpace::GetFigureAnchorLocation() const
{
	// The base of the box, not its centre: `Bounds` is centred on the actor, so the floor of the
	// space sits half a height lower.
	const FVector Centre = Bounds->GetComponentLocation();
	return FVector(Centre.X, Centre.Y, Centre.Z - Bounds->GetScaledBoxExtent().Z);
}

FText ASpace::GetSelectableName_Implementation() const
{
	// The actor label is what the designer already typed in the outliner while building the map.
	return FText::FromString(GetActorNameOrLabel());
}
