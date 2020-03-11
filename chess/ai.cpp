#include "ai.h"
#include "generator.h"
#include "movement.h"
#include "clock.h"
#include "transposition.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <map>

#define LIMIT 8

static Clock clock;
static int table_uses = 0;

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
	int hash_key = transposition::hash(state);
	
	if (transposition::exists(hash_key))
	{
		Transposition value = transposition::get(hash_key);
		
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
		transposition::add(hash_key, { depth, alpha, beta });
		return { eval, -1, -1, -1, -1 };
	}

	std::vector<struct Move> moves = getMoves(state, color);

	EvaluatedMove best_move = { color ? INT_MAX : INT_MIN, -1, -1, -1, -1 };

	for (Move move : moves)
	{
		if (clock.getSeconds() >= LIMIT)
			return { -1, -1, -1, -1, -1 };

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

	if (!transposition::exists(hash_key))
		transposition::add(hash_key, { depth, alpha, beta });

	return best_move;
}

EvaluatedMove getNextMove(struct State* state, bool color)
{

	if (!color)
		return minimax(state, 2, INT_MIN, INT_MAX, color);

	clock.start();
	table_uses = 0;

	int hash_key = transposition::hash(state);
	wprintf(L"Hash: %d\n", hash_key);

	int depth = 4;
	EvaluatedMove move = minimaxWithMemory(state, depth, INT_MIN, INT_MAX, color);

	while (clock.getSeconds() < LIMIT)
	{
		wprintf(L"Depth: %d\n", depth);
		EvaluatedMove deeper = minimaxWithMemory(state, depth + 2, INT_MIN, INT_MAX, color);

		if (clock.getSeconds() >= LIMIT)
			break;

		move = deeper;
		depth += 2;
	}

	wprintf(L"Transposition table has been used %d times this turn.\n", table_uses);
	return move;
}
