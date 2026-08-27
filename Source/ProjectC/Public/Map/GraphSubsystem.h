// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Map/GraphMath.h"
#include "GraphSubsystem.generated.h"

class ASpace;

/**
 * El grafo de espacios del nivel: adyacencias, distancias y grado.
 *
 * Es un `UWorldSubsystem` y no estado del `GameState` porque **responde preguntas, no decide** y
 * porque su ciclo de vida es exactamente el del nivel. Ver 02-managers-y-subsystems.md.
 *
 * **El sellado no es ceremonia.** Cada ASpace se registra en su BeginPlay, así que hasta que
 * terminaron todos el grafo está incompleto — y un BFS sobre un grafo incompleto **no crashea:
 * contesta mal**. Devuelve "no hay camino" donde había uno, o uno más largo. En un juego donde el
 * camino más corto decide a dónde se mueven los enemigos, mentir es peor que crashear.
 *
 * Por eso toda consulta antes del sellado falla ruidoso y devuelve un resultado vacío.
 * Registrado como D-21 en design/gdd/06-decisiones/registro.md.
 */
UCLASS()
class PROJECTC_API UGraphSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Alta de un `Space`. La llama ASpace::BeginPlay; no hace falta ordenarla a mano. */
	void RegisterSpace(ASpace* Space);

	/**
	 * Cierra el registro, arma las aristas desde los vecinos de cada `Space` y corre los
	 * invariantes. A partir de acá las consultas responden.
	 *
	 * La llama el `GameMode` cuando arranca la misión, no el propio subsystem: quién decide que
	 * el mundo está listo es el árbitro.
	 */
	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool SealGraph();

	UFUNCTION(BlueprintPure, Category = "Graph")
	bool IsSealed() const { return bSealed; }

	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetSpaceCount() const { return Spaces.Num(); }

	/** `INDEX_NONE` si no hay camino, o si el grafo todavía no está sellado. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDistance(const ASpace* From, const ASpace* To) const;

	/** Cuántos espacios son adyacentes. Lo consultan las cartas, no sólo el layout. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	int32 GetDegree(const ASpace* Space) const;

	/** Los `Space` a los que se puede llegar gastando hasta `MaxSteps` pasos. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetReachable(const ASpace* From, int32 MaxSteps) const;

	/** Un camino más corto, incluidos los dos extremos. Vacío si no hay. */
	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetShortestPath(const ASpace* From, const ASpace* To) const;

	/**
	 * Los candidatos empatados a distancia mínima.
	 *
	 * Si devuelve más de uno, **el juego tiene que preguntar**: el manual no admite desempate
	 * automático. Si devuelve uno solo, no se abre selección.
	 */
	UFUNCTION(BlueprintPure, Category = "Graph")
	TArray<ASpace*> GetNearest(const ASpace* From, const TArray<ASpace*>& Candidates) const;

	/** Índice interno de un `Space`, o `INDEX_NONE`. Útil para tests y debug. */
	int32 IndexOf(const ASpace* Space) const;

private:
	/** Deja constancia y devuelve false si todavía no se selló. */
	bool EnsureSealed(const TCHAR* Context) const;

	TArray<ASpace*> ToSpaces(const TArray<int32>& Indices) const;

	UPROPERTY(Transient)
	TArray<TObjectPtr<ASpace>> Spaces;

	TArray<FGraphEdge> Edges;

	bool bSealed = false;
};
