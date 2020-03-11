#include "ai.h"
#include "generator.h"
#include "movement.h"
#include "clock.h"
#include "transposition.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <map>

#define LIMIT 10

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
		if (clock.getSeconds() >= LIMIT)
			return { -1, -1, -1, -1, -1 };

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
	transposition::UINT64 hash_key = transposition::hash(state);
	
	if (transposition::exists(hash_key))
	{
		Transposition value = transposition::get(hash_key);
		
		if (value.depth == depth || (value.depth > depth && value.depth % 2 == depth % 2) )
		{
			table_uses++;
			return value.move;
		}
	}

	if (depth == 0)
	{
		EvaluatedMove move = { state->evaluate(), -1, -1, -1, -1 };
		transposition::add(hash_key, { depth, move });
		return move;
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
		transposition::add(hash_key, { depth, best_move });

	return best_move;
}

EvaluatedMove getNextMove(struct State* state, bool color)
{
	clock.start();
	table_uses = 0;

	if (color)
		return minimax(state, 4, INT_MIN, INT_MAX, color);

	transposition::UINT64 hash_key = transposition::hash(state);
	wprintf(L"Hash: %llu\n", hash_key);

	int depth = 4;
	EvaluatedMove move = minimaxWithMemory(state, depth, INT_MIN, INT_MAX, color);
	
	wprintf(L"Depth: %d\n", depth);
	wprintf(L"Processing depth 6... ");
	EvaluatedMove deeper = minimaxWithMemory(state, depth + 2, INT_MIN, INT_MAX, color);

	if (clock.getSeconds() < LIMIT)
	{
		move = deeper;
		depth += 2;
		wprintf(L"done.\n");
	}
	else
	{
		wprintf(L"falling back to depth 4.\n");
	}

	wprintf(L"Transposition table has been used %d times this turn.\n", table_uses);
	return move;
}
