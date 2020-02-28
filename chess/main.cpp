#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"
#include "generator.h"

#include <vector>

int main()
{
	State state;
	state.initialize();

	bool player_color = false;

	Move last_move = { -1, -1, -1, -1 };
	int turn = 0;
	while(true)
	{
		draw(&state, last_move);

		bool color = turn % 2 != 0; //white = 0, black = 1

		if (player_color)
		{
			std::vector<struct Move> moves;
			// Get all the possible moves that can be made
			for (int x = 0; x < BOARD_SIZE; x++)
				for (int y = 0; y < BOARD_SIZE; y++)
					if (getColor(state.tiles[x][y]) == color) // AI can only move the color assigned
						for (struct Vector vector : getMoves(&state, { x, y }))
							moves.push_back({ x, y, vector.x, vector.y });

			//TODO: alphabeta

			Move move = moves[std::rand() % moves.size()];
			executeMove(&state, move, true);
			last_move = move;
			turn++;
		}
		else
		{
			Move move = getMove(color);

			if (isLegalMove(&state, move))
			{
				executeMove(&state, move, true);
				last_move = move;
				turn++;
			}
		}
	}
}