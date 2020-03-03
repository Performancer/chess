#include "evaluation.h"
#include "state.h"

int getValue(char type, int x, int y)
{
	int value = 0;
	switch (type)
	{
		case KING:
		{
			value += king[x][y];
			break;
		}
		case QUEEN:
		{
			value = 900;
			value += queen[x][y];
			break;
		}
		case ROOK:
		{
			value = 500;
			value += rook[x][y];
			break;
		}
		case BISHOP:
		{
			value = 330;
			value += bishop[x][y];
			break;
		}
		case KNIGHT:
		{
			value = 320;
			value += knight[x][y];
			break;
		}
		case PAWN:
		{
			value = 100;
			value += pawn[x][y];
			break;
		}
	}

	return value;
}