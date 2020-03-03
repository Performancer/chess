#pragma once
#include "vector.h"
#include "state.h"

struct Move
{
	struct Vector from;
	struct Vector to;
};

struct EvaluatedMove
{
	int eval = 0;
	Move move;
};