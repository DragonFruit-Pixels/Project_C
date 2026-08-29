// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/GraphMath.h"
#include "Algo/Reverse.h"

namespace
{
	/**
	 * Adjacency list for one concrete query.
	 *
	 * It is built per query and deliberately not cached: the figure's permissions and the blocks
	 * change the set of edges, so a cache would be one table per combination. At the map size the
	 * GDD calls for, the cost is negligible.
	 */
	TArray<TArray<int32>> BuildAdjacency(int32 NodeCount, const TArray<FGraphEdge>& Edges, const FGraphQuery& Query)
	{
		TArray<TArray<int32>> Adjacency;
		Adjacency.SetNum(FMath::Max(NodeCount, 0));

		for (const FGraphEdge& Edge : Edges)
		{
			if (!FGraphMath::EdgeApplies(Edge, Query))
			{
				continue;
			}

			// An out-of-range edge is a badly built map, not a case to swallow silently.
			if (!Adjacency.IsValidIndex(Edge.A) || !Adjacency.IsValidIndex(Edge.B) || Edge.A == Edge.B)
			{
				continue;
			}

			Adjacency[Edge.A].AddUnique(Edge.B);
			Adjacency[Edge.B].AddUnique(Edge.A);
		}

		return Adjacency;
	}

	/** BFS that also records where each node came from, so the path can be rebuilt. */
	void BreadthFirst(const TArray<TArray<int32>>& Adjacency, int32 From, TArray<int32>& OutDistances, TArray<int32>* OutPrevious)
	{
		const int32 NodeCount = Adjacency.Num();

		OutDistances.Init(FGraphMath::Unreachable, NodeCount);
		if (OutPrevious)
		{
			OutPrevious->Init(INDEX_NONE, NodeCount);
		}

		if (!Adjacency.IsValidIndex(From))
		{
			return;
		}

		OutDistances[From] = 0;

		TArray<int32> Queue;
		Queue.Reserve(NodeCount);
		Queue.Add(From);

		for (int32 Head = 0; Head < Queue.Num(); ++Head)
		{
			const int32 Current = Queue[Head];
			for (const int32 Neighbour : Adjacency[Current])
			{
				if (OutDistances[Neighbour] != FGraphMath::Unreachable)
				{
					continue;
				}

				OutDistances[Neighbour] = OutDistances[Current] + 1;
				if (OutPrevious)
				{
					(*OutPrevious)[Neighbour] = Current;
				}
				Queue.Add(Neighbour);
			}
		}
	}

	/** Distance extremes, shared between Nearest and Farthest. */
	TArray<int32> Extremes(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query, bool bWantNearest)
	{
		TArray<int32> Distances;
		BreadthFirst(BuildAdjacency(NodeCount, Edges, Query), From, Distances, nullptr);

		int32 Best = bWantNearest ? FGraphMath::Unreachable : -1;
		TArray<int32> Winners;

		for (const int32 Candidate : Candidates)
		{
			if (!Distances.IsValidIndex(Candidate))
			{
				continue;
			}

			const int32 D = Distances[Candidate];

			// An unreachable node is not "the farthest": it drops out of the candidate set.
			if (D == FGraphMath::Unreachable)
			{
				continue;
			}

			if ((bWantNearest && D < Best) || (!bWantNearest && D > Best))
			{
				Best = D;
				Winners.Reset();
			}

			if (D == Best)
			{
				Winners.AddUnique(Candidate);
			}
		}

		return Winners;
	}
}

bool FGraphMath::EdgeApplies(const FGraphEdge& Edge, const FGraphQuery& Query)
{
	// A Trinket's conditional adjacency enables movement but is ignored when measuring.
	if (Edge.bMovementOnly && !Query.bIncludeMovementOnly)
	{
		return false;
	}

	// A block only stops whoever cannot ignore it.
	if (Edge.bBlocked && !Query.bIgnoresBlocked)
	{
		return false;
	}

	return true;
}

TArray<int32> FGraphMath::Distances(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const FGraphQuery& Query)
{
	TArray<int32> Result;
	BreadthFirst(BuildAdjacency(NodeCount, Edges, Query), From, Result, nullptr);
	return Result;
}

int32 FGraphMath::Distance(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query)
{
	const TArray<int32> All = Distances(NodeCount, Edges, From, Query);
	return All.IsValidIndex(To) ? All[To] : Unreachable;
}

int32 FGraphMath::Degree(const TArray<FGraphEdge>& Edges, int32 Node, const FGraphQuery& Query)
{
	TArray<int32> Neighbours;

	for (const FGraphEdge& Edge : Edges)
	{
		if (!EdgeApplies(Edge, Query) || Edge.A == Edge.B)
		{
			continue;
		}

		if (Edge.A == Node)
		{
			Neighbours.AddUnique(Edge.B);
		}
		else if (Edge.B == Node)
		{
			Neighbours.AddUnique(Edge.A);
		}
	}

	return Neighbours.Num();
}

TArray<int32> FGraphMath::Reachable(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 MaxSteps, const FGraphQuery& Query)
{
	const TArray<int32> All = Distances(NodeCount, Edges, From, Query);

	TArray<int32> Result;
	for (int32 Node = 0; Node < All.Num(); ++Node)
	{
		if (All[Node] != Unreachable && All[Node] <= MaxSteps)
		{
			Result.Add(Node);
		}
	}

	return Result;
}

TArray<int32> FGraphMath::ShortestPath(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query)
{
	TArray<int32> Dist;
	TArray<int32> Previous;
	BreadthFirst(BuildAdjacency(NodeCount, Edges, Query), From, Dist, &Previous);

	if (!Dist.IsValidIndex(To) || Dist[To] == Unreachable)
	{
		return TArray<int32>();
	}

	TArray<int32> Path;
	for (int32 Node = To; Node != INDEX_NONE; Node = Previous[Node])
	{
		Path.Add(Node);
		if (Node == From)
		{
			break;
		}
	}

	Algo::Reverse(Path);
	return Path;
}

TArray<int32> FGraphMath::Nearest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query)
{
	return Extremes(NodeCount, Edges, From, Candidates, Query, /*bWantNearest*/ true);
}

TArray<int32> FGraphMath::Farthest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query)
{
	return Extremes(NodeCount, Edges, From, Candidates, Query, /*bWantNearest*/ false);
}

int32 FGraphMath::PushTowards(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, int32 Steps, const FGraphQuery& Query)
{
	const TArray<int32> Path = ShortestPath(NodeCount, Edges, From, To, Query);

	// With no path, the pushing step is skipped and the card keeps resolving.
	if (Path.Num() == 0)
	{
		return INDEX_NONE;
	}

	if (Steps <= 0)
	{
		return From;
	}

	// It stops on arrival: the leftover is discarded, it does not overshoot.
	const int32 Index = FMath::Min(Steps, Path.Num() - 1);
	return Path[Index];
}
