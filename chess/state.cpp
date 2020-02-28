#include "state.h"

char getType(char tile)
{
	return (tile | BLACK) ^ BLACK;
}

bool getColor(char tile)
{
	return  (tile & BLACK) == BLACK;
}

bool isAlly(char a, char b)
{
	return (a & BLACK) == (b & BLACK);
}

void State::initialize()
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

void State::copyState(struct State* copyTo)
{
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			copyTo->tiles[x][y] = tiles[x][y];
}

double State::evaluate()
{
	double white = 0;
	double black = 0;

	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			char tile = tiles[x][y];

			if (tile == EMPTY)
				continue;

			char type = getType(tile);
			bool color = getColor(tile);

			double value = 0;

			switch (type)
			{
				case QUEEN:		value = 9;		break;
				case ROOK:		value = 5;		break;
				case BISHOP:	value = 3.25;	break;
				case KNIGHT:	value = 3;		break;
				case PAWN:		value = 1;		break;
			}

			if (color)
				black += value;
			else
				white += value;
		}
	}

	if (white == 0)
		white = 0.1;

	return black / white;
}
