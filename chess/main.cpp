#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"

int main()
{
	State state;
	state.initialize();

	int turn = 0;

	while(true)
	{
		draw(&state);

		Move move = getMove( turn % 2 != 0);

		if (isLegalMove(&state, move))
		{
			executeMove(&state, move);
			turn++;
		}
	}
}