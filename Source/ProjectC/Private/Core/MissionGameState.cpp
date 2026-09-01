#include "Core/MissionGameState.h"
#include "Interfaces/Selectable.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectCTurn, Log, All);

void AMissionGameState::RegisterFigure(AActor* Figure)
{
	if (Figure == nullptr)
	{
		return;
	}

	if (Party.Contains(Figure))
	{
		return;
	}

	Party.Add(Figure);

	UE_LOG(LogProjectCTurn, Log, TEXT("%s joined the party (%d figures)."),
		*Figure->GetName(), Party.Num());
}

void AMissionGameState::UnregisterFigure(AActor* Figure)
{
	Party.Remove(Figure);
	Retired.Remove(Figure);

	if (ActiveFigure == Figure)
	{
		RetireActiveFigure();
	}
}

TArray<AActor*> AMissionGameState::GetParty() const
{
	TArray<AActor*> Figures;
	Figures.Reserve(Party.Num());

	for (const TObjectPtr<AActor>& Figure : Party)
	{
		if (IsValid(Figure))
		{
			Figures.Add(Figure);
		}
	}

	return Figures;
}

bool AMissionGameState::HasActedThisRound(AActor* Figure) const
{
	return Figure != nullptr && Retired.Contains(Figure);
}

bool AMissionGameState::CanActThisRound(AActor* Figure) const
{
	if (Figure == nullptr || !Party.Contains(Figure) || Retired.Contains(Figure))
	{
		return false;
	}

	return !Figure->Implements<USelectable>() || ISelectable::Execute_CanBeSelected(Figure);
}

TArray<AActor*> AMissionGameState::GetFiguresLeftThisRound() const
{
	TArray<AActor*> Left;

	for (const TObjectPtr<AActor>& Figure : Party)
	{
		if (IsValid(Figure) && CanActThisRound(Figure))
		{
			Left.Add(Figure);
		}
	}

	return Left;
}

int32 AMissionGameState::GetFiguresLeftCount() const
{
	return GetFiguresLeftThisRound().Num();
}

bool AMissionGameState::SetActiveFigure(AActor* Figure)
{
	if (ActiveFigure == Figure)
	{
		return Figure != nullptr;
	}

	if (Figure != nullptr && !CanActThisRound(Figure))
	{
		return false;
	}

	if (ActiveFigure != nullptr && Figure != nullptr)
	{
		return false;
	}

	ActiveFigure = Figure;

	UE_LOG(LogProjectCTurn, Log, TEXT("Active figure is now %s."),
		Figure ? *Figure->GetName() : TEXT("nobody"));

	OnActiveFigureChanged.Broadcast(ActiveFigure);

	return true;
}

void AMissionGameState::RetireActiveFigure()
{
	if (ActiveFigure == nullptr)
	{
		return;
	}

	Retired.AddUnique(ActiveFigure);

	UE_LOG(LogProjectCTurn, Log, TEXT("%s is done for round %d (%d left)."),
		*ActiveFigure->GetName(), Round, GetFiguresLeftCount());

	ActiveFigure = nullptr;
	OnActiveFigureChanged.Broadcast(nullptr);
}

void AMissionGameState::BeginRound()
{
	Retired.Reset();
	++Round;

	UE_LOG(LogProjectCTurn, Log, TEXT("Round %d begins with %d figures able to act."),
		Round, GetFiguresLeftCount());

	OnRoundBegan.Broadcast(Round);
}
