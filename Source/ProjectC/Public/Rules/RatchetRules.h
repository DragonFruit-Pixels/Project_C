// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Reglas puras del track de `Ratchet`.
 *
 * No hereda de UObject y no toca el mundo a propósito: es la parte de la mecánica central que se
 * puede testear sin engine. Los tests viven en Tests/RatchetRulesTest.cpp.
 *
 * Diseño: ver design/gdd/02-personaje/trinquete.md
 * Números: los declara design/gdd/07-balance/perillas-y-constantes.md, y nadie más.
 */
struct PROJECTC_API FRatchetRules
{
	/**
	 * Avanza la posición y la clampea al largo del track.
	 *
	 * El clamp existe porque pasarse del final no significa nada: llegar al final ya es perder el
	 * personaje, y un valor 23 en un track de 20 sería un estado imposible de mostrar en la UI.
	 */
	static int32 Advance(int32 From, int32 Amount, int32 TrackLength);

	/**
	 * Los umbrales que quedaron atravesados al pasar de `From` a `To`.
	 *
	 * El intervalo es **abierto en From y cerrado en To**: caer justo encima de un umbral lo
	 * cruza, y volver a evaluar desde esa misma posición no lo cruza de nuevo. Eso es lo que
	 * evita el bug de disparar dos veces el mismo umbral, que es la clase de error que en un
	 * grafo de Blueprint se descubre jugando.
	 *
	 * Devuelve los umbrales en orden ascendente. Un solo avance puede cruzar varios.
	 */
	static TArray<int32> ThresholdsCrossed(int32 From, int32 To, const TArray<int32>& Thresholds);

	/** El personaje se pierde al llegar al final del track, no al pasarlo. */
	static bool IsLost(int32 Position, int32 TrackLength);
};
