#include <list>
#include "state.h"
#include "vector.h"

void swap(int* a, int* b)
{
	int* temp = a;
	a = b;
	b = temp;
}

bool outsideBoard(int x, int y)
{
	return x < 0 || y < 0 || x >= 8 || y >= 8;
}

std::list<struct Vector*> getMoves(struct State* state, struct Vector* from, char type, char range)
{
	char piece = state->tiles[from->x][from->y];
	
	if(type == EMPTY)
		type = getType(piece);

	std::list<struct Vector*> moves;

	switch (type)
	{
	case KING:

		//TODO: castling

		moves = getMoves(state, from, BISHOP, 1);
		moves.merge(getMoves(state, from, ROOK, 1));
		break;
	case QUEEN:
		moves = getMoves(state, from, BISHOP, 8);
		moves.merge(getMoves(state, from, ROOK, 8));
		break;
	case ROOK:
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j < range; j++)
			{
				int x = from->x + (i < 2 ? j : 0)	* (i % 2 == 0 ? 1 : -1);
				int y = from->y + (i >= 2 ? j : 0)	* (i % 2 == 0 ? 1 : -1);

				if(i >= 2 )
					swap(&x, &y);

				if (outsideBoard(x, y))
					break;

				char tile = state->tiles[x][y];

				if (tile != EMPTY)
				{
					if (!isAlly(piece, tile))
					{
						Vector *move = new Vector;
						*move = { x, y };
						moves.push_back(move);
					}
					break;
				}

				Vector *move = new Vector;
				*move = { x, y };
				moves.push_back(move);
			}
		}
		break;
	case BISHOP: 
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j < range; j++)
			{
				int x = from->x + ((i % 2 == 0) ? j : -j);
				int y = from->y + ((i < 2) ? j : -j);

				if (outsideBoard(x, y))
					break;

				char tile = state->tiles[x][y];

				if (tile != EMPTY)
				{
					if (!isAlly(piece, tile))
					{
						Vector *move = new Vector;
						*move = { x, y };
						moves.push_back(move);
					}
					break;
				}

				Vector *move = new Vector;
				*move = { x, y };
				moves.push_back(move);
			}
		}
		break;
	case KNIGHT:
		for (int i = 0; i < 8; i++)
		{
			int x = ((i / 2) % 2 == 0) ? 2 : -2;
			int y = (i % 2 == 0) ? 1 : -1;

			if (x >= 4)
				swap(&x, &y);

			x += from->x;
			y += from->y;

			if (outsideBoard(x, y))
				continue;

			char tile = state->tiles[x][y];

			if (tile == EMPTY || !isAlly(piece, tile))
			{
				Vector *move = new Vector;
				*move = { x, y };
				moves.push_back(move);
			}
		}
		break;
	case PAWN:
		for (int i = 0; i < 4; i++)
		{
			bool is_black = getColor(piece);
			bool two_step = i == 3;

			int x = from->x;
			int y = from->y;

			if (two_step)
			{
				if (is_black && from->y != 6 || !is_black && from->y != 1)
					continue;

				y += is_black ? -2 : 2;
			}
			else
			{
				x += i - 1;
				y += is_black ? -1 : 1;
			}

			if (outsideBoard(x, y))
				continue;

			char tile = state->tiles[x][y];

			if (x == from->x && tile == EMPTY || !isAlly(piece, tile))
			{
				Vector *move = new Vector;
				*move = { x, y };
				moves.push_back(move);
			}
		}

		break;
	}

	return moves;
}