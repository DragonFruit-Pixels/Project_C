// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/MissionPlayerController.h"
#include "Core/MissionGameMode.h"
#include "Core/CameraPawn.h"
#include "Core/ProjectCCollision.h"
#include "Map/Space.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectCInput, Log, All);

AMissionPlayerController::AMissionPlayerController()
{
	// Un juego de tablero se juega con el mouse a la vista. No es una preferencia: sin cursor no
	// hay hover, y sin hover el jugador no sabe qué va a clickear.
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AMissionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// GameAndUI y no GameOnly: los widgets del HUD (clase 7) tienen que poder recibir clicks sin
	// que el mundo deje de recibirlos. Cambiarlo después obliga a revisar cada widget.
	//
	// `SetHideCursorDuringCapture(false)`: GameAndUI pone al viewport en `CaptureDuringMouseDown`,
	// y por defecto esconde el cursor mientras el botón está apretado. En un juego que se juega
	// entero con el cursor eso es simplemente incorrecto: el jugador pierde de vista lo que está
	// por clickear justo en el momento en que lo clickea.
	SetInputMode(FInputModeGameAndUI()
		.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock)
		.SetHideCursorDuringCapture(false));

	// El contexto se agrega por el subsystem del LocalPlayer, no por el controller: es el punto
	// que el temario marca como servicio de Enhanced Input.
	// Ruidoso a propósito, y en las tres ramas. El síntoma de cualquiera de estas fallas es el
	// mismo -"el juego no hace nada"- y ese síntoma no dice dónde mirar. Una rama muda acá cuesta
	// una sesión entera de debug.
	const ULocalPlayer* const LocalPlayer = GetLocalPlayer();
	if (LocalPlayer == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("%s no tiene LocalPlayer en BeginPlay: no se puede agregar el contexto de input."),
			*GetName());
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* const Input =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (Input == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("No hay UEnhancedInputLocalPlayerSubsystem. Revisar que el plugin EnhancedInput este activo."));
		return;
	}

	if (MissionContext == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("%s no tiene MissionContext asignado: no va a responder ningun input. ")
			TEXT("Se asigna en el Blueprint hijo (BP_PlayerController_Mission)."),
			*GetName());
		return;
	}

	Input->AddMappingContext(MissionContext, MissionContextPriority);

	UE_LOG(LogProjectCInput, Log, TEXT("Contexto de input '%s' agregado con prioridad %d."),
		*MissionContext->GetName(), MissionContextPriority);
}

void AMissionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(InputComponent);
	if (Input == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("El InputComponent no es un UEnhancedInputComponent. Revisar DefaultInputComponentClass ")
			TEXT("en Project Settings > Input."));
		return;
	}

	// Started y no Triggered para el click: Triggered repite mientras el boton esta apretado, y
	// un click sostenido sobre un destino legal gastaria las 3 acciones de una.
	if (SelectAction != nullptr)
	{
		Input->BindAction(SelectAction, ETriggerEvent::Started, this, &AMissionPlayerController::HandleSelect);
	}
	else
	{
		UE_LOG(LogProjectCInput, Error, TEXT("SelectAction sin asignar: esa accion no responde."));
	}

	if (CancelAction != nullptr)
	{
		Input->BindAction(CancelAction, ETriggerEvent::Started, this, &AMissionPlayerController::HandleCancel);
	}
	else
	{
		UE_LOG(LogProjectCInput, Error, TEXT("CancelAction sin asignar: esa accion no responde."));
	}

	// Los tres de camara si son Triggered: valen mientras la tecla siga apretada.
	if (CameraPanAction != nullptr)
	{
		Input->BindAction(CameraPanAction, ETriggerEvent::Triggered, this, &AMissionPlayerController::HandlePan);
	}
	else
	{
		UE_LOG(LogProjectCInput, Error, TEXT("CameraPanAction sin asignar: esa accion no responde."));
	}

	if (CameraZoomAction != nullptr)
	{
		Input->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AMissionPlayerController::HandleZoom);
	}
	else
	{
		UE_LOG(LogProjectCInput, Error, TEXT("CameraZoomAction sin asignar: esa accion no responde."));
	}

	if (CameraOrbitAction != nullptr)
	{
		Input->BindAction(CameraOrbitAction, ETriggerEvent::Triggered, this, &AMissionPlayerController::HandleOrbit);
	}
	else
	{
		UE_LOG(LogProjectCInput, Error, TEXT("CameraOrbitAction sin asignar: esa accion no responde."));
	}
}

void AMissionPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	AActor* const UnderCursor = TraceSelectableUnderCursor();
	if (UnderCursor == HoveredActor)
	{
		return;
	}

	HoveredActor = UnderCursor;
	RefreshHighlights();
	OnHoveredChanged.Broadcast(HoveredActor);
}

