// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Rules/RatchetRules.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

/**
 * Tests del track de `Ratchet`.
 *
 * Verifican los números verificados de design/gdd/07-balance/perillas-y-constantes.md: 20 espacios,
 * 6 umbrales en 4/8/12/15/18/19 con saltos 4-4-4-3-3-1, y muerte al llegar al final.
 *
 * Esto es lo que Blueprint no podía dar: los criterios de aceptación del GDD dejan de ser párrafos.
 */

namespace
{
	/**
	 * Función y no constante global: un TArray a nivel de archivo asigna memoria durante la
	 * inicialización estática, antes de que exista GMalloc. Es una de las formas clásicas de
	 * romper el arranque del engine.
	 */
	TArray<int32> DefaultThresholds()
	{
		return { 4, 8, 12, 15, 18, 19 };
	}

	constexpr int32 DefaultTrackLength = 20;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRatchetThresholdsTest,
	"ProjectC.Rules.Ratchet.Thresholds",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FRatchetThresholdsTest::RunTest(const FString& Parameters)
{
	const TArray<int32> Thresholds = DefaultThresholds();

	// Avanzando de a 1 desde 0 hasta el final, se cruzan exactamente 6 umbrales, en orden.
	TArray<int32> Seen;
	for (int32 Position = 0; Position < DefaultTrackLength; ++Position)
	{
		Seen.Append(FRatchetRules::ThresholdsCrossed(Position, Position + 1, Thresholds));
	}

	TestEqual(TEXT("se cruzan 6 umbrales avanzando de a 1"), Seen.Num(), 6);
	if (Seen.Num() == 6)
	{
		TestEqual(TEXT("umbral 1"), Seen[0], 4);
		TestEqual(TEXT("umbral 2"), Seen[1], 8);
		TestEqual(TEXT("umbral 3"), Seen[2], 12);
		TestEqual(TEXT("umbral 4"), Seen[3], 15);
		TestEqual(TEXT("umbral 5"), Seen[4], 18);
		TestEqual(TEXT("umbral 6"), Seen[5], 19);
	}

	// El espaciado ACELERA: 4-4-4-3-3-1. Es lo que hace que el final del track entregue dos
	// niveles de skill casi juntos y después mate.
	const int32 ExpectedGaps[] = { 4, 4, 4, 3, 3, 1 };
	int32 Previous = 0;
	for (int32 Index = 0; Index < Thresholds.Num(); ++Index)
	{
		TestEqual(
			*FString::Printf(TEXT("salto hasta el umbral %d"), Index + 1),
			Thresholds[Index] - Previous,
			ExpectedGaps[Index]);
		Previous = Thresholds[Index];
	}

	// El último umbral queda a UNA casilla de la muerte.
	TestEqual(TEXT("el ultimo umbral esta a 1 del final"), DefaultTrackLength - Thresholds.Last(), 1);

	// Un solo avance grande cruza todos los umbrales que se saltearon.
	const TArray<int32> AllAtOnce = FRatchetRules::ThresholdsCrossed(0, DefaultTrackLength, Thresholds);
	TestEqual(TEXT("un avance de 0 al final cruza los 6"), AllAtOnce.Num(), 6);

	// El intervalo es abierto en From: volver a evaluar parado sobre un umbral no lo cruza de
	// nuevo. Sin esto, el mismo umbral dispararia dos veces.
	TestEqual(TEXT("no se recruza el umbral en el que uno esta parado"),
		FRatchetRules::ThresholdsCrossed(4, 4, Thresholds).Num(), 0);

	// Y es cerrado en To: caer justo encima de un umbral lo cruza.
	TestEqual(TEXT("caer exactamente en un umbral lo cruza"),
		FRatchetRules::ThresholdsCrossed(3, 4, Thresholds).Num(), 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRatchetAdvanceAndLossTest,
	"ProjectC.Rules.Ratchet.AdvanceAndLoss",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FRatchetAdvanceAndLossTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("avance normal"), FRatchetRules::Advance(0, 3, DefaultTrackLength), 3);

	// Pasarse del final no significa nada: se clampea, porque llegar al final ya es perder.
	TestEqual(TEXT("el avance se clampea al largo del track"),
		FRatchetRules::Advance(18, 9, DefaultTrackLength), DefaultTrackLength);

	// Se pierde el personaje AL LLEGAR al final, no al pasarlo.
	TestFalse(TEXT("la casilla 19 no mata"), FRatchetRules::IsLost(19, DefaultTrackLength));
	TestTrue(TEXT("la casilla 20 mata"), FRatchetRules::IsLost(20, DefaultTrackLength));

	// El trinquete SOLO SUBE: es la propiedad que define la mecánica central.
	TestEqual(TEXT("un Toll negativo no mueve nada"), FRatchetRules::Advance(5, -3, DefaultTrackLength), 5);
	TestEqual(TEXT("no se cruzan umbrales hacia atras"),
		FRatchetRules::ThresholdsCrossed(12, 4, DefaultThresholds()).Num(), 0);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
