#include "ai.h"
#include "generator.h"
#include "movement.h"

#include <limits>
#include <vector>
#include <algorithm>
#include <iostream>

EvaluatedMove minimax(struct State* state, int depth, bool is_black)
{
	if (state->isCheckmate(true))
		return { 1000, -1, -1, -1, -1 };
	else if (state->isCheckmate(false))
		return { -1000, -1, -1, -1, -1 };
	else if (depth == 0)
		return { state->evaluate(), -1, -1, -1, -1 };

	std::vector<struct Move> moves = getMoves(state, is_black);

	EvaluatedMove best_move = { is_black ? INT_MAX : INT_MIN, -1, -1, -1, -1 };

	for (Move move : moves)
	{
		struct State temp;
		state->copyState(&temp);
		executeMove(&temp, move);

		EvaluatedMove evaluated_move = minimax(&temp, depth - 1, !is_black);

		if (is_black && evaluated_move.eval < best_move.eval)
			best_move = { evaluated_move.eval, move };
		else if (!is_black && evaluated_move.eval > best_move.eval)
			best_move = { evaluated_move.eval, move };
	}

	return best_move;
}

Move getNextMove(struct State* state, bool color)
{
	return minimax(state, 4, color).move;
}
