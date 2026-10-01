#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectCRulesLibrary.generated.h"

UCLASS()
class PROJECTC_API UProjectCRulesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Ratchet")
	static int32 RatchetAdvance(int32 From, int32 Amount, int32 TrackLength);

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	static TArray<int32> RatchetThresholdsCrossed(int32 From, int32 To, const TArray<int32>& Thresholds);

	UFUNCTION(BlueprintPure, Category = "Ratchet")
	static bool RatchetIsLost(int32 Position, int32 TrackLength);

	UFUNCTION(BlueprintPure, Category = "Selection")
	static TArray<TEnumAsByte<EObjectTypeQuery>> GetSpaceObjectTypes();

	UFUNCTION(BlueprintPure, Category = "Selection")
	static TArray<TEnumAsByte<EObjectTypeQuery>> GetFigureObjectTypes();
};
