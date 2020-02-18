#pragma once
#include "vector.h"
#include "state.h"

struct Move
{
	struct Vector from;
	struct Vector to;

	bool isCastling(struct State* state)
	{
		char piece = state->tiles[from.x][from.y];
		char type = getType(piece);
		bool color = getColor(piece);

		if (type == KING && from.x == 4 && from.y == (color ? 7 : 0))
		{
			if (to.y == from.y && (to.x == 2 || to.x == 6))
				return true;
		}

		return false;
	}
};