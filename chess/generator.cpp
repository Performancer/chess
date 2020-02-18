#include "generator.h"
#include <iostream>

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

bool outsideBoard(int x, int y)
{
	return x < 0 || y < 0 || x >= 8 || y >= 8;
}

std::list<struct Vector*> getMoves(struct State* state, struct Vector from, char type, char range)
{
	char piece = state->tiles[from.x][from.y];
	
	if(type == EMPTY)
		type = getType(piece);

	bool color = getColor(piece);

	std::list<struct Vector*> moves;
	std::list<struct Vector*>  bonus;

	switch (type)
	{
	case KING:
		moves = getMoves(state, from, BISHOP, 2);
		bonus = getMoves(state, from, ROOK, 2);

		for(struct Vector* vector : bonus)
			moves.push_back(vector);

		if (color ? state->black_can_castle_king_side : state->white_can_castle_king_side)
		{
			Vector* vector = new Vector;
			vector->x = 6;
			vector->y = color ? 7 : 0;
			moves.push_back(vector);
		}
		
		if (color ? state->black_can_castle_queen_side : state->white_can_castle_queen_side)
		{
			Vector* vector = new Vector;
			vector->x = 2;
			vector->y = color ? 7 : 0;
			moves.push_back(vector);
		}
		break;
	case QUEEN:
		moves = getMoves(state, from, BISHOP, 8);
		bonus = getMoves(state, from, ROOK, 8);

		for (struct Vector* vector : bonus)
			moves.push_back(vector);
		break;
	case ROOK:
		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j < range; j++)
			{
				int x = (i % 2 == 0 ? j : -j);
				int y = 0;

				if(i >= 2 )
					swap(&x, &y);

				x += from.x;
				y += from.y;

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
				int x = from.x + ((i % 2 == 0) ? j : -j);
				int y = from.y + ((i < 2) ? j : -j);

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

			if (i >= 4)
				swap(&x, &y);

			x += from.x;
			y += from.y;

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

			int x = from.x;
			int y = from.y;

			if (two_step)
			{
				if (is_black && from.y != 6 || !is_black && from.y != 1)
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

			if (x == from.x)
			{
				if (tile == EMPTY)
				{
					//advance
					Vector *move = new Vector;
					*move = { x, y };
					moves.push_back(move);
				}
			}
			else
			{
				if (tile != EMPTY)
				{
					if (!isAlly(piece, tile))
					{
						//eat
						Vector *move = new Vector;
						*move = { x, y };
						moves.push_back(move);
					}
				}
				else if (getType(state->tiles[x][from.y]) == PAWN && !isAlly(piece, state->tiles[x][from.y]))
				{
					//en passant
					Vector *move = new Vector;
					*move = { x, y };
					moves.push_back(move);
				}
			}
		}

		break;
	}

	return moves;
}

bool isThreatened(struct State* state, Vector square, bool color)
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			char tile = state->tiles[x][y];

			if (tile != EMPTY && getColor(tile) != color)
			{
				Vector from = { x, y };
				std::list<Vector*> moves = getMoves(state, from, EMPTY, 8);

				for (Vector* move : moves)
				{
					if (move->x == square.x && move->y == square.y)
					{
						std::wcout << "Square " << "(" << move->x << ", " << move->y << ")" << " is threatened by (" << x << ", " << y << ")." << std::endl;
						return true;
					}
				}

				moves.clear();
			}
		}
	}

	return false;
}

bool isCheck(struct State* state, bool color)
{
	Vector king_location;

	//find the coordinates of our king
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			char tile = state->tiles[x][y];
			if (getType(tile) == KING && getColor(tile) == color)
				king_location = { x, y };
		}
	}

	return isThreatened(state, king_location, color);
}