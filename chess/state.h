#pragma once
#include "vector.h"

#define KING	0x0
#define QUEEN	0x1
#define ROOK	0x2
#define BISHOP	0x3
#define KNIGHT	0x4
#define PAWN	0x5
#define EMPTY	0x6
#define BLACK	0x8 //It is fast to compare values stored as a bitmask.

struct State
{
	char tiles[8][8];

	bool white_can_castle_king_side = true;
	bool white_can_castle_queen_side = true;
	bool black_can_castle_king_side = true;
	bool black_can_castle_queen_side = true;

	//records if a pawn makes a move of two squares from its starting square
	Vector en_passant = { -1, -1 };

	void initialize()
	{
		for (int x = 0; x < 8; x++)
			for (int y = 0; y < 8; y++)
				tiles[x][y] = EMPTY;

		tiles[0][0] = ROOK;
		tiles[1][0] = KNIGHT;
		tiles[2][0] = BISHOP;
		tiles[3][0] = QUEEN;
		tiles[4][0] = KING;
		tiles[5][0] = BISHOP;
		tiles[6][0] = KNIGHT;
		tiles[7][0] = ROOK;

		for (int x = 0; x < 8; x++)
			tiles[x][1] = PAWN;

		tiles[0][7] = ROOK		| BLACK;
		tiles[1][7] = KNIGHT	| BLACK;
		tiles[2][7] = BISHOP	| BLACK;
		tiles[3][7] = QUEEN		| BLACK;
		tiles[4][7] = KING		| BLACK;
		tiles[5][7] = BISHOP	| BLACK;
		tiles[6][7] = KNIGHT	| BLACK;
		tiles[7][7] = ROOK		| BLACK;

		for (int x = 0; x < 8; x++)
			tiles[x][6] = PAWN | BLACK;
	}

	void copyState(struct State* copyTo)
	{
		for (int x = 0; x < 8; x++)
			for (int y = 0; y < 8; y++)
				copyTo->tiles[x][y] = tiles[x][y];
	}
};

char getType(char tile);
bool getColor(char tile);
bool isAlly(char a, char b);