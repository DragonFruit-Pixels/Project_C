// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/GraphMath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

/**
 * La regla de legalidad de la accion `Move`.
 *
 * `AMissionGameMode::GetLegalDestinations` es lo que decide a donde puede ir una figura, y lo
 * decide llamando a `FGraphMath::Reachable` con las perillas de
 * design/gdd/07-balance/perillas-y-constantes.md: 3 espacios por `Move`.
 *
 * El GameMode necesita mundo y no se puede instanciar en un test de unidad. Lo que si se testea
 * es **la regla que consulta**, que es donde vive la logica: si el alcance esta bien, lo unico
 * que queda del lado del GameMode es cobrar la accion y sacar el origen de la lista.
 *
 * Reusa el grafo de ejemplo de design/gdd/01-fundamentos/mapa-y-espacios.md, el mismo que ya usa
 * GraphMathTest.cpp. No se invento ningun caso.
 */

namespace
{
	/**
	 *   S1 - S2 - S3 - S4 - S5          escalera:  S1 <-> S4
	 *    |                              bloqueo:   pasaje S1 - S2
	 *   S6 - S7
	 */
	enum : int32 { S1 = 0, S2, S3, S4, S5, S6, S7, NodeCount };

	TArray<FGraphEdge> ExampleGraph()
	{
		return {
			FGraphEdge(S1, S2, /*bBlocked*/ true),
			FGraphEdge(S2, S3),
			FGraphEdge(S3, S4),
			FGraphEdge(S4, S5),
			FGraphEdge(S1, S6),
			FGraphEdge(S6, S7),
			FGraphEdge(S1, S4), // escalera: adyacentes "para todo efecto"
		};
	}

	/** La perilla real del GDD: 3 espacios por accion de `Move` (manual pag. 13). */
	constexpr int32 SpacesPerMove = 3;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FMoveLegalityTest,
	"ProjectC.Rules.Move.Legality",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FMoveLegalityTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();
	const FGraphQuery Movement = FGraphQuery::ForMovement();

	// --- A un paso desde S1 ---
	//
	// Un solo caso verifica las dos reglas raras del mapa a la vez: el pasaje S1-S2 esta
	// bloqueado, asi que S2 **no** entra aunque sea vecino; y la escalera S1-S4 es una arista
	// como cualquier otra, asi que S4 **si** entra aunque este del otro lado del tablero.
	{
		const TArray<int32> OneStep = FGraphMath::Reachable(NodeCount, Edges, S1, 1, Movement);

		TestTrue(TEXT("a 1 paso se llega a S6"), OneStep.Contains(S6));
		TestTrue(TEXT("a 1 paso se llega a S4 por la escalera"), OneStep.Contains(S4));
		TestFalse(TEXT("a 1 paso NO se llega a S2: el pasaje esta bloqueado"), OneStep.Contains(S2));
		TestFalse(TEXT("a 1 paso NO se llega a S5: esta a 2"), OneStep.Contains(S5));

		// El origen entra porque d(a,a)=0 <= MaxSteps. Es correcto para "que espacios alcanzo",
		// y es exactamente por eso que GetLegalDestinations lo saca despues: quedarse quieto no
		// es un destino, es no gastar la accion.
		TestTrue(TEXT("Reachable incluye el origen"), OneStep.Contains(S1));
		// {S1, S4, S6} y nada mas. El conteo esta aparte de los Contains a proposito: los Contains
		// verifican que este lo que tiene que estar, el conteo verifica que NO este nada mas.
		TestEqual(TEXT("a 1 paso desde S1 hay 3 nodos contando el origen"), OneStep.Num(), 3);
	}

	// --- El permiso de ignorar bloqueos es de la consulta, no del mapa ---
	//
	// Mismo tablero, misma casilla, dos figuras distintas y dos conjuntos de destinos distintos.
	{
		FGraphQuery Ignoring = FGraphQuery::ForMovement();
		Ignoring.bIgnoresBlocked = true;

		const TArray<int32> OneStep = FGraphMath::Reachable(NodeCount, Edges, S1, 1, Ignoring);

		TestTrue(TEXT("ignorando bloqueos, a 1 paso si se llega a S2"), OneStep.Contains(S2));
	}

	// --- Sin pasos no hay movimiento ---
	{
		const TArray<int32> NoSteps = FGraphMath::Reachable(NodeCount, Edges, S1, 0, Movement);

		TestEqual(TEXT("con 0 pasos solo queda el origen"), NoSteps.Num(), 1);
		TestTrue(TEXT("y ese origen es S1"), NoSteps.Contains(S1));
	}

	// --- Con la perilla real, el BFS no se corta antes de tiempo ---
	//
	// A 3 pasos desde S1 se llega a todo el tablero, incluido S2: bloqueado de frente, pero
	// alcanzable rodeando por la escalera (S1 -> S4 -> S3 -> S2). Que un bloqueo no aisle un
	// espacio sino que lo encarezca es la propiedad que hace que las paredes sean interesantes.
	{
		const TArray<int32> ThreeSteps = FGraphMath::Reachable(NodeCount, Edges, S1, SpacesPerMove, Movement);

		TestEqual(TEXT("a 3 pasos desde S1 se alcanza el tablero entero"), ThreeSteps.Num(), NodeCount);
		TestTrue(TEXT("S2 se alcanza rodeando el pasaje bloqueado"), ThreeSteps.Contains(S2));
	}

	// --- El alcance crece con los pasos, nunca se achica ---
	//
	// Es la propiedad que hace que subir la perilla de 3 a 4 no pueda quitar destinos. Barata de
	// verificar y es la que se rompe si alguien "optimiza" el BFS con una poda mal puesta.
	{
		int32 Previous = 0;
		for (int32 Steps = 0; Steps <= NodeCount; ++Steps)
		{
			const int32 Count = FGraphMath::Reachable(NodeCount, Edges, S1, Steps, Movement).Num();
			TestTrue(
				FString::Printf(TEXT("el alcance a %d pasos no es menor que a %d"), Steps, Steps - 1),
				Count >= Previous);
			Previous = Count;
		}
	}

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
