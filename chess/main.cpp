#include "state.h"
#include "input.h"
#include "output.h"
#include "generator.h"

void movePiece(struct State* state, int from_x, int from_y, int to_x, int to_y)
{
	char piece = state->tiles[from_x][from_y];
	state->tiles[from_x][from_y] = EMPTY;
	state->tiles[to_x][to_y]= piece;
}

bool isLegalMove(struct State* state, Vector* from, Vector* to)
{
	for (Vector* vector : getMoves(state, from, EMPTY, 8))
		if (vector->x == to->x && vector->y == to->y)
			return true;

	return false;
}

bool executeMove(struct State* state, move* move, bool is_white)
{
	if (move->type == OFFICER_MOVE)
	{
		Vector from = { move->from_x, move->from_y };
		Vector to = { move->to_x, move->to_y };

		bool legal = isLegalMove(state, &from, &to);

		if(legal)
			movePiece(state, move->from_x, move->from_y, move->to_x, move->to_y);

		return legal;
	}
	else if (move->type == CASTLING_QUEEN_SIDE)
	{
		int y = is_white ? 0 : 7;
		movePiece(state, 4, y, 2, y);
		movePiece(state, 0, y, 3, y);
	}
	else if (move->type == CASTLING_KING_SIDE)
	{
		int y = is_white ? 0 : 7;
		movePiece(state, 4, y, 6, y);
		movePiece(state, 7, y, 5, y);
	}
	else if (move->type == PAWN_MOVE)
	{
		//TODO: find out which pawn is moving
	}

	delete move;
}

int main()
{
	State state;
	state.initialize();

	while(true)
	{
		draw(&state);
		executeMove(&state, getMove(), true);
	}
}