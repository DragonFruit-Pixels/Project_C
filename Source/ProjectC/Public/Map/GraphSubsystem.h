// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Map/GraphMath.h"
#include "GraphSubsystem.generated.h"

class ASpace;

/**
 * The level's space graph: adjacency, distances and degree.
 *
 * It is a `UWorldSubsystem` and not `GameState` state because **it answers questions, it does not
 * decide**, and because its lifetime is exactly the level's. See 02-managers-y-subsystems.md.
 *
 * **Sealing is not ceremony.** Every ASpace registers itself in its BeginPlay, so until all of
 * them are done the graph is incomplete -- and a BFS over an incomplete graph **does not crash:
 * it answers wrong**. It reports "no path" where there was one, or a longer one. In a game where
 * the shortest path decides where the enemies move, lying is worse than crashing.
 *
 * That is why every query before sealing fails loudly and returns an empty result.
 * Recorded as D-21 in design/gdd/06-decisiones/registro.md.
 */
UCLASS()
class PROJECTC_API UGraphSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Registers a `Space`. ASpace::BeginPlay calls it; no manual ordering required. */
	void RegisterSpace(ASpace* Space);

	/**
	 * Closes registration, builds the edges from each `Space`'s neighbours and runs the
	 * invariants. From here on, queries answer.
	 *
	 * The `GameMode` calls it when the mission starts, not the subsystem itself: deciding that
	 * the world is ready is the referee's call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool SealGraph();

	UFUNCTION(BlueprintPure, Category = "Graph")
	bool IsSealed() const { return bSealed; }

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetSpaceCount() const { return Spaces.Num(); }

	/** `INDEX_NONE` if there is no path, or if the graph is not sealed yet. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDistance(const ASpace* From, const ASpace* To) const;

	/** How many spaces are adjacent. The cards query it, not just the layout. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDegree(const ASpace* Space) const;

	/** The `Space` actors reachable by spending up to `MaxSteps` steps. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetReachable(const ASpace* From, int32 MaxSteps) const;

	/** A shortest path, both endpoints included. Empty if there is none. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetShortestPath(const ASpace* From, const ASpace* To) const;

	/**
	 * The candidates tied at minimum distance.
	 *
	 * If it returns more than one, **the game has to ask**: the rulebook does not allow an
	 * automatic tiebreak. If it returns exactly one, no selection is opened.
	 */
	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetNearest(const ASpace* From, const TArray<ASpace*>& Candidates) const;

	/** Internal index of a `Space`, or `INDEX_NONE`. Useful for tests and debugging. */
	int32 IndexOf(const ASpace* Space) const;

private:
	/** Logs the breach and returns false if the graph is not sealed yet. */
	bool EnsureSealed(const TCHAR* Context) const;

	TArray<ASpace*> ToSpaces(const TArray<int32>& Indices) const;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ASpace>> Spaces;

	TArray<FGraphEdge> Edges;

	bool bSealed = false;
};
