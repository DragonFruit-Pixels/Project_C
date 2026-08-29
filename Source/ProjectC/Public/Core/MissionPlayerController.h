// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/Selectable.h"
#include "MissionPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ASpace;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredChanged, AActor*, HoveredActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, AActor*, SelectedActor);

/**
 * La voluntad del jugador: input, cámara y dueño de la UI.
 *
 * **El jugador no tiene cuerpo.** Conceptualmente es la cámara y quien da las órdenes, no una
 * figura del tablero: posee un `ACameraPawn` y **nunca** posee a los personajes. Las figuras se
 * eligen por raycast contra `ISelectable`, que es una sola ruta de input para figuras, `Space` y
 * dados.
 *
 * Consecuencia: todas las figuras del juego —personajes y enemigos— se mueven igual, cada una
 * por su propio AIController. Hay un solo camino de movimiento, no dos.
 *
 * **No decide reglas.** Pregunta al `AMissionGameMode` qué es legal y le pide que mueva; nunca
 * calcula alcance por su cuenta. Si un widget necesita un dato, lo lee por interfaz o lo escucha
 * por delegate; no castea a un personaje concreto.
 */
UCLASS()
class PROJECTC_API AMissionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMissionPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/**
	 * El hover se resuelve acá, un trazo por frame.
	 *
	 * Es la segunda y última excepción a "no hay Tick" del proyecto, y como la de `ACameraPawn`
	 * está del lado de la presentación: el hover no toca estado de juego. La alternativa
	 * —trazar sólo cuando el mouse se movió— ahorra un trazo contra ~10 cajas y agrega el bug de
	 * que mover la cámara con el mouse quieto deja el resaltado pegado en el espacio equivocado.
	 */
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SelectActor(AActor* NewSelection);

	UFUNCTION(BlueprintCallable, Category = "Selection")
	void ClearSelection();

	UFUNCTION(BlueprintPure, Category = "Selection")
	AActor* GetSelectedActor() const { return SelectedActor; }

	UFUNCTION(BlueprintPure, Category = "Selection")
	AActor* GetHoveredActor() const { return HoveredActor; }

	/**
	 * Avisa que cambió el hover o la selección.
	 *
	 * Son dispatchers y no llamadas directas porque el que habla no sabe quién escucha: hoy nadie,
	 * mañana el HUD, un sonido y un log de debug. Ver 03-comunicacion-y-referencias.md.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnHoveredChanged OnHoveredChanged;

	UPROPERTY(BlueprintAssignable, Category = "Selection")
	FOnSelectionChanged OnSelectionChanged;

protected:
	// --- Assets de Enhanced Input. Los enchufa BP_PlayerController_Mission. ---

	/**
	 * `EditDefaultsOnly` y no `EditAnywhere`: el mapeo de teclas es de la clase, no de la
	 * instancia. Dos controllers del mismo tipo con teclas distintas es un bug, no una feature.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MissionContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	int32 MissionContextPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraPanAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraZoomAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CameraOrbitAction;

	/**
	 * El actor bajo el cursor en el canal `Selectable`, o null.
	 *
	 * **Es el único raycast del juego**, y lo consumen las dos rutas: el hover de cada frame y el
	 * click. Que sea uno solo es lo que hace imposible que lo que se ilumina y lo que se puede
	 * clickear difieran.
	 *
	 * `BlueprintNativeEvent` a propósito. El raycast es presentación, no regla
	 * (08-presentacion-y-reglas.md), así que vive del lado de Blueprint sin violar nada: lo
	 * sobreescribe `BP_PlayerController_Mission` con un `Line Trace By Channel`, que deja el rayo,
	 * su alcance, el canal y la lista de ignorados a la vista -y dibujables con `DrawDebugType`-
	 * en lugar de escondidos dentro de un helper de C++.
	 *
	 * C++ conserva la implementación por defecto y equivalente. Si el grafo se borra o se rompe,
	 * el juego sigue respondiendo: esa red es lo que hace barato experimentar en el Blueprint.
	 *
	 * El largo del rayo es `MouseInterface > Trace Distance` (`HitResultTraceDistance`, 100000 cm),
	 * y es a propósito la del engine: la lee el grafo y la lee también
	 * `GetHitResultUnderCursorByChannel`, que es el fallback de acá abajo. Una sola perilla es lo
	 * que impide que las dos rutas se separen. Sobra para un tablero: la cámara nunca se aleja
	 * más de 4500.
	 *
	 * Se llama por el nombre pelado: en una `UCLASS` es UHT quien escribe el cuerpo de
	 * `TraceSelectableUnderCursor()`, y ese cuerpo despacha al grafo si hay override y al
	 * `_Implementation` si no. (El prefijo `Execute_` es sólo para interfaces, como `ISelectable`.)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
	AActor* TraceSelectableUnderCursor();
	virtual AActor* TraceSelectableUnderCursor_Implementation();

	// --- Handlers ---

	void HandleSelect();
	void HandleCancel();
	void HandlePan(const FInputActionValue& Value);
	void HandleZoom(const FInputActionValue& Value);
	void HandleOrbit(const FInputActionValue& Value);

private:
	/** Vuelve a preguntar al árbitro qué destinos son legales para la selección actual. */
	void RefreshLegalDestinations();

	/**
	 * Empuja el estado visual a todo lo que corresponda.
	 *
	 * Apaga todo lo que estaba prendido y vuelve a prender desde cero. Es O(n) sobre un puñado de
	 * actores y es lo que hace imposible el bug clásico de resaltado: un `Space` que queda
	 * iluminado porque el evento que lo apagaba se perdió en una rama.
	 *
	 * La precedencia sale del orden en que se aplica: `Legal`, después `Hovered`, después
	 * `Selected`. El último gana.
	 */
	void RefreshHighlights();

	UPROPERTY(Transient)
	TObjectPtr<AActor> HoveredActor;

	UPROPERTY(Transient)
	TObjectPtr<AActor> SelectedActor;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ASpace>> LegalDestinations;

	/** Lo que ahora mismo tiene un resaltado distinto de `None`, para poder apagarlo. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> HighlightedActors;
};
