// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Core/MissionPlayerController.h"
#include "Core/MissionGameMode.h"
#include "Core/ProjectCCollision.h"
#include "Map/Space.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectCInput, Log, All);

AMissionPlayerController::AMissionPlayerController()
{
	// A board game is played with the mouse in sight. This is not a preference: with no cursor
	// there is no hover, and with no hover the player cannot tell what they are about to click.
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AMissionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// GameAndUI and not GameOnly: the HUD widgets (class 7) have to be able to receive clicks
	// without the world losing them. Changing it later means reviewing every widget.
	//
	// `SetHideCursorDuringCapture(false)`: GameAndUI puts the viewport in `CaptureDuringMouseDown`,
	// and by default hides the cursor while the button is held. In a game played entirely with the
	// cursor that is simply wrong: the player loses sight of what they are about to click at the
	// exact moment they click it.
	SetInputMode(FInputModeGameAndUI()
		.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock)
		.SetHideCursorDuringCapture(false));

	// The context is added through the LocalPlayer's subsystem, not through the controller: that
	// is the point the syllabus marks as the Enhanced Input service.
	// Loud on purpose, in all three branches. The symptom of any of these failures is the same --
	// "the game does nothing" -- and that symptom does not say where to look. A silent branch here
	// costs a whole debugging session.
	const ULocalPlayer* const LocalPlayer = GetLocalPlayer();
	if (LocalPlayer == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("%s has no LocalPlayer in BeginPlay: the input context cannot be added."),
			*GetName());
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* const Input =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (Input == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("No UEnhancedInputLocalPlayerSubsystem. Check that the EnhancedInput plugin is enabled."));
		return;
	}

	if (MissionContext == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("%s has no MissionContext assigned: it will not respond to any input. ")
			TEXT("It is assigned in the child Blueprint (BP_PlayerController_Mission)."),
			*GetName());
		return;
	}

	Input->AddMappingContext(MissionContext, MissionContextPriority);

	UE_LOG(LogProjectCInput, Log, TEXT("Input context '%s' added with priority %d."),
		*MissionContext->GetName(), MissionContextPriority);
}

void AMissionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(InputComponent);
	if (Input == nullptr)
	{
		UE_LOG(LogProjectCInput, Error,
			TEXT("The InputComponent is not a UEnhancedInputComponent. Check DefaultInputComponentClass ")
			TEXT("in Project Settings > Input."));
		return;
	}

	// Started and not Triggered for the click: Triggered repeats while the button is held, and a
	// held click over a legal destination would spend all 3 actions at once.
	if (SelectAction != nullptr)
	{
		Input->BindAction(SelectAction, ETriggerEvent::Started, this, &AMissionPlayerController::HandleSelect);
	}
	else
	{
		UE_LOG(LogProjectCInput, Error, TEXT("SelectAction is unassigned: that action will not respond."));
	}

	if (CancelAction != nullptr)
	{
		Input->BindAction(CancelAction, ETriggerEvent::Started, this, &AMissionPlayerController::HandleCancel);
	}
	else
	{
		UE_LOG(LogProjectCInput, Error, TEXT("CancelAction is unassigned: that action will not respond."));
	}

	// The camera binds nothing here. Its three actions are heard by BP_CameraPawn itself, on the
	// pawn the player possesses, so possession and input are one thing instead of two. This
	// controller still adds the mapping context that carries them: the context is per LocalPlayer,
	// so adding it here is what makes the pawn's events fire at all.
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

	// The `Selectable` channel exists precisely for this: there is no need to filter the result or
	// ask the actor whether it cared, because only selectable things block it.
	const bool bHit = GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ProjectCCollision::Selectable),
		/*bTraceComplex*/ false,
		Hit);

	if (!bHit)
	{
		return nullptr;
	}

	AActor* const Actor = Hit.GetActor();

	// Blocking the channel is not enough: the interface is the contract, the channel is only the
	// filter.
	return (Actor != nullptr && Actor->Implements<USelectable>()) ? Actor : nullptr;
}

void AMissionPlayerController::HandleSelect()
{
	AActor* const Hit = TraceSelectableUnderCursor();

	// A click is a player event, not a tick: logging it costs nothing and is what turns "nothing
	// happens" into a line that says why.
	UE_LOG(LogProjectCInput, Log, TEXT("Click on %s (selected: %s)"),
		Hit ? *Hit->GetName() : TEXT("nothing"),
		SelectedActor ? *SelectedActor->GetName() : TEXT("nothing"));

	if (Hit == nullptr)
	{
		// Click on empty space: deselect. It is what anyone who has played a tactics game expects.
		ClearSelection();
		return;
	}

	// First, the move order: only if something is selected and the destination was lit. It queries
	// the same list that was painted; it does not recompute.
	ASpace* const HitSpace = Cast<ASpace>(Hit);
	if (SelectedActor != nullptr && HitSpace != nullptr && LegalDestinations.Contains(HitSpace))
	{
		if (AMissionGameMode* const GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AMissionGameMode>() : nullptr)
		{
			if (GameMode->TryMoveFigure(SelectedActor, HitSpace))
			{
				// The figure stays selected: spending one of three actions and having to reselect
				// for the other two would be tedium, not decision.
				RefreshLegalDestinations();
				RefreshHighlights();
				return;
			}
		}
	}

	// If it was not an order, it is a selection attempt.
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

	// Who can move where is the referee's call. If this were computed here there would be two
	// versions of the same rule, and the one the player sees would be the wrong one.
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
	// Clear everything first. That is what makes an orphaned highlight impossible.
	for (const TObjectPtr<AActor>& Actor : HighlightedActors)
	{
		if (IsValid(Actor) && Actor->Implements<USelectable>())
		{
			ISelectable::Execute_SetHighlight(Actor, ESelectionHighlight::None);
		}
	}
	HighlightedActors.Reset();

	// Then light up again in increasing order of precedence: the last writer wins.
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
