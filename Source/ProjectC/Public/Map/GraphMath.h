// Copyright DragonFruit Pixels. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Una arista del grafo de espacios.
 *
 * Los nodos son índices, no punteros: esta capa no conoce actores ni el mundo. Traducir de
 * `ASpace` a índice es trabajo de UGraphSubsystem.
 */
struct PROJECTC_API FGraphEdge
{
	int32 A = INDEX_NONE;
	int32 B = INDEX_NONE;

	/**
	 * Pasaje bloqueado en este instante.
	 *
	 * No se ignora solo: una figura que puede ignorar bloqueos lo cruza igual, y para ella
	 * tampoco cuenta al medir distancias. Por eso el bloqueo es de la arista pero la decisión
	 * es de la consulta.
	 */
	bool bBlocked = false;

	/**
	 * Adyacencia condicional de un `Trinket`: **habilita el movimiento pero se ignora al
	 * calcular el camino más corto** (manual p. 20).
	 *
	 * Es la razón por la que hay dos grafos y no uno. Consecuencia concreta: podés estar a 1
	 * paso de un enemigo que el juego reporta a distancia 4.
	 */
	bool bMovementOnly = false;

	FGraphEdge() = default;
	FGraphEdge(int32 InA, int32 InB, bool bInBlocked = false, bool bInMovementOnly = false)
		: A(InA), B(InB), bBlocked(bInBlocked), bMovementOnly(bInMovementOnly) {}
};

/**
 * Con qué permisos se evalúa el grafo.
 *
 * La misma consulta, el mismo tablero y dos contextos distintos dan **dos tablas de distancia
 * distintas**. Eso no es un bug: es la excepción del manual (p. 20), y por eso el permiso es un
 * parámetro de la consulta y nunca estado del mapa.
 */
struct PROJECTC_API FGraphQuery
{
	/** La figura cruza pasajes bloqueados, y para ella esos bloqueos tampoco cuentan. */
	bool bIgnoresBlocked = false;

	/**
	 * true para resolver **movimiento** (incluye las aristas de `Trinket`),
	 * false para medir **distancia** (las excluye). Ver F2 en mapa-y-espacios.md.
	 */
	bool bIncludeMovementOnly = false;

	static FGraphQuery ForDistance() { return FGraphQuery{}; }
	static FGraphQuery ForMovement() { FGraphQuery Q; Q.bIncludeMovementOnly = true; return Q; }
};

/**
 * Teoría de grafos del mapa. Funciones puras, sin `UObject` y sin mundo, para que sean
 * testeables sin engine — mismo patrón que FRatchetRules.
 *
 * Todas las aristas pesan 1, así que **es BFS y no hace falta Dijkstra**. Con 48 nodos de techo y
 * grado promedio 2.5, un BFS completo visita 48 nodos y 120 aristas: es despreciable y se puede
 * correr por consulta sin cachear nada.
 *
 * Diseño y fórmulas: design/gdd/01-fundamentos/mapa-y-espacios.md (F1 a F8).
 * Los tests verifican los criterios de aceptación de ese mismo documento.
 */
struct PROJECTC_API FGraphMath
{
	/** Distancia devuelta para un nodo al que no se puede llegar con estos permisos. */
	static constexpr int32 Unreachable = TNumericLimits<int32>::Max();

	/**
	 * BFS desde `From`. Devuelve un array de tamaño `NodeCount`; los inalcanzables quedan en
	 * `Unreachable`.
	 *
	 * `d(a, a) = 0` siempre: si algo está en tu espacio, es lo más cercano que existe.
	 */
	static TArray<int32> Distances(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const FGraphQuery& Query);

	/** Distancia puntual. `Unreachable` si no hay camino. */
	static int32 Distance(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query);

	/**
	 * Cuántos espacios son adyacentes a `Node`.
	 *
	 * No es solo una propiedad del layout: **las cartas lo consultan** — `Claustrophobia` lee el
	 * grado del espacio donde estás. Por eso el mapa no puede ser una retícula uniforme.
	 */
	static int32 Degree(const TArray<FGraphEdge>& Edges, int32 Node, const FGraphQuery& Query);

	/**
	 * Los nodos a distancia <= `MaxSteps` (F4).
	 *
	 * **Sirve para pintar el highlight de destinos, no para resolver el movimiento**: el camino
	 * no tiene que ser simple —se puede volver al espacio de origen y salir de nuevo— y cada
	 * salida vuelve a disparar "te siguen" y `Hazard`.
	 */
	static TArray<int32> Reachable(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 MaxSteps, const FGraphQuery& Query);

	/**
	 * Un camino más corto de `From` a `To`, incluyendo los dos extremos. Vacío si no hay camino.
	 *
	 * Puede haber varios de la misma longitud y **todos son válidos**; el manual dice que elige
	 * el jugador (p. 15). Esta función devuelve uno determinista; ofrecer la elección es trabajo
	 * de la capa que pregunta.
	 */
	static TArray<int32> ShortestPath(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, const FGraphQuery& Query);

	/**
	 * Los candidatos a distancia mínima desde `From` (F5). Los inalcanzables **salen del
	 * conjunto**: un espacio al que no se puede llegar no es "el más cercano", es nada.
	 *
	 * Devuelve todos los empatados. Si son más de uno, **el juego tiene que preguntar** — no
	 * autoseleccionar ni usar el orden de spawn como desempate (manual p. 20).
	 */
	static TArray<int32> Nearest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query);

	/** Los candidatos a distancia máxima. Misma regla de empate y de inalcanzables. */
	static TArray<int32> Farthest(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, const TArray<int32>& Candidates, const FGraphQuery& Query);

	/**
	 * Dónde termina una figura empujada `Steps` pasos desde `From` hacia `To` (F6).
	 *
	 * **Para al llegar y descarta los pasos sobrantes**: no rebota, no sigue de largo, no queda
	 * "cargada". Si no hay camino devuelve `INDEX_NONE`, y el paso que la empujaba **se saltea**
	 * sin que la carta falle.
	 */
	static int32 PushTowards(int32 NodeCount, const TArray<FGraphEdge>& Edges, int32 From, int32 To, int32 Steps, const FGraphQuery& Query);

	/** Una arista participa de esta consulta. */
	static bool EdgeApplies(const FGraphEdge& Edge, const FGraphQuery& Query);
};
