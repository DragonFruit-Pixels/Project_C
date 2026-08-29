// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MissionGameMode.generated.h"

/**
 * Las fases del turno, explícitas.
 *
 * Están declaradas como dato y no implícitas en el orden de los nodos porque la secuencia del GDD
 * (design/gdd/03-resolucion/turno.md) tiene pasos obligatorios que se saltean fácil. Y porque un
 * juego por turnos con fases explícitas no necesita Tick: todo pasa porque una fase lo disparó.
 *
 * El turno de un personaje son **4 fases** y ninguna se puede saltear. La fase 4 se abre acá en
 * sus 5 pasos en vez de quedar como un solo valor: dejarlos implícitos ya costó un bug —el
 * chequeo del reloj había quedado al nivel de ronda, cuando el GDD lo pone en cada turno.
 */
UENUM(BlueprintType)
enum class EMissionTurnPhase : uint8
{
	NotStarted,

	StartOfRound,

	/** Entrar acá recarga las acciones. Una ronda son tantos CharacterTurn como personajes vivos. */
	CharacterTurn,

	/** Fase 1 — tres acciones. */
	Actions,

	/** Fase 2 — se roba 1 carta de presión y se resuelve de arriba hacia abajo. */
	PressureCard,

	/**
	 * Fase 3 — `Reckoning`. Espacio `Clear` obliga a robar recompensa; si hay enemigos, todos
	 * atacan al personaje activo. Lo que decide es el estado al **empezar** la fase, así que
	 * necesita ser una fase propia y no un paso adentro de otra: el snapshot necesita un borde.
	 */
	Reckoning,

	// Fase 4 — fin de turno. Cinco pasos, en este orden.

	/** 4.1 — efectos de fin de turno, menos los del adversario. El jugador elige el orden. */
	EndOfTurnEffects,

	/** 4.2 — un dado estándar por token de `Hazard`, como una sola tirada. Después se descartan todos. */
	Hazard,

	/**
	 * 4.3 — si hay 3 o más `Doom Symbol` en el descarte: el adversario avanza y **todo el
	 * descarte se remezcla**. Corre en cada turno, no una vez por ronda: al remezclarse el
	 * contador se resetea, así que chequear por ronda además tira el excedente acumulado.
	 */
	ClockCheck,

	/** 4.4 — chequeo de manifestación del adversario. */
	ManifestationCheck,

	/** 4.5 — efectos de fin de turno del adversario, resolviendo los stages revelados en orden. */
	AdversaryEndOfTurn,

	EndOfRound,

	Finished
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionPhaseChanged, EMissionTurnPhase, NewPhase);

class ASpace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFigureMoved, AActor*, Figure, ASpace*, From, ASpace*, To);

/**
 * El árbitro de la misión: reglas, secuencia de turno, victoria y derrota.
 *
 * **Decide, no guarda.** El estado que otros necesitan leer vive en AMissionGameState. Si algo de
 * acá tiene que ser leído por la UI o por un personaje, está en el lugar equivocado.
 *
 * Ver design/architecture/01-por-donde-se-empieza.md, paso 3.
 */
UCLASS()
class PROJECTC_API AMissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMissionGameMode();

	/**
	 * Sella el grafo apenas terminaron los `BeginPlay` de todos los `ASpace`.
	 *
	 * Quién decide que el mundo está listo es el árbitro, no el subsystem: el subsystem responde
	 * preguntas. Ver D-21.
	 */
	virtual void StartPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SetPhase(EMissionTurnPhase NewPhase);

	UFUNCTION(BlueprintPure, Category = "Mission")
	EMissionTurnPhase GetPhase() const { return Phase; }

	/** Consume una acción del turno. Devuelve false si no quedaban. */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	bool SpendAction();

	UFUNCTION(BlueprintPure, Category = "Mission")
	int32 GetActionsRemaining() const { return ActionsRemaining; }

	/**
	 * Los `Space` a los que `Figure` puede llegar gastando una acción de `Move`.
	 *
	 * Existe para que la presentación pinte los destinos **sin reimplementar la regla**. Si el
	 * HUD calculara su propio alcance, habría dos versiones de "qué es legal" y el día que una
	 * cambie el jugador vería un destino iluminado que el juego rechaza.
	 *
	 * Excluye el espacio de origen: quedarse quieto no es un movimiento, es no gastar la acción.
	 */
	UFUNCTION(BlueprintPure, Category = "Mission")
	TArray<ASpace*> GetLegalDestinations(AActor* Figure) const;

	/**
	 * Mueve una figura y cobra la acción. Devuelve false y **no cambia nada** si no era legal.
	 *
	 * Es el único camino por el que una figura cambia de `Space`. Vive acá y no en el
	 * `PlayerController` porque decidir es del árbitro: cuando la muevan los enemigos (clase 9)
	 * van a entrar por esta misma puerta, no por una segunda.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	bool TryMoveFigure(AActor* Figure, ASpace* To);

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnMissionPhaseChanged OnPhaseChanged;

	/**
	 * Una figura se movió. Lo escucha la presentación para animar.
	 *
	 * La regla no espera a que la animación termine: el movimiento ya pasó cuando esto se emite.
	 * Ver design/architecture/08-presentacion-y-reglas.md.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnFigureMoved OnFigureMoved;

protected:
	/**
	 * 3 acciones por turno.
	 *
	 * El dueño de este número es design/gdd/07-balance/perillas-y-constantes.md. Está acá como
	 * default editable y va a salir de DA_MissionConfig cuando exista (clase 12) — la costura ya
	 * está puesta para que eso sea cambiar de dónde viene el valor, no reescribir las reglas.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 ActionsPerTurn = 3;

	/**
	 * 3 espacios por acción de `Move` (manual pág. 13).
	 *
	 * Rango seguro 2–4: a 4 el kiting se vuelve viable y los enemigos pegajosos dejan de ser una
	 * amenaza. Mismo dueño y mismo destino que `ActionsPerTurn`:
	 * design/gdd/07-balance/perillas-y-constantes.md, y DA_MissionConfig cuando exista.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mission", meta = (ClampMin = "1"))
	int32 SpacesPerMove = 3;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	EMissionTurnPhase Phase = EMissionTurnPhase::NotStarted;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Mission")
	int32 ActionsRemaining = 0;
};
