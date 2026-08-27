// Copyright DragonFruit Pixels. All Rights Reserved.

#include "Map/GraphMath.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

/**
 * Tests del grafo de espacios.
 *
 * Verifican los criterios de aceptación de design/gdd/01-fundamentos/mapa-y-espacios.md, que
 * traen el grafo de ejemplo y las dos tablas de distancia esperadas ya escritas. No se inventó
 * ningún caso: cada test apunta a un criterio numerado del GDD.
 */

namespace
{
	/**
	 * El grafo de ejemplo de F3, que el GDD reusa en F4, F5 y F6:
	 *
	 *   S1 — S2 — S3 — S4 — S5          escalera:  S1 <-> S4
	 *    |                              bloqueo:   pasaje S1 — S2
	 *   S6 — S7
	 *
	 * Índices 0..6 = S1..S7.
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
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGraphDistancesTest,
	"ProjectC.Rules.Graph.Distances",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FGraphDistancesTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();

	// Criterio 3: respetando el bloqueo, S2 esta a 3 porque hay que dar la vuelta por la escalera.
	const TArray<int32> Respecting = FGraphMath::Distances(NodeCount, Edges, S1, FGraphQuery::ForDistance());
	const TArray<int32> ExpectedRespecting = { 0, 3, 2, 1, 2, 1, 2 };
	for (int32 Node = 0; Node < NodeCount; ++Node)
	{
		TestEqual(FString::Printf(TEXT("respetando el bloqueo, d(S1,S%d)"), Node + 1), Respecting[Node], ExpectedRespecting[Node]);
	}

	// Criterio 4: la misma consulta para una figura que ignora bloqueos, sin mutar el grafo.
	FGraphQuery Ignoring = FGraphQuery::ForDistance();
	Ignoring.bIgnoresBlocked = true;

	const TArray<int32> WithIgnore = FGraphMath::Distances(NodeCount, Edges, S1, Ignoring);
	const TArray<int32> ExpectedIgnoring = { 0, 1, 2, 1, 2, 1, 2 };
	for (int32 Node = 0; Node < NodeCount; ++Node)
	{
		TestEqual(FString::Printf(TEXT("ignorando el bloqueo, d(S1,S%d)"), Node + 1), WithIgnore[Node], ExpectedIgnoring[Node]);
	}

	// Criterio 2: la distancia de un espacio a si mismo es 0.
	TestEqual(TEXT("d(s,s) = 0"), FGraphMath::Distance(NodeCount, Edges, S3, S3, FGraphQuery::ForDistance()), 0);

	// Criterio 5: sacar un token del par escalera deja a S4 a 3 pasos, no a 1.
	//
	// Se mide con el pasaje S1-S2 libre, que es el supuesto del criterio: con el bloqueo puesto
	// Y sin escalera, S1 queda encerrado con S6 y S7, y S4 pasa a ser inalcanzable. Son dos
	// escenarios distintos y mezclarlos fue el error de la primera version de este test.
	TArray<FGraphEdge> NoStair = Edges;
	NoStair.RemoveAt(NoStair.Num() - 1);

	TestEqual(TEXT("sin la escalera y con el pasaje libre, d(S1,S4) = 3"),
		FGraphMath::Distance(NodeCount, NoStair, S1, S4, Ignoring), 3);

	TestEqual(TEXT("sin la escalera y con el bloqueo puesto, S4 es inalcanzable"),
		FGraphMath::Distance(NodeCount, NoStair, S1, S4, FGraphQuery::ForDistance()), FGraphMath::Unreachable);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGraphUnreachableAndDegreeTest,
	"ProjectC.Rules.Graph.UnreachableAndDegree",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FGraphUnreachableAndDegreeTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();

	// El grado es un valor que las cartas consultan (Claustrophobia), no solo layout.
	TestEqual(TEXT("grado(S5) = 1"), FGraphMath::Degree(Edges, S5, FGraphQuery::ForDistance()), 1);

	// **Un pasaje bloqueado no cuenta como adyacente.** Sale de F2: Adyacente(a,b,f) se define
	// sobre E_mov(f), que excluye los bloqueos que esa figura no puede cruzar. Consecuencia de
	// juego: bloquear un pasaje le baja el grado al espacio, y Claustrophobia lo lee distinto.
	FGraphQuery Ignoring = FGraphQuery::ForDistance();
	Ignoring.bIgnoresBlocked = true;

	TestEqual(TEXT("grado(S1) con el bloqueo puesto = 2"), FGraphMath::Degree(Edges, S1, FGraphQuery::ForDistance()), 2);
	TestEqual(TEXT("grado(S1) para quien lo ignora = 3"), FGraphMath::Degree(Edges, S1, Ignoring), 3);
	TestEqual(TEXT("grado(S2) con el bloqueo puesto = 1"), FGraphMath::Degree(Edges, S2, FGraphQuery::ForDistance()), 1);

	// Un nodo aislado queda inalcanzable, no a distancia 0 ni negativa.
	TArray<FGraphEdge> WithIsland = Edges;
	const int32 IslandCount = NodeCount + 1;
	const int32 Island = NodeCount;
	TestEqual(TEXT("un nodo sin aristas es Unreachable"),
		FGraphMath::Distance(IslandCount, WithIsland, S1, Island, FGraphQuery::ForDistance()), FGraphMath::Unreachable);

	// Una arista de Trinket habilita moverse pero NO acorta la distancia medida (F2).
	TArray<FGraphEdge> WithTrinket = Edges;
	WithTrinket.Add(FGraphEdge(S1, S7, /*bBlocked*/ false, /*bMovementOnly*/ true));

	TestEqual(TEXT("con Trinket, moverse S1->S7 cuesta 1"),
		FGraphMath::Distance(NodeCount, WithTrinket, S1, S7, FGraphQuery::ForMovement()), 1);
	TestEqual(TEXT("pero la distancia reportada sigue siendo 2"),
		FGraphMath::Distance(NodeCount, WithTrinket, S1, S7, FGraphQuery::ForDistance()), 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGraphPushAndTiesTest,
	"ProjectC.Rules.Graph.PushAndTies",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FGraphPushAndTiesTest::RunTest(const FString& Parameters)
{
	const TArray<FGraphEdge> Edges = ExampleGraph();
	const FGraphQuery Q = FGraphQuery::ForDistance();

	// F6 ejemplo A: E2 en S3, destino S1, d=2, la carta empuja 2 -> llega.
	TestEqual(TEXT("empuje de 2 con d=2 llega al destino"), FGraphMath::PushTowards(NodeCount, Edges, S3, S1, 2, Q), (int32)S1);

	// F6 ejemplo B: E1 en S2, destino S1, d=3 respetando el bloqueo, empuje 2 -> queda a 1.
	const int32 Landed = FGraphMath::PushTowards(NodeCount, Edges, S2, S1, 2, Q);
	TestEqual(TEXT("empuje de 2 con d=3 deja la figura a distancia 1"),
		FGraphMath::Distance(NodeCount, Edges, Landed, S1, Q), 1);

	// Criterio 7 / F6 ejemplo C: le sobra paso y NO lo usa. Nunca sobrepasa el destino.
	TestEqual(TEXT("empuje de 2 con d=1 termina en el destino, no mas alla"),
		FGraphMath::PushTowards(NodeCount, Edges, S4, S1, 2, Q), (int32)S1);

	// F6 ejemplo D: sin camino, el paso se saltea. No es un error.
	TArray<FGraphEdge> WithIsland = Edges;
	const int32 IslandCount = NodeCount + 1;
	TestEqual(TEXT("sin camino, el empuje devuelve INDEX_NONE y la carta sigue"),
		FGraphMath::PushTowards(IslandCount, WithIsland, NodeCount, S1, 2, Q), (int32)INDEX_NONE);

	// Criterio 8: dos candidatos a la misma distancia devuelven los dos, para que el juego pregunte.
	const TArray<int32> Tied = FGraphMath::Nearest(NodeCount, Edges, S1, { S3, S5, S2 }, Q);
	TestEqual(TEXT("empate a 2 devuelve los dos candidatos"), Tied.Num(), 2);
	TestTrue(TEXT("el empate incluye S3"), Tied.Contains(S3));
	TestTrue(TEXT("el empate incluye S5"), Tied.Contains(S5));

	// Un solo candidato mas cercano no abre seleccion.
	const TArray<int32> Single = FGraphMath::Nearest(NodeCount, Edges, S1, { S4, S3 }, Q);
	TestEqual(TEXT("sin empate devuelve uno solo"), Single.Num(), 1);
	TestEqual(TEXT("y es S4, que esta a 1 por la escalera"), Single[0], (int32)S4);

	// F4: con Move = 3 desde S1, incluso S2 entra en el conjunto alcanzable.
	const TArray<int32> Within3 = FGraphMath::Reachable(NodeCount, Edges, S1, 3, Q);
	TestEqual(TEXT("con Move=3 se alcanzan los 7 espacios"), Within3.Num(), 7);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
