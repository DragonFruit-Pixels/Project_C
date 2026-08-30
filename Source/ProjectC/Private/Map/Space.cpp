// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/Space.h"
#include "Map/GraphSubsystem.h"
#include "Core/ProjectCCollision.h"
#include "Components/BoxComponent.h"

ASpace::ASpace()
{

	PrimaryActorTick.bCanEverTick = false;

	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	SetRootComponent(Bounds);

	Bounds->SetCollisionProfileName(ProjectCCollision::SpaceProfile());

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

	const FVector Centre = Bounds->GetComponentLocation();
	return FVector(Centre.X, Centre.Y, Centre.Z - Bounds->GetScaledBoxExtent().Z);
}

FText ASpace::GetSelectableName_Implementation() const
{

	return FText::FromString(GetActorNameOrLabel());
}
