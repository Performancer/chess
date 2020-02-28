#include "generator.h"
#include "movement.h"
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
	{
		if (state->tiles[5][from.y] == EMPTY && state->tiles[6][from.y] == EMPTY && !isThreatened(state, { 5, from.y }, color))
			moves.push_back({ 6, color ? 7 : 0 });
	}

	if (color ? state->black_can_castle_queen_side : state->white_can_castle_queen_side)
	{
		if (state->tiles[1][from.y] == EMPTY && state->tiles[2][from.y] == EMPTY && state->tiles[3][from.y] == EMPTY && !isThreatened(state, { 3, from.y }, color))
			moves.push_back({ 2, color ? 7 : 0 });
	}

	return moves;
}

bool pawnHasMoved(int y, bool is_black)
{
	return is_black && y == 6 && !is_black && y == 1;
}

std::list<struct Vector> getPawnMoves(struct State* state, struct Vector from)
{
	std::list<struct Vector> moves;

	char piece = state->tiles[from.x][from.y];
	bool is_black = getColor(piece);	
	int y = from.y + (is_black ? -1 : 1);

	for (int x = from.x - 1; x < from.x + 2; x++)
	{
		if (outsideBoard(x, y))
			continue;

		if (x == from.x)
		{
			if (state->tiles[from.x][from.y + y] == EMPTY)
			{
				moves.push_back({ from.x, y }); //advance

				if (!pawnHasMoved(from.y, is_black) && state->tiles[from.x][y + 1] == EMPTY)
					moves.push_back({ from.x, y + 1 }); //move two squares ahead
			}
		}
		else
		{
			char target = state->tiles[x][y];
			char side = state->tiles[x][from.y];

			if ((target != EMPTY && !isAlly(piece, target)) || (target == EMPTY && state->en_passant.isEqual(x, from.y) && !isAlly(piece, side)))
				moves.push_back({ x, y }); //eat
		}
	}

	return moves;
}

std::list<struct Vector> getMoves(struct State* state, struct Vector from)
{
	std::list<struct Vector> moves;

	char square = state->tiles[from.x][from.y];

	switch (getType(state->tiles[from.x][from.y]))
	{
		case KING:		moves = getKingMoves(state, from);		break;
		case QUEEN:		moves = getQueenMoves(state, from);		break;
		case ROOK:		moves = getRookMoves(state, from);		break;
		case BISHOP:	moves = getBishopMoves(state, from);	break;
		case KNIGHT:	moves = getKnightMoves(state, from);	break;
		case PAWN:		moves = getPawnMoves(state, from);		break;
	}

	std::list<struct Vector> validated;

	for (struct Vector to : moves)
	{
		State temp;
		state->copyState(&temp);
		executeMove(&temp, { from, to });

		if (!isCheck(&temp, getColor(square)))
			validated.push_back(to);
	}

	return validated;
}

bool isThreatened(struct State* state, struct Vector square, bool color)
{
	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			char tile = state->tiles[x][y];

			if (tile != EMPTY && getColor(tile) != color)
			{
				struct Vector from = { x, y };

				std::list<struct Vector> vector = getBishopMoves(state, square);
				for (struct Vector from : vector)
				{
					char tile = state->tiles[from.x][from.y];

					if (tile == EMPTY)
						continue;

					char type = getType(tile);

					if (getColor(tile) != color && (type == BISHOP || type == QUEEN || (type == KING && abs(square.x - from.x) <= 1 && abs(square.y - from.y) <= 1)))
						return true;
				}

				vector = getRookMoves(state, square);
				for (struct Vector from : vector)
				{
					char tile = state->tiles[from.x][from.y];

					if (tile == EMPTY)
						continue;

					char type = getType(tile);

					if (getColor(tile) != color && (type == ROOK || type == QUEEN || (type == KING && abs(square.x - from.x) <= 1 && abs(square.y - from.y) <= 1)))
						return true;
				}

				vector = getKnightMoves(state, square);
				for (struct Vector from : vector)
				{
					char tile = state->tiles[from.x][from.y];

					if (tile != EMPTY && getColor(tile) != color && getType(tile) == KNIGHT)
						return true;
				}

				vector = getPawnMoves(state, square);
				for (struct Vector from : vector)
				{
					int direction = color ? -1 : 1;

					char tile = state->tiles[from.x - 1][from.y + direction];
					if (tile != EMPTY && getColor(tile) != color && getType(tile) == PAWN)
						return true;

					tile = state->tiles[from.x + 1][from.y + direction];
					if (tile != EMPTY && getColor(tile) != color && getType(tile) == PAWN)
						return true;
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
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			char tile = state->tiles[x][y];
			if (getType(tile) == KING && getColor(tile) == color)
				square = { x, y };
		}

	return isThreatened(state, square, color);
}