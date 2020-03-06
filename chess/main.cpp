#include "clock.h"
#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"
#include "generator.h"
#include "ai.h"
#include "transposition.h"

int main()
{
	initialize_zobrist();

	State state;
	state.initialize();

	int turn = 0;
	Move last_move = { -1, -1, -1, -1 };
	bool player_color = false;
	int seconds, militm;

	while(true)
	{
		startClock();
		draw(&state, last_move);

		bool color = turn % 2 != 0; //white = 0, black = 1

		if (false)
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

			if (evad_move.move.from.isEqual(-1, -1))
				break;

			wprintf(L"Turn: %d Eval: %d\n", (turn + 1), evad_move.eval);

			/*std::vector<struct Vector> moves = getMoves(&state, evad_move.move.from);
			std::wcout << "Possible moves are: " << std::endl;
			for (auto move : moves)
				std::wcout << "   to (" << move.x << ", " << move.y << ")" << std::endl;
			*/

			executeMove(&state, evad_move.move, true);
			last_move = evad_move.move;
			turn++;
		}

		stopClock(&seconds, &militm);
		wprintf(L"Last frame took %ld.%03d seconds.\n", seconds, militm);
	}

	wprintf(L"The game has ended.");
	getchar();
}