#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

namespace ProjectCCollision
{
	inline constexpr ECollisionChannel Space = ECC_GameTraceChannel1;

	inline constexpr ECollisionChannel Figure = ECC_GameTraceChannel2;

	inline constexpr ECollisionChannel Selectable = ECC_GameTraceChannel3;

	inline FName SpaceProfile() { return FName(TEXT("Space")); }

	inline FName FigureProfile() { return FName(TEXT("Figure")); }
}
