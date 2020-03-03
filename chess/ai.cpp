#include "ai.h"
#include "generator.h"
#include "movement.h"

#include <limits>
#include <vector>
#include <algorithm>
#include <iostream>

EvaluatedMove minimax(struct State* state, int depth, int alpha, int beta, bool is_black)
{
	if (!state->has_black_king)
		return { 20000, -1, -1, -1, -1 };
	else if (!state->has_white_king)
		return { -20000, -1, -1, -1, -1 };
	else if (depth == 0)
		return { state->evaluate(), -1, -1, -1, -1 };

	std::vector<struct Move> moves = getMoves(state, is_black);

	EvaluatedMove best_move = { is_black ? INT_MAX : INT_MIN, -1, -1, -1, -1 };

	for (Move move : moves)
	{
		struct State temp;
		state->copyState(&temp);
		executeMove(&temp, move);

		EvaluatedMove evad_move = minimax(&temp, depth - 1, alpha, beta, !is_black);

		if (is_black)
		{
			if (evad_move.eval < best_move.eval)
				best_move = { evad_move.eval, move };

			beta = std::min(beta, evad_move.eval);
		}
		else
		{
			if(evad_move.eval > best_move.eval)
				best_move = { evad_move.eval, move };

			alpha = std::max(alpha, evad_move.eval);
		}

		if (beta <= alpha)
			break;
	}

	/*for(int i = 0; i < (6 - depth); i++ )
		std::wcout << "  ";

	std::wcout << "depth: " << depth << " " << "eval: " << best_move.eval << std::endl; */

	return best_move;
}


EvaluatedMove getNextMove(struct State* state, bool color)
{
	return minimax(state, 4, INT_MIN, INT_MAX, color);
}