AActor* AMissionPlayerController::TraceSelectableUnderCursor_Implementation()
{
	FHitResult Hit;

	// El canal `Selectable` existe justamente para esto: no hay que filtrar el resultado ni
	// preguntarle al actor si le interesaba, porque solo lo bloquea lo que es seleccionable.
	const bool bHit = GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ProjectCCollision::Selectable),
		/*bTraceComplex*/ false,
		Hit);

	if (!bHit)
	{
		return nullptr;
	}

	AActor* const Actor = Hit.GetActor();

	// Que bloquee el canal no alcanza: la interfaz es el contrato, el canal es solo el filtro.
	return (Actor != nullptr && Actor->Implements<USelectable>()) ? Actor : nullptr;
}

void AMissionPlayerController::HandleSelect()
{
	AActor* const Hit = TraceSelectableUnderCursor();

	// Un click es un evento del jugador, no un tick: loguearlo no cuesta nada y es lo que
	// convierte "no pasa nada" en una linea que dice por que.
	UE_LOG(LogProjectCInput, Log, TEXT("Click sobre %s (seleccionado: %s)"),
		Hit ? *Hit->GetName() : TEXT("nada"),
		SelectedActor ? *SelectedActor->GetName() : TEXT("nada"));

	if (Hit == nullptr)
	{
		// Click al vacío: deseleccionar. Es lo que espera cualquiera que haya jugado un táctico.
		ClearSelection();
		return;
	}

	// Primero, la orden de movimiento: solo si hay algo seleccionado y el destino estaba
	// iluminado. Se consulta la misma lista que se pintó, no se recalcula.
	ASpace* const HitSpace = Cast<ASpace>(Hit);
	if (SelectedActor != nullptr && HitSpace != nullptr && LegalDestinations.Contains(HitSpace))
	{
		if (AMissionGameMode* const GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AMissionGameMode>() : nullptr)
		{
			if (GameMode->TryMoveFigure(SelectedActor, HitSpace))
			{
				// La figura sigue seleccionada: gastar una de tres acciones y tener que
				// reseleccionar para las otras dos sería tedio, no decisión.
				RefreshLegalDestinations();
				RefreshHighlights();
				return;
			}
		}
	}

	// Si no era una orden, es un intento de selección.
	if (ISelectable::Execute_CanBeSelected(Hit))
	{
		SelectActor(Hit);
	}
	else
	{
		ClearSelection();
	}
}

void AMissionPlayerController::HandleCancel()
{
	ClearSelection();
}

void AMissionPlayerController::HandlePan(const FInputActionValue& Value)
{
	if (ACameraPawn* const CameraPawn = Cast<ACameraPawn>(GetPawn()))
	{
		CameraPawn->AddPanInput(Value.Get<FVector2D>());
	}
}

void AMissionPlayerController::HandleZoom(const FInputActionValue& Value)
{
	if (ACameraPawn* const CameraPawn = Cast<ACameraPawn>(GetPawn()))
	{
		CameraPawn->AddZoomInput(Value.Get<float>());
	}
}

void AMissionPlayerController::HandleOrbit(const FInputActionValue& Value)
{
	if (ACameraPawn* const CameraPawn = Cast<ACameraPawn>(GetPawn()))
	{
		CameraPawn->AddOrbitInput(Value.Get<float>());
	}
}

void AMissionPlayerController::SelectActor(AActor* NewSelection)
{
	if (SelectedActor == NewSelection)
	{
		return;
	}

	SelectedActor = NewSelection;

	RefreshLegalDestinations();
	RefreshHighlights();

	OnSelectionChanged.Broadcast(SelectedActor);
}

void AMissionPlayerController::ClearSelection()
{
	SelectActor(nullptr);
}

void AMissionPlayerController::RefreshLegalDestinations()
{
	LegalDestinations.Reset();

	if (SelectedActor == nullptr)
	{
		return;
	}

	// Quién puede moverse a dónde lo decide el árbitro. Si esto se calculara acá habría dos
	// versiones de la misma regla, y la que ve el jugador sería la equivocada.
	if (const AMissionGameMode* const GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AMissionGameMode>() : nullptr)
	{
		for (ASpace* const Destination : GameMode->GetLegalDestinations(SelectedActor))
		{
			LegalDestinations.Add(Destination);
		}
	}
}

void AMissionPlayerController::RefreshHighlights()
{
	// Apagar todo primero. Es lo que hace que no exista el resaltado huérfano.
	for (const TObjectPtr<AActor>& Actor : HighlightedActors)
	{
		if (IsValid(Actor) && Actor->Implements<USelectable>())
		{
			ISelectable::Execute_SetHighlight(Actor, ESelectionHighlight::None);
		}
	}
	HighlightedActors.Reset();

	// Y volver a prender en orden de precedencia creciente: el último que escribe, gana.
	auto Apply = [this](AActor* Actor, ESelectionHighlight Highlight)
	{
		if (IsValid(Actor) && Actor->Implements<USelectable>())
		{
			ISelectable::Execute_SetHighlight(Actor, Highlight);
			HighlightedActors.AddUnique(Actor);
		}
	};

	for (const TObjectPtr<ASpace>& Destination : LegalDestinations)
	{
		Apply(Destination, ESelectionHighlight::Legal);
	}

	Apply(HoveredActor, ESelectionHighlight::Hovered);
	Apply(SelectedActor, ESelectionHighlight::Selected);
}
