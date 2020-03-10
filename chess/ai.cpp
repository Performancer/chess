#include "ai.h"
#include "generator.h"
#include "movement.h"
#include "transposition.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <map>

int table_uses = 0;

EvaluatedMove minimax(struct State* state, int depth, int alpha, int beta, bool color)
{
	if (depth == 0)
		return { state->evaluate(), -1, -1, -1, -1 };

	std::vector<struct Move> moves = getMoves(state, color);

	EvaluatedMove best_move = { color ? INT_MAX : INT_MIN, -1, -1, -1, -1 };

	for (Move move : moves)
	{
		struct State temp = state->simulate(move);

		EvaluatedMove evad_move = minimax(&temp, depth - 1, alpha, beta, !color);

		if (color)
		{
			if (evad_move.eval < best_move.eval)
				best_move = { evad_move.eval, move };

			beta = std::min(beta, evad_move.eval);
		}
		else
		{
			if (evad_move.eval > best_move.eval)
				best_move = { evad_move.eval, move };

			alpha = std::max(alpha, evad_move.eval);
		}

		if (beta <= alpha)
			break;
	}

	return best_move;
}


EvaluatedMove minimaxWithMemory(struct State* state, int depth, int alpha, int beta, bool color)
{
	int hash_key = hash(state);
	
	if (transpositionExists(hash_key))
	{
		Transposition value = getTransposition(hash_key);
		
		if (value.depth >= depth)
		{
			table_uses++;

			if(!color)
				alpha = std::max(alpha, value.upper);
			else
				beta = std::min(beta, value.lower);
		}
	}

	if (depth == 0)
	{
		int eval = state->evaluate();
		addTransposition(hash_key, { depth, alpha, beta });
		return { eval, -1, -1, -1, -1 };
	}

	std::vector<struct Move> moves = getMoves(state, color);

	EvaluatedMove best_move = { color ? INT_MAX : INT_MIN, -1, -1, -1, -1 };

	for (Move move : moves)
	{
		struct State temp = state->simulate(move);

		EvaluatedMove evad_move = minimaxWithMemory(&temp, depth - 1, alpha, beta, !color);

		if (color)
		{
			if (evad_move.eval < best_move.eval)
				best_move = { evad_move.eval, move};

			beta = std::min(beta, evad_move.eval);
		}
		else
		{
			if(evad_move.eval > best_move.eval)
				best_move = { evad_move.eval, move};

			alpha = std::max(alpha, evad_move.eval);
		}

		if (beta <= alpha)
			break;
	}

	if (!transpositionExists(hash_key))
		addTransposition(hash_key, { depth, alpha, beta });

	return best_move;
}

EvaluatedMove getNextMove(struct State* state, bool color)
{
	table_uses = 0;
	int hash_key = hash(state);
	int depth;

	int pieces = state->blacks + state->whites;

	if (pieces > 16)
		depth = 4;
	else if (pieces > 8)
		depth = 6;
	else  if (pieces > 4)
		depth = 8;
	else
		depth = 10;

	wprintf(L"Hash: %d\n", hash_key);
	wprintf(L"Depth: %d\n", depth);

	EvaluatedMove move = !color ? minimaxWithMemory(state, 4, INT_MIN, INT_MAX, color) : minimax(state, 4, INT_MIN, INT_MAX, color);
	wprintf(L"Transposition table has been used %d times this turn.\n", table_uses);
	return move;
}
