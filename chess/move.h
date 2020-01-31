#pragma once

#define INVALID_MOVE 0
#define OFFICER_MOVE 1
#define PAWN_MOVE 2
#define CASTLING_QUEEN_SIDE 3
#define CASTLING_KING_SIDE 4

struct move
{
	char type = INVALID_MOVE;
	int from_x = 0;
	int from_y = 0;
	int to_x = 0;
	int to_y = 0;
};