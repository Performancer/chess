#pragma once
#include "vector.h"

#define NORMAL_MOVE 0x0
#define CASTLING_QUEEN_SIDE 0x1
#define CASTLING_KING_SIDE 0x2

struct Move
{
	char type;
	struct Vector from;
	struct Vector to;
};