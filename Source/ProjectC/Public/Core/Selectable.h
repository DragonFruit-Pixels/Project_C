// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Selectable.generated.h"

/**
 * Como se esta mostrando ahora mismo una cosa seleccionable.
 *
 * Es **un estado, no cuatro eventos**. La alternativa —`OnHoverBegin`, `OnHoverEnd`,
 * `OnSelected`, `OnDeselected`— obliga a cada implementador a reconstruir el estado a partir de
 * la secuencia de avisos, y el primer caso raro lo rompe: hoverear algo que ya esta seleccionado
 * manda `OnHoverBegin` y el objeto no sabe si al salir tiene que volver a "normal" o a
 * "seleccionado".
 *
 * Con un estado, el que decide es uno solo —`AMissionPlayerController`, que es el unico que
 * conoce las tres cosas a la vez— y el Blueprint solo dibuja lo que le dicen. La precedencia
 * (`Selected` > `Hovered` > `Legal` > `None`) se resuelve ahi y no se replica.
 *
 * Los valores mapean 1 a 1 al parametro escalar `HighlightState` del material.
 */
UENUM(BlueprintType)
enum class ESelectionHighlight : uint8
{
	/** Sin resaltar. */
	None = 0,

	/** Un destino legal de la accion en curso. Se pinta en conjunto, no de a uno. */
	Legal = 1,

	/** El mouse esta encima. */
	Hovered = 2,

	/** Es la seleccion actual del jugador. */
	Selected = 3
};

UINTERFACE(BlueprintType, MinimalAPI)
class USelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Lo que el mouse puede elegir: un `Space`, una figura, y mas adelante un dado.
 *
 * Existe para que la seleccion **no se resuelva casteando**. El `PlayerController` traza contra
 * el canal `Selectable` y le habla al actor que pego sin saber de que clase es: es tráfico
 * lateral, y el mecanismo del proyecto para eso es la interfaz
 * (design/architecture/03-comunicacion-y-referencias.md).
 *
 * Todo es `BlueprintNativeEvent`: C++ da un default razonable y el Blueprint hijo decide **como
 * se ve**, que es presentación y por lo tanto no va en C++
 * (design/architecture/08-presentacion-y-reglas.md).
 */
class PROJECTC_API ISelectable
{
	GENERATED_BODY()

public:
	/**
	 * Si ahora mismo se puede elegir esto.
	 *
	 * No es "existe": un `Space` fuera de alcance sigue siendo trazable y sigue mostrando su
	 * nombre, pero no se selecciona. Default true.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	bool CanBeSelected() const;
	virtual bool CanBeSelected_Implementation() const { return true; }

	/**
	 * Muestra este estado. El default de C++ no hace nada a propósito: sin subclase Blueprint no
	 * hay material que tocar, y una base que asume que existe un mesh se rompe con el primer
	 * seleccionable que no lo tenga.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	void SetHighlight(ESelectionHighlight Highlight);
	virtual void SetHighlight_Implementation(ESelectionHighlight /*Highlight*/) {}

	/** Para debug y, mas adelante, para el tooltip del HUD. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selectable")
	FText GetSelectableName() const;
	virtual FText GetSelectableName_Implementation() const { return FText::GetEmpty(); }
};
