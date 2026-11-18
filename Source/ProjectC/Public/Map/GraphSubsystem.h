#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Map/GraphMath.h"
#include "GraphSubsystem.generated.h"

UCLASS()
class PROJECTC_API UGraphSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Graph")
	void RegisterSpace(AActor* Space, const TArray<AActor*>& Neighbours, const TArray<AActor*>& BlockedTowards);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool SealGraph();

	UFUNCTION(BlueprintPure, Category = "Graph")
	bool IsSealed() const { return bSealed; }

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetSpaceCount() const { return Spaces.Num(); }

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDistance(const AActor* From, const AActor* To) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDegree(const AActor* Space) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<AActor*> GetReachable(const AActor* From, int32 MaxSteps) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<AActor*> GetShortestPath(const AActor* From, const AActor* To) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<AActor*> GetNearest(const AActor* From, const TArray<AActor*>& Candidates) const;

	int32 IndexOf(const AActor* Space) const;

private:
	struct FSpaceLinks
	{
		TArray<TWeakObjectPtr<AActor>> Neighbours;
		TArray<TWeakObjectPtr<AActor>> BlockedTowards;
	};

	bool EnsureSealed(const TCHAR* Context) const;

	TArray<AActor*> ToSpaces(const TArray<int32>& Indices) const;

	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> Spaces;

	TArray<FSpaceLinks> Links;

	TArray<FGraphEdge> Edges;

	bool bSealed = false;
};
