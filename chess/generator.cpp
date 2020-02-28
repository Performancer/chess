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
	return x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE;
}

bool isValidTile(struct State* state, struct Vector from, struct Vector to)
{
	if (!outsideBoard(to.x, to.y))
	{
		char tile = state->tiles[to.x][to.y];
		if (tile == EMPTY || !isAlly(state->tiles[from.x][from.y], tile))
			return true;
	}

	return false;
}

std::list<struct Vector> getKnightMoves(struct State* state, struct Vector from)
{
	std::list<struct Vector> moves;

	for (int i = 0; i < 8; i++)
	{
		int x = ((i / 2) % 2 == 0) ? 2 : -2;
		int y = (i % 2 == 0) ? 1 : -1;

		if (i >= 4)
			swap(&x, &y);

		struct Vector to = { from.x + x, from.y + y };

		if (isValidTile(state, from, to))
			moves.push_back(to);
	}

	return moves;
}

std::list<struct Vector> getBishopMoves(struct State* state, struct Vector from, int range = BOARD_SIZE)
{
	std::list<struct Vector> moves;

	for (int direction = 0; direction < 4; direction++)
		for (int distance = 1; distance < range + 1; distance++)
		{
			int x = direction % 2 == 0 ? distance : -distance;
			int y = direction < 2 ? distance : -distance;
			Vector to = { from.x + x , from.y + y };

			if (!isValidTile(state, from, to))
				break;
				
			moves.push_back(to);
		}

	return moves;
}

std::list<struct Vector> getRookMoves(struct State* state, struct Vector from, int range = BOARD_SIZE)
{
	std::list<struct Vector> moves;

	for (int direction = 0; direction < 4; direction++)
		for (int distance = 1; distance < range + 1; distance++)
		{
			int x = direction % 2 == 0 ? distance : -distance;
			int y = 0;

			if (direction >= 2)
				swap(&x, &y);

			Vector to = { from.x + x , from.y + y};

			if (!isValidTile(state, from, to))
				break;

			moves.push_back(to);
		}

	return moves;
}

std::list<struct Vector> getQueenMoves(struct State* state, struct Vector from, int range = BOARD_SIZE)
{
	std::list<struct Vector> moves;
	for (struct Vector vector : getBishopMoves(state, from, range))
		moves.push_back(vector);
	for (struct Vector vector : getRookMoves(state, from, range))
		moves.push_back(vector);

	return moves;
}

std::list<struct Vector> getKingMoves(struct State* state, struct Vector from)
{
	std::list<struct Vector> moves = getQueenMoves(state, from, 1);

	bool color = getColor(state->tiles[from.x][from.y]);
	if (color ? state->black_can_castle_king_side : state->white_can_castle_king_side)
		moves.push_back({ 6, color ? 7 : 0 });
	if (color ? state->black_can_castle_queen_side : state->white_can_castle_queen_side)
		moves.push_back({2, color ? 7 : 0 });

	return moves;
}

std::list<struct Vector> getPawnMoves(struct State* state, struct Vector from)
{
	std::list<struct Vector> moves;
	char piece = state->tiles[from.x][from.y];

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
				moves.push_back({ x, y }); //advance
		}
		else
		{
			if (tile != EMPTY)
			{
				if (!isAlly(piece, tile))
					moves.push_back({ x, y }); //eat
			}
			else if (getType(state->tiles[x][from.y]) == PAWN && !isAlly(piece, state->tiles[x][from.y]))
				moves.push_back({ x, y }); //en passant
		}
	}

	return moves;
}

std::list<struct Vector> getMoves(struct State* state, struct Vector from)
{
	std::list<struct Vector> moves;

	switch (getType(state->tiles[from.x][from.y]))
	{
		case KING:		moves = getKingMoves(state, from);		break;
		case QUEEN:		moves = getQueenMoves(state, from);		break;
		case ROOK:		moves = getRookMoves(state, from);		break;
		case BISHOP:	moves = getBishopMoves(state, from);	break;
		case KNIGHT:	moves = getKnightMoves(state, from);	break;
		case PAWN:		moves = getPawnMoves(state, from);		break;
	}

	return moves;
}

bool isThreatened(struct State* state, struct Vector square, bool color)
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			char tile = state->tiles[x][y];

			if (tile != EMPTY && getColor(tile) != color)
			{
				struct Vector from = { x, y };
				std::list<struct Vector> moves = getMoves(state, from);

				for (struct Vector move : moves)
				{
					if (move.x == square.x && move.y == square.y)
					{
						std::wcout << "Square " << "(" << move.x << ", " << move.y << ")" << " is threatened by (" << x << ", " << y << ")." << std::endl;
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool isCheck(struct State* state, bool color)
{
	struct Vector square;

	//find the coordinates of the king
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			char tile = state->tiles[x][y];
			if (getType(tile) == KING && getColor(tile) == color)
				square = { x, y };
		}
	}

	return isThreatened(state, square, color);
}