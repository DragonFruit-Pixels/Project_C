#include "Map/GraphSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectCGraph, Log, All);

void UGraphSubsystem::RegisterSpace(AActor* Space, const TArray<AActor*>& Neighbours, const TArray<AActor*>& BlockedTowards)
{
	if (!IsValid(Space))
	{
		return;
	}

	if (bSealed)
	{
		UE_LOG(LogProjectCGraph, Error,
			TEXT("'%s' registered AFTER the graph was sealed. The graph is incomplete and queries will answer wrong."),
			*Space->GetName());
		return;
	}

	FSpaceLinks NewLinks;
	NewLinks.Neighbours.Append(Neighbours);
	NewLinks.BlockedTowards.Append(BlockedTowards);

	const int32 Existing = IndexOf(Space);
	if (Existing != INDEX_NONE)
	{
		Links[Existing] = MoveTemp(NewLinks);
		return;
	}

	Spaces.Add(Space);
	Links.Add(MoveTemp(NewLinks));
}

bool UGraphSubsystem::SealGraph()
{
	if (bSealed)
	{
		return true;
	}

	Edges.Reset();
	bool bValid = true;

	for (int32 Index = 0; Index < Spaces.Num(); ++Index)
	{
		AActor* Space = Spaces[Index];
		if (!IsValid(Space))
		{
			continue;
		}

		for (const TWeakObjectPtr<AActor>& NeighbourPtr : Links[Index].Neighbours)
		{
			AActor* Neighbour = NeighbourPtr.Get();
			if (!IsValid(Neighbour))
			{
				UE_LOG(LogProjectCGraph, Error, TEXT("'%s' lists a null neighbour."), *Space->GetName());
				bValid = false;
				continue;
			}

			const int32 NeighbourIndex = IndexOf(Neighbour);
			if (NeighbourIndex == INDEX_NONE)
			{
				UE_LOG(LogProjectCGraph, Error,
					TEXT("'%s' lists '%s', which is not registered. Is its room still unloaded?"),
					*Space->GetName(), *Neighbour->GetName());
				bValid = false;
				continue;
			}

			if (!Links[NeighbourIndex].Neighbours.Contains(Space))
			{
				UE_LOG(LogProjectCGraph, Error,
					TEXT("Asymmetric adjacency: '%s' lists '%s', but not the other way round."),
					*Space->GetName(), *Neighbour->GetName());
				bValid = false;
				continue;
			}

			if (NeighbourIndex == Index)
			{
				UE_LOG(LogProjectCGraph, Error, TEXT("'%s' lists itself as a neighbour."), *Space->GetName());
				bValid = false;
				continue;
			}

			if (NeighbourIndex < Index)
			{
				continue;
			}

			const bool bBlocked =
				Links[Index].BlockedTowards.Contains(Neighbour) || Links[NeighbourIndex].BlockedTowards.Contains(Space);

			Edges.Emplace(Index, NeighbourIndex, bBlocked);
		}
	}

	for (int32 Index = 0; Index < Spaces.Num(); ++Index)
	{
		const int32 Degree = FGraphMath::Degree(Edges, Index, FGraphQuery::ForMovement());
		if (Degree < 1 || Degree > 6)
		{
			UE_LOG(LogProjectCGraph, Error,
				TEXT("'%s' has degree %d; the valid range is 1..6."),
				*Spaces[Index]->GetName(), Degree);
			bValid = false;
		}
	}

	if (Spaces.Num() > 0)
	{
		FGraphQuery Unblocked = FGraphQuery::ForMovement();
		Unblocked.bIgnoresBlocked = true;

		const TArray<int32> Reach = FGraphMath::Distances(Spaces.Num(), Edges, 0, Unblocked);
		for (int32 Index = 0; Index < Reach.Num(); ++Index)
		{
			if (Reach[Index] == FGraphMath::Unreachable)
			{
				UE_LOG(LogProjectCGraph, Error,
					TEXT("'%s' is unreachable even with no blocks: the graph is split."),
					*Spaces[Index]->GetName());
				bValid = false;
			}
		}
	}

	bSealed = true;

	UE_LOG(LogProjectCGraph, Log, TEXT("Graph sealed: %d spaces, %d edges. Invariants: %s"),
		Spaces.Num(), Edges.Num(), bValid ? TEXT("OK") : TEXT("FAILED"));

	return bValid;
}

