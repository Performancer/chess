#pragma once
#include "vector.h"

#define KING	0x0
#define QUEEN	0x1
#define ROOK	0x2
#define BISHOP	0x3
#define KNIGHT	0x4
#define PAWN	0x5
#define EMPTY	0x6
#define BLACK	0x8 //it is faster to compare values stored as a bitmask

#define BOARD_SIZE 8

struct State
{
	char tiles[8][8];

	bool white_can_castle_king_side = true;
	bool white_can_castle_queen_side = true;
	bool black_can_castle_king_side = true;
	bool black_can_castle_queen_side = true;

	//records if a pawn makes a move of two squares from its starting square
	Vector en_passant = { -1, -1 };

	void initialize();
	void copyState(struct State* copyTo);

	double evaluate();
};

char getType(char tile);
bool getColor(char tile);
bool isAlly(char a, char b);