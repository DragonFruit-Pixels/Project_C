// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Selectable.h"
#include "Space.generated.h"

class UBoxComponent;

/**
 * Un nodo del grafo: un `Space`.
 *
 * **Por qué es un Actor y no solo datos.** Tiene que existir en el mundo para que el raycast del
 * mouse lo seleccione (clase 4), para que la figura tenga a dónde caminar, y para que las salas
 * puedan editarse en sublevels distintos por personas distintas. El grafo *lógico* igual vive en
 * UGraphSubsystem, no repartido entre los actores.
 *
 * **Las paredes no existen.** Una pared es la ausencia de arista: si dos `Space` no se listan como
 * vecinos, no son adyacentes, haya o no geometría entre ellos.
 *
 * Diseño: design/gdd/01-fundamentos/mapa-y-espacios.md
 */
UCLASS()
class PROJECTC_API ASpace : public AActor, public ISelectable
{
	GENERATED_BODY()

public:
	ASpace();

	/**
	 * Los `Space` adyacentes a éste.
	 *
	 * **La adyacencia es bidireccional o no existe** (F1): si A lista a B, B tiene que listar a
	 * A. UGraphSubsystem valida la simetría al sellar y falla ruidoso si falta un lado — una
	 * flecha de un solo sentido es un mapa mal armado, no una arista dirigida.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Space")
	TArray<TObjectPtr<ASpace>> Neighbours;

	/**
	 * Pasajes bloqueados ahora mismo, como subconjunto de `Neighbours`.
	 *
	 * Un bloqueo no borra la arista: la deja intransitable para quien no pueda ignorarlo, y una
	 * figura con ese permiso la cruza y además no la cuenta al medir distancias.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Space")
	TArray<TObjectPtr<ASpace>> BlockedTowards;

	/** El volumen que el raycast de selección pega. Perfil de colisión `Space`: sólo query. */
	UFUNCTION(BlueprintPure, Category = "Space")
	UBoxComponent* GetBounds() const { return Bounds; }

	/**
	 * Dónde apoya los pies una figura parada acá: el centro de la base de `Bounds`.
	 *
	 * Existe para que quien mueve una figura no tenga que saber cómo está armado el `Space`. Sin
	 * esto, cada llamador reconstruye el offset a mano y el día que `BP_Space` cambie la altura
	 * de la caja las figuras se hunden en silencio.
	 */
	UFUNCTION(BlueprintPure, Category = "Space")
	FVector GetFigureAnchorLocation() const;

	// --- ISelectable ---

	virtual bool CanBeSelected_Implementation() const override { return true; }
	virtual FText GetSelectableName_Implementation() const override;

protected:
	virtual void BeginPlay() override;

	/**
	 * Volumen de selección, no de física.
	 *
	 * Con perfil `Space` queda en QueryOnly: no simula, no bloquea el movimiento y no choca con
	 * las figuras — no hay límite de figuras por espacio, así que nada debe empujar a nada.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Space")
	TObjectPtr<UBoxComponent> Bounds;
};
