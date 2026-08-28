// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/Space.h"
#include "Map/GraphSubsystem.h"
#include "Core/ProjectCCollision.h"
#include "Components/BoxComponent.h"

ASpace::ASpace()
{
	// Un juego por turnos con fases explicitas no necesita Tick: todo pasa porque una fase lo
	// disparo. Ver 01-por-donde-se-empieza.md, paso 3.
	PrimaryActorTick.bCanEverTick = false;

	Bounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds"));
	SetRootComponent(Bounds);

	// Perfil definido en Config/DefaultEngine.ini: QueryOnly, bloquea solo el canal Selectable.
	Bounds->SetCollisionProfileName(ProjectCCollision::SpaceProfile());

	// 400 uu de lado = 4 m. Un Space tiene que poder alojar 4 Character, 6 Creature y el
	// Adversary a la vez: no hay limite de ocupacion, y el problema es de layout, no de reglas.
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
	// Base de la caja, no su centro: `Bounds` esta centrado en el actor, asi que el piso del
	// espacio esta media altura mas abajo.
	const FVector Centre = Bounds->GetComponentLocation();
	return FVector(Centre.X, Centre.Y, Centre.Z - Bounds->GetScaledBoxExtent().Z);
}

FText ASpace::GetSelectableName_Implementation() const
{
	// El label del actor es lo que el diseñador ya escribio en el outliner al armar el mapa.
	return FText::FromString(GetActorNameOrLabel());
}
