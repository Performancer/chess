#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"
#include "generator.h"
#include "ai.h"

#include <iostream>

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
			EvaluatedMove evad_move = getNextMove(&state, color);

			std::wcout << "Eval: "<< evad_move.eval << std::endl;

			/*std::vector<struct Vector> moves = getMoves(&state, evad_move.move.from);
			std::wcout << "Possible moves are: " << std::endl;
			for (auto move : moves)
				std::wcout << "   to (" << move.x << ", " << move.y << ")" << std::endl;
			*/

			executeMove(&state, evad_move.move, true);
			last_move = evad_move.move;
			turn++;
		}
	}

	wprintf(L"The game has ended.");
	getchar();
}