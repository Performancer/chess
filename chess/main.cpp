#include "state.h"
#include "input.h"
#include "output.h"
#include "movement.h"

int main()
{
	State state;
	state.initialize();

	while(true)
	{
		draw(&state);

		Move move = getMove();

		if (move.type != NORMAL_MOVE || isLegalMove(&state, move))
			executeMove(&state, move, true);
	}
}