bool UGraphSubsystem::EnsureSealed(const TCHAR* Context) const
{
	if (bSealed)
	{
		return true;
	}

	UE_LOG(LogProjectCGraph, Error,
		TEXT("'%s' was queried with the graph unsealed. SealGraph() has not been called. An empty result is returned on purpose: answering over an incomplete graph would be lying."),
		Context);
	return false;
}

int32 UGraphSubsystem::IndexOf(const AActor* Space) const
{
	if (Space == nullptr)
	{
		return INDEX_NONE;
	}

	for (int32 Index = 0; Index < Spaces.Num(); ++Index)
	{
		if (Spaces[Index].Get() == Space)
		{
			return Index;
		}
	}

	return INDEX_NONE;
}

TArray<AActor*> UGraphSubsystem::ToSpaces(const TArray<int32>& Indices) const
{
	TArray<AActor*> Result;
	Result.Reserve(Indices.Num());
	for (const int32 Index : Indices)
	{
		if (Spaces.IsValidIndex(Index))
		{
			Result.Add(Spaces[Index]);
		}
	}
	return Result;
}

int32 UGraphSubsystem::GetDistance(const AActor* From, const AActor* To) const
{
	if (!EnsureSealed(TEXT("GetDistance")))
	{
		return INDEX_NONE;
	}

	const int32 A = IndexOf(From);
	const int32 B = IndexOf(To);
	if (A == INDEX_NONE || B == INDEX_NONE)
	{
		return INDEX_NONE;
	}

	const int32 D = FGraphMath::Distance(Spaces.Num(), Edges, A, B, FGraphQuery::ForDistance());
	return D == FGraphMath::Unreachable ? INDEX_NONE : D;
}

int32 UGraphSubsystem::GetDegree(const AActor* Space) const
{
	if (!EnsureSealed(TEXT("GetDegree")))
	{
		return 0;
	}

	const int32 Index = IndexOf(Space);
	return Index == INDEX_NONE ? 0 : FGraphMath::Degree(Edges, Index, FGraphQuery::ForMovement());
}

TArray<AActor*> UGraphSubsystem::GetReachable(const AActor* From, int32 MaxSteps) const
{
	if (!EnsureSealed(TEXT("GetReachable")))
	{
		return TArray<AActor*>();
	}

	const int32 A = IndexOf(From);
	if (A == INDEX_NONE)
	{
		return TArray<AActor*>();
	}

	return ToSpaces(FGraphMath::Reachable(Spaces.Num(), Edges, A, MaxSteps, FGraphQuery::ForMovement()));
}

TArray<AActor*> UGraphSubsystem::GetShortestPath(const AActor* From, const AActor* To) const
{
	if (!EnsureSealed(TEXT("GetShortestPath")))
	{
		return TArray<AActor*>();
	}

	const int32 A = IndexOf(From);
	const int32 B = IndexOf(To);
	if (A == INDEX_NONE || B == INDEX_NONE)
	{
		return TArray<AActor*>();
	}

	return ToSpaces(FGraphMath::ShortestPath(Spaces.Num(), Edges, A, B, FGraphQuery::ForDistance()));
}

TArray<AActor*> UGraphSubsystem::GetNearest(const AActor* From, const TArray<AActor*>& Candidates) const
{
	if (!EnsureSealed(TEXT("GetNearest")))
	{
		return TArray<AActor*>();
	}

	const int32 A = IndexOf(From);
	if (A == INDEX_NONE)
	{
		return TArray<AActor*>();
	}

	TArray<int32> CandidateIndices;
	CandidateIndices.Reserve(Candidates.Num());
	for (const AActor* Candidate : Candidates)
	{
		const int32 Index = IndexOf(Candidate);
		if (Index != INDEX_NONE)
		{
			CandidateIndices.Add(Index);
		}
	}

	return ToSpaces(FGraphMath::Nearest(Spaces.Num(), Edges, A, CandidateIndices, FGraphQuery::ForDistance()));
}
