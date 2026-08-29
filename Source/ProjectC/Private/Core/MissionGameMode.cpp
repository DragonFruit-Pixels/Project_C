// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/MissionGameMode.h"
#include "Core/MissionGameState.h"
#include "Core/MissionPlayerController.h"
#include "Core/MissionPlayerState.h"
#include "Core/CameraPawn.h"
#include "Characters/OccupancyComponent.h"
#include "Map/GraphSubsystem.h"
#include "Map/Space.h"

AMissionGameMode::AMissionGameMode()
{
	// El GameMode se enchufa desde la subclase Blueprint, no desde acá. Estos defaults son para
	// que el módulo sea coherente si alguien lo usa directo.
	GameStateClass = AMissionGameState::StaticClass();
	PlayerControllerClass = AMissionPlayerController::StaticClass();
	PlayerStateClass = AMissionPlayerState::StaticClass();

	// El jugador posee una cámara y nunca un personaje. Ver CameraPawn.h y
	// design/architecture/04-mapa-de-clases.md.
	DefaultPawnClass = ACameraPawn::StaticClass();

	bStartPlayersAsSpectators = false;
	PrimaryActorTick.bCanEverTick = false;
}

void AMissionGameMode::StartPlay()
{
	// Super::StartPlay() es lo que dispara los BeginPlay de todos los actores, y ahi es donde
	// cada ASpace se registra. Sellar antes daria un grafo incompleto, que es peor que ninguno.
	Super::StartPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGraphSubsystem* Graph = World->GetSubsystem<UGraphSubsystem>())
		{
			Graph->SealGraph();
		}
	}

	// Arranque mínimo del turno, y nada más que eso.
	//
	// La máquina de turno de verdad —rondas, rotación de personaje, las 4 fases— no existe
	// todavía: es clase 5. Sin esto `ActionsRemaining` queda en 0 y ninguna acción se puede
	// pagar, así que no habría nada que probar. Entrar a `CharacterTurn` es lo único que recarga
	// las acciones (ver SetPhase), y de ahí se pasa a `Actions`.
	SetPhase(EMissionTurnPhase::CharacterTurn);
	SetPhase(EMissionTurnPhase::Actions);
}

// Nota: `Figure` no es `const AActor*` aunque no se modifique. Blueprint no soporta parametros
// de puntero a objeto const, y esta funcion tiene que ser llamable desde el HUD.
TArray<ASpace*> AMissionGameMode::GetLegalDestinations(AActor* Figure) const
{
	TArray<ASpace*> Destinations;

	if (Figure == nullptr)
	{
		return Destinations;
	}

	const UOccupancyComponent* const Occupancy = Figure->FindComponentByClass<UOccupancyComponent>();
	if (Occupancy == nullptr || Occupancy->GetSpace() == nullptr)
	{
		return Destinations;
	}

	const UWorld* const World = GetWorld();
	const UGraphSubsystem* const Graph = World ? World->GetSubsystem<UGraphSubsystem>() : nullptr;
	if (Graph == nullptr)
	{
		return Destinations;
	}

	ASpace* const From = Occupancy->GetSpace();

	// La regla de alcance ya está escrita y testeada en FGraphMath; acá sólo se consulta.
	Destinations = Graph->GetReachable(From, SpacesPerMove);

	// `Reachable` incluye el origen porque d(a,a)=0 <= MaxSteps. Como destino no sirve.
	Destinations.Remove(From);

	return Destinations;
}

bool AMissionGameMode::TryMoveFigure(AActor* Figure, ASpace* To)
{
	if (Figure == nullptr || To == nullptr)
	{
		return false;
	}

	UOccupancyComponent* const Occupancy = Figure->FindComponentByClass<UOccupancyComponent>();
	if (Occupancy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryMoveFigure: %s no tiene UOccupancyComponent"), *Figure->GetName());
		return false;
	}

	// La legalidad se pregunta a la misma función que pinta el highlight. Es lo que garantiza que
	// lo iluminado y lo permitido no puedan diferir.
	if (!GetLegalDestinations(Figure).Contains(To))
	{
		return false;
	}

	// La acción se cobra después de validar y antes de mover: si `SpendAction` falla no quedan
	// acciones, y la figura no se movió.
	if (!SpendAction())
	{
		return false;
	}

	ASpace* const From = Occupancy->GetSpace();
	Occupancy->SetSpace(To);

	// Teletransporte, no caminata. El pathfinding es clase 9; hasta entonces el movimiento del
	// tablero es instantáneo y la animación se la debe la capa de presentación.
	const FVector Anchor = To->GetFigureAnchorLocation();
	Figure->SetActorLocation(Anchor + FVector(0.0f, 0.0f, Figure->GetSimpleCollisionHalfHeight()));

	OnFigureMoved.Broadcast(Figure, From, To);

	return true;
}

void AMissionGameMode::SetPhase(EMissionTurnPhase NewPhase)
{
	if (Phase == NewPhase)
	{
		return;
	}

	Phase = NewPhase;

	// Entrar al turno de un personaje es lo único que recarga las acciones. Ponerlo acá y no en el
	// llamador es lo que evita que un camino nuevo se olvide de recargarlas.
	if (Phase == EMissionTurnPhase::CharacterTurn)
	{
		ActionsRemaining = ActionsPerTurn;
	}

	OnPhaseChanged.Broadcast(Phase);
}

bool AMissionGameMode::SpendAction()
{
	if (ActionsRemaining <= 0)
	{
		return false;
	}

	--ActionsRemaining;
	return true;
}
