#include "Core/ScalabilitySubsystem.h"

#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectCScalability, Log, All);

void UScalabilitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GIsEditor || !GEngine)
	{
		return;
	}

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings || Settings->GetLastCPUBenchmarkResult() >= 0.0f)
	{
		return;
	}

	Settings->RunHardwareBenchmark();
	Settings->ApplyHardwareBenchmarkResults();
	Settings->SaveSettings();

	UE_LOG(LogProjectCScalability, Log, TEXT("First launch: benchmark CPU %.1f GPU %.1f, overall quality %d"),
		Settings->GetLastCPUBenchmarkResult(), Settings->GetLastGPUBenchmarkResult(), Settings->GetOverallScalabilityLevel());
}
