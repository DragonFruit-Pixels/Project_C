// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Map/GraphMath.h"
#include "GraphSubsystem.generated.h"

class ASpace;

UCLASS()
class PROJECTC_API UGraphSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterSpace(ASpace* Space);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool SealGraph();

	UFUNCTION(BlueprintPure, Category = "Graph")
	bool IsSealed() const { return bSealed; }

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetSpaceCount() const { return Spaces.Num(); }

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDistance(const ASpace* From, const ASpace* To) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDegree(const ASpace* Space) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetReachable(const ASpace* From, int32 MaxSteps) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetShortestPath(const ASpace* From, const ASpace* To) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetNearest(const ASpace* From, const TArray<ASpace*>& Candidates) const;

	int32 IndexOf(const ASpace* Space) const;

private:
	bool EnsureSealed(const TCHAR* Context) const;

	TArray<ASpace*> ToSpaces(const TArray<int32>& Indices) const;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ASpace>> Spaces;

	TArray<FGraphEdge> Edges;

	bool bSealed = false;
};
