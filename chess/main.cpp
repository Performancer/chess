#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"
#include "generator.h"

#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>

std::vector<struct Move> getPossibleMoves(struct State* state, bool color)
{
	std::vector<struct Move> moves;

	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			if (getColor(state->tiles[x][y]) == color) //AI can only move the color assigned
				for (struct Vector vector : getMoves(state, { x, y }))
					moves.push_back({ x, y, vector.x, vector.y });

	return moves;
}

int minimax(struct State* state, int depth, bool maximizingPlayer)
{
	if (state->isCheckmate(true))
		return 1000;  //black has lost
	else if (state->isCheckmate(false))
		return -1000; //white has lost

	if (depth == 0)
		return state->evaluate();

	std::vector<struct Move> moves = getPossibleMoves(state, maximizingPlayer);

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;

		for (Move move : moves)
		{
			struct State temp;
			state->copyState(&temp);
			executeMove(&temp, move);

			int eval = minimax(&temp, depth - 1, false);
			maxEval = std::max(maxEval, eval);
		}

		return maxEval;
	}
	else
	{
		int minEval = INT_MAX;

		for (Move move : moves)
		{
			struct State temp;
			state->copyState(&temp);
			executeMove(&temp, move);

			int eval = minimax(&temp, depth - 1, true);
			minEval = std::min(minEval, eval);
		}

		return minEval;
	}
}

Move getNextMove(struct State* state, bool color)
{
	std::vector<struct Move> moves = getPossibleMoves(state, color);

	if (moves.size() == 0)
		return { -1, -1, -1, -1 };

	int index = 0;
	int final_eval = color ? INT_MAX : INT_MIN;
	int minEval = INT_MAX;
	int maxEval = INT_MIN;

	for (int i = 0; i < moves.size(); i++)
	{
		struct State temp;
		state->copyState(&temp);
		executeMove(&temp, moves[i]);

		int eval = minimax(&temp, 3, color);

		minEval = std::min(minEval, eval);
		maxEval = std::max(maxEval, eval);

		if (color && eval < final_eval)
		{
			index = i;
			final_eval = eval;
		}
		else if (!color && eval > final_eval)
		{
			index = i;
			final_eval = eval;
		}
	}

	std::wcout << "MIN: " << minEval << std::endl;
	std::wcout << "MAX: " << maxEval << std::endl;
	std::wcout << "Evaluation: " << final_eval << std::endl;

	return moves[index];
}

int main()
{
	State state;
	state.initialize();

	bool player_color = true;

	Move last_move = { -1, -1, -1, -1 };
	int turn = 0;
	while(true)
	{
		draw(&state, last_move);

		bool color = turn % 2 != 0; //white = 0, black = 1

		if (color == player_color)
		{
			Move move = getMove(color);

			if (isLegalMove(&state, move))
			{
				executeMove(&state, move, true);
				last_move = move;
				turn++;
			}
		}
		else
		{
			Move move = getNextMove(&state, color);
			executeMove(&state, move, true);
			last_move = move;
			turn++;
		}
	}

	wprintf(L"The game has ended.");
	getchar();
}