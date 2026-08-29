// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * An edge of the space graph.
 *
 * Nodes are indices, not pointers: this layer knows nothing about actors or the world.
 * Translating from `ASpace` to an index is UGraphSubsystem's job.
 */
struct PROJECTC_API FGraphEdge
{
	int32 A = INDEX_NONE;
	int32 B = INDEX_NONE;

	/**
	 * Passage blocked at this instant.
	 *
	 * It does not get ignored on its own: a figure that can ignore blocks crosses it anyway, and
	 * for that figure the block does not count when measuring distances either. That is why the
	 * block belongs to the edge but the decision belongs to the query.
	 */
	bool bBlocked = false;

	/**
	 * Conditional adjacency granted by a `Trinket`: **it enables movement but is ignored when
	 * computing the shortest path** (rulebook p. 20).
	 *
	 * This is the reason there are two graphs and not one. Concrete consequence: you can be 1
	 * step away from an enemy that the game reports at distance 4.
	 */
	bool bMovementOnly = false;

	FGraphEdge() = default;
	FGraphEdge(int32 InA, int32 InB, bool bInBlocked = false, bool bInMovementOnly = false)
		: A(InA), B(InB), bBlocked(bInBlocked), bMovementOnly(bInMovementOnly) {}
};

/**
 * The permissions the graph is evaluated with.
 *
 * The same query, the same board and two different contexts give **two different distance
 * tables**. That is not a bug: it is the rulebook's exception (p. 20), and it is why the
 * permission is a parameter of the query and never state on the map.
 */
struct PROJECTC_API FGraphQuery
{
	/** The figure crosses blocked passages, and for it those blocks do not count either. */
	bool bIgnoresBlocked = false;

	/**
	 * true to resolve **movement** (includes the `Trinket` edges),
	 * false to measure **distance** (excludes them). See F2 in mapa-y-espacios.md.
	 */
	bool bIncludeMovementOnly = false;

	static FGraphQuery ForDistance() { return FGraphQuery{}; }
	static FGraphQuery ForMovement() { FGraphQuery Q; Q.bIncludeMovementOnly = true; return Q; }
};

/**
 * Graph theory for the map. Pure functions, no `UObject` and no world, so they are testable
 * without the engine -- the same pattern as FRatchetRules.
 *
 * Every edge weighs 1, so **this is BFS and Dijkstra is unnecessary**. With a ceiling of 48 nodes
 * and an average degree of 2.5, a full BFS visits 48 nodes and 120 edges: negligible, and cheap
 * enough to run per query without caching anything.
 *
 * Design and formulas: design/gdd/01-fundamentos/mapa-y-espacios.md (F1 through F8).
 * The tests verify the acceptance criteria of that same document.
 */
struct PROJECTC_API FGraphMath
{
	/** Distance returned for a node that cannot be reached with these permissions. */
	static constexpr int32 Unreachable = TNumericLimits<int32>::Max();

	/**
	 * BFS from `From`. Returns an array of size `NodeCount`; unreachable nodes stay at
	 * `Unreachable`.
	 *
	 * `d(a, a) = 0` always: if something is in your space, it is the closest thing there is.
	 */
	static TArray<int32> Distances(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const FGraphQuery& Query);

	/** Single distance. `Unreachable` if there is no path. */
	static int32 Distance(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query);

	/**
	 * How many spaces are adjacent to `Node`.
	 *
	 * It is not just a property of the layout: **the cards query it** -- `Claustrophobia` reads
	 * the degree of the space you are standing on. That is why the map cannot be a uniform grid.
	 */
	static int32 Degree(const TArray<FGraphEdge>& Edges, int32 Node, const FGraphQuery& Query);

	/**
	 * The nodes at distance <= `MaxSteps` (F4).
	 *
	 * **Use it to paint the destination highlight, not to resolve the movement**: the path does
	 * not have to be simple -- you can return to the origin space and leave again -- and every
	 * exit re-triggers "they follow you" and `Hazard`.
	 */
	static TArray<int32> Reachable(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 MaxSteps, const FGraphQuery& Query);

	/**
	 * A shortest path from `From` to `To`, including both endpoints. Empty if there is no path.
	 *
	 * There can be several of the same length and **all of them are valid**; the rulebook says
	 * the player chooses (p. 15). This function returns a deterministic one; offering the choice
	 * is the job of the layer that asks.
	 */
	static TArray<int32> ShortestPath(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query);

	/**
	 * The candidates at minimum distance from `From` (F5). Unreachable ones **drop out of the
	 * set**: a space you cannot reach is not "the closest one", it is nothing.
	 *
	 * Returns every tied candidate. If there is more than one, **the game has to ask** -- it must
	 * not auto-select, nor use spawn order as a tiebreak (rulebook p. 20).
	 */
	static TArray<int32> Nearest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query);

	/** The candidates at maximum distance. Same rule for ties and for unreachable nodes. */
	static TArray<int32> Farthest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query);

	/**
	 * Where a figure pushed `Steps` steps from `From` towards `To` ends up (F6).
	 *
	 * **It stops on arrival and discards the leftover steps**: it does not bounce, does not
	 * overshoot, and is not left "charged". If there is no path it returns `INDEX_NONE`, and the
	 * step that was pushing it **is skipped** without the card failing.
	 */
	static int32 PushTowards(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, int32 Steps, const FGraphQuery& Query);

	/** Whether an edge takes part in this query. */
	static bool EdgeApplies(const FGraphEdge& Edge, const FGraphQuery& Query);
};
