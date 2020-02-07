#include "state.h"
#include "input.h"
#include "output.h"

bool white_can_castle_king_side = true;
bool white_can_castle_queen_side = true;
bool black_can_castle_king_side = true;
bool black_can_castle_queen_side = true;

void movePiece(struct State* state, int from_x, int from_y, int to_x, int to_y)
{
	char piece = state->tiles[from_x][from_y];
	state->tiles[from_x][from_y] = EMPTY;
	state->tiles[to_x][to_y]= piece;
}

void executeMove(struct State* state, move* move, bool is_white)
{
	if (move->type == OFFICER_MOVE)
	{
		movePiece(state, move->from_x, move->from_y, move->to_x, move->to_y);
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