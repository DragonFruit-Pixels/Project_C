// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

/**
 * Los canales y perfiles de colision propios del proyecto, con nombre.
 *
 * Los canales se declaran en Config/DefaultEngine.ini como `ECC_GameTraceChannel1..3`, que son
 * ranuras posicionales: si alguien inserta un canal nuevo, los numeros se corren y cada trace
 * escrito a mano empieza a pegarle a otra cosa **sin error de compilacion**. Este header es el
 * unico lugar donde vive esa correspondencia.
 *
 * Los perfiles son FName y se devuelven por funcion, no por constante global: construir un FName
 * durante la inicializacion estatica corre antes de que exista la tabla de nombres del engine.
 * Es el mismo motivo por el que los tests arman sus TArray dentro de una funcion.
 */
namespace ProjectCCollision
{
	/** Nodo del grafo. Objeto, no trazado: `bTraceType=False` en el .ini. */
	inline constexpr ECollisionChannel Space = ECC_GameTraceChannel1;

	/** Personaje, enemigo o Ally. Objeto. */
	inline constexpr ECollisionChannel Figure = ECC_GameTraceChannel2;

	/**
	 * El canal que traza el mouse. Es **canal de trazado** (`bTraceType=True`), no de objeto:
	 * no describe que es una cosa, describe una pregunta que se le hace al mundo.
	 */
	inline constexpr ECollisionChannel Selectable = ECC_GameTraceChannel3;

	/** Perfil de `ASpace::Bounds`: QueryOnly, solo bloquea `Selectable`. */
	inline FName SpaceProfile() { return FName(TEXT("Space")); }

	/** Perfil del volumen de seleccion de una figura. No es el perfil de su capsula. */
	inline FName FigureProfile() { return FName(TEXT("Figure")); }
}
