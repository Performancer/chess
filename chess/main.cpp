#include "clock.h"
#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"
#include "generator.h"
#include "ai.h"
#include "transposition.h"

void printPossibleMoves(struct State* state, struct Vector from)
{
	std::vector<struct Vector> moves;
	getMoves(moves, state, from);

	wprintf(L"Possible moves are:\n");
	for (auto move : moves)
		wprintf(L"   to (%d, %d)\n", move.x, move.y);
}

int main()
{
	State state;
	state.initialize();

	int turn = 0;
	Clock chess_clock;
	Move last_move = { -1, -1, -1, -1 };
	bool player_color = false;

	Clock total;
	total.start();

	wprintf(L"GAME IS STARTING: THIS MESSAGE IS USED TO DISMORAL THE OPPONENT\n\n");

	while(true)
	{
		chess_clock.start();
		draw(&state, last_move);

		bool color = turn % 2 != 0;
		wprintf(L"------- TURN %d - %s -------\n", (turn + 1), (color ? L"BLACK" : L"WHITE"));

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

			if (evad_move.move.from.isEqual(-1, -1))
				break;

			wprintf(L"Evaluation: %d\n", evad_move.eval);

			executeMove(&state, evad_move.move, true);
			last_move = evad_move.move;
			turn++;
		}

		wprintf(L"Last frame took %ld.%03d seconds.\n", chess_clock.getSeconds(), chess_clock.getMilliseconds());
		wprintf(L"There are %d pieces left.\n", state.whites + state.blacks);
	}

	wprintf(L"The game has ended. Process took %ld.%03d seconds in total.\n", total.getSeconds(), total.getMilliseconds());
	system("pause");
}