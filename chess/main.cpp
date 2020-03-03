#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"
#include "generator.h"
#include "ai.h"

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