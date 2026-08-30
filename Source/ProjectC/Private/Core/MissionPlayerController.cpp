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

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AMissionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI()
		.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock)
		.SetHideCursorDuringCapture(false));

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

TArray<TEnumAsByte<EObjectTypeQuery>> AMissionPlayerController::GetSpaceObjectTypes()
{
	return { UEngineTypes::ConvertToObjectType(ProjectCCollision::Space) };
}

TArray<TEnumAsByte<EObjectTypeQuery>> AMissionPlayerController::GetFigureObjectTypes()
{
	return { UEngineTypes::ConvertToObjectType(ProjectCCollision::Figure) };
}

AActor* AMissionPlayerController::TraceSelectableUnderCursor_Implementation()
{
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes =
		(InteractionMode == EInteractionMode::Move) ? GetSpaceObjectTypes() : GetFigureObjectTypes();

	FHitResult Hit;
	if (!GetHitResultUnderCursorForObjects(ObjectTypes, /*bTraceComplex*/ false, Hit))
	{
		return nullptr;
	}

	AActor* const Actor = Hit.GetActor();
	return (Actor != nullptr && Actor->Implements<USelectable>()) ? Actor : nullptr;
}

void AMissionPlayerController::HandleSelect()
{
	AActor* const Hit = TraceSelectableUnderCursor();

	UE_LOG(LogProjectCInput, Log, TEXT("Click on %s (selected: %s)"),
		Hit ? *Hit->GetName() : TEXT("nothing"),
		SelectedActor ? *SelectedActor->GetName() : TEXT("nothing"));

	if (Hit == nullptr)
	{

		ClearSelection();
		return;
	}

	ASpace* const HitSpace = Cast<ASpace>(Hit);
	if (SelectedActor != nullptr && HitSpace != nullptr && LegalDestinations.Contains(HitSpace))
	{
		if (AMissionGameMode* const GameMode = GetWorld() ? GetWorld()->GetAuthGameMode<AMissionGameMode>() : nullptr)
		{
			if (GameMode->TryMoveFigure(SelectedActor, HitSpace))
			{
				RefreshLegalDestinations();
				RefreshHighlights();
			}
		}
		return;
	}

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

	InteractionMode = LegalDestinations.IsEmpty() ? EInteractionMode::SelectFigure : EInteractionMode::Move;

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

	for (const TObjectPtr<AActor>& Actor : HighlightedActors)
	{
		if (IsValid(Actor) && Actor->Implements<USelectable>())
		{
			ISelectable::Execute_SetHighlight(Actor, ESelectionHighlight::None);
		}
	}
	HighlightedActors.Reset();

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
