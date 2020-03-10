#pragma once
#include "move.h"

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

	int whites = 16;
	int blacks = 16;
	struct Vector white_king = { 4, 0 };
	struct Vector black_king = { 4, 7 };

	bool white_can_castle_king_side = true;
	bool white_can_castle_queen_side = true;
	bool black_can_castle_king_side = true;
	bool black_can_castle_queen_side = true;

	//records if a pawn makes a move of two squares from its starting square
	struct Vector en_passant = { -1, -1 };

	void initialize();

	struct State simulate(struct Move move) const;
	bool isCheck(bool color);
	int evaluate() const;
};

char getType(char tile);
bool getColor(char tile);
bool isAlly(char a, char b);