#include "state.h"
#include "generator.h"
#include "move.h"
#include <math.h>

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

int State::evaluate()
{
	int white = 0;
	int black = 0;

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
				case KING: 
				{
					if (y == color ? 7 : 0)
						value += 2;
					
					if (x == 2 && y == color ? 7 : 0)
						value += 10;
					else if (x == 6 && color ? 7 : 0)
						value += 10;
					break;
				}
				case QUEEN: 
				{
					value += 9;
					value += (8 - (abs(x - 4) + abs(x - 4))) / 2;
					break;
				}
				case ROOK:
				{
					value = 5;

					if ((!color && y != 0) || (color && y != 7))
						value += 1;

					break;
				}
				case BISHOP:
				{
					value = 3.25;

					if ((!color && y != 0) || (color && y != 7))
						value += 1;

					break; 
				}
				case KNIGHT: 
				{
					value = 3;

					if (x > 2 && x < 6)
						value += 1;

					break; 
				}
				case PAWN:		value = 1;		break;
			}

			if (color)
				black += value;
			else
				white += value;
		}
	}

	return white - black;
}

struct Vector State::getKing(bool color)
{
	struct Vector king = { -1, -1 };

	//find the king
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			char tile = tiles[x][y];
			if (getType(tile) == KING && getColor(tile) == color)
				king = { x, y };
		}

	return king;
}

bool State::isCheckmate(bool color)
{
	struct Vector king = getKing(color);
	return king.isEqual(-1, -1);
}
