// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct PROJECTC_API FGraphEdge
{
	int32 A = INDEX_NONE;
	int32 B = INDEX_NONE;

	bool bBlocked = false;

	bool bMovementOnly = false;

	FGraphEdge() = default;
	FGraphEdge(int32 InA, int32 InB, bool bInBlocked = false, bool bInMovementOnly = false)
		: A(InA), B(InB), bBlocked(bInBlocked), bMovementOnly(bInMovementOnly) {}
};

struct PROJECTC_API FGraphQuery
{
	bool bIgnoresBlocked = false;

	bool bIncludeMovementOnly = false;

	static FGraphQuery ForDistance() { return FGraphQuery{}; }
	static FGraphQuery ForMovement() { FGraphQuery Q; Q.bIncludeMovementOnly = true; return Q; }
};

struct PROJECTC_API FGraphMath
{
	static constexpr int32 Unreachable = TNumericLimits<int32>::Max();

	static TArray<int32> Distances(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const FGraphQuery& Query);

	static int32 Distance(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query);

	static int32 Degree(const TArray<FGraphEdge>& Edges, int32 Node, const FGraphQuery& Query);

	static TArray<int32> Reachable(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 MaxSteps, const FGraphQuery& Query);

	static TArray<int32> ShortestPath(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query);

	static TArray<int32> Nearest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query);

	static TArray<int32> Farthest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query);

	static int32 PushTowards(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, int32 Steps, const FGraphQuery& Query);

	static bool EdgeApplies(const FGraphEdge& Edge, const FGraphQuery& Query);
};
