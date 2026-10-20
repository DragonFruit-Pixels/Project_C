#pragma once

#include "Map/GraphMath.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace ProjectCTest
{
	enum : int32 { S1 = 0, S2, S3, S4, S5, S6, S7, NodeCount };

	inline TArray<FGraphEdge> ExampleGraph()
	{
		return {
			FGraphEdge(S1, S2, true),
			FGraphEdge(S2, S3),
			FGraphEdge(S3, S4),
			FGraphEdge(S4, S5),
			FGraphEdge(S1, S6),
			FGraphEdge(S6, S7),
			FGraphEdge(S1, S4),
		};
	}
}

#endif
