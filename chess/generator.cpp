#include "generator.h"
#include "movement.h"
#include "validation.h"

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

bool outsideBoard(struct Vector vector)
{
	return vector.x < 0 || vector.y < 0 || vector.x >= BOARD_SIZE || vector.y >= BOARD_SIZE;
}

std::vector<struct Vector> getKnightMoves(struct State* state, struct Vector from)
{
	std::vector<struct Vector> moves;

	for (int i = 0; i < 8; i++)
	{
		int x = ((i / 2) % 2 == 0) ? 2 : -2;
		int y = (i % 2 == 0) ? 1 : -1;

		if (i >= 4)
			swap(&x, &y);

		struct Vector to = { from.x + x, from.y + y };

		if (outsideBoard(to))
			continue;

		char tile = state->tiles[to.x][to.y];
		if (tile == EMPTY || !isAlly(state->tiles[from.x][from.y], tile))
			moves.push_back(to);
	}

	return moves;
}

std::vector<struct Vector> getBishopMoves(struct State* state, struct Vector from, int range)
{
	std::vector<struct Vector> moves;

	for (int direction = 0; direction < 4; direction++)
		for (int distance = 1; distance < range + 1; distance++)
		{
			int x = direction % 2 == 0 ? distance : -distance;
			int y = direction < 2 ? distance : -distance;
			Vector to = { from.x + x , from.y + y };

			if (outsideBoard(to))
				break;

			char tile = state->tiles[to.x][to.y];

			if (tile == EMPTY)
				moves.push_back(to);
			else
			{
				if (!isAlly(state->tiles[from.x][from.y], tile))
					moves.push_back(to);

				break;
			}
		}

	return moves;
}

std::vector<struct Vector> getRookMoves(struct State* state, struct Vector from, int range)
{
	std::vector<struct Vector> moves;

	for (int direction = 0; direction < 4; direction++)
		for (int distance = 1; distance < range + 1; distance++)
		{
			int x = direction % 2 == 0 ? distance : -distance;
			int y = 0;

			if (direction >= 2)
				swap(&x, &y);

			Vector to = { from.x + x , from.y + y};

			if (outsideBoard(to))
				break;

			char tile = state->tiles[to.x][to.y];

			if (tile == EMPTY)
				moves.push_back(to);
			else
			{
				if (!isAlly(state->tiles[from.x][from.y], tile))
					moves.push_back(to);

				break;
			}
		}

	return moves;
}

std::vector<struct Vector> getQueenMoves(struct State* state, struct Vector from, int range = BOARD_SIZE)
{
	std::vector<struct Vector> moves;
	for (struct Vector vector : getBishopMoves(state, from, range))
		moves.push_back(vector);
	for (struct Vector vector : getRookMoves(state, from, range))
		moves.push_back(vector);

	return moves;
}

std::vector<struct Vector> getKingMoves(struct State* state, struct Vector from)
{
	std::vector<struct Vector> moves = getQueenMoves(state, from, 1);

	//	1. The king and the chosen rook are on the player's first rank.
	//	2. Neither the king nor the chosen rook has previously moved.
	//	3. There are no pieces between the king and the chosen rook.
	//	4. The king is not currently in check.
	//	5. The king does not pass through a square that is attacked by an enemy piece.
	//	6. The king does not end up in check. (True of any legal move.)

	bool color = getColor(state->tiles[from.x][from.y]);

	if (!state->isCheck(color))
	{
		if (color ? state->black_can_castle_king_side : state->white_can_castle_king_side)
		{
			if (state->tiles[5][from.y] == EMPTY && state->tiles[6][from.y] == EMPTY && !isThreatened(state, { 5, from.y }, color))
				moves.push_back({ 6, from.y });
		}

		if (color ? state->black_can_castle_queen_side : state->white_can_castle_queen_side)
		{
			if (state->tiles[1][from.y] == EMPTY && state->tiles[2][from.y] == EMPTY && state->tiles[3][from.y] == EMPTY && !isThreatened(state, { 3, from.y }, color))
				moves.push_back({ 2, from.y });
		}
	}

	return moves;
}

bool pawnHasMoved(int y, bool is_black)
{
	if (is_black)
		return y != 6;

	return y != 1;
}

std::vector<struct Vector> getPawnMoves(struct State* state, struct Vector from)
{
	std::vector<struct Vector> moves;

	char piece = state->tiles[from.x][from.y];
	bool is_black = getColor(piece);	
	int y = from.y + (is_black ? -1 : 1);

	for (int x = from.x - 1; x < from.x + 2; x++)
	{
		if (outsideBoard({ x, y }))
			continue;

		if (x == from.x)
		{
			if (state->tiles[from.x][y] == EMPTY)
			{
				moves.push_back({ from.x, y }); //advance

				if (!pawnHasMoved(from.y, is_black) && state->tiles[from.x][y + (is_black ? -1 : 1)] == EMPTY)
					moves.push_back({ from.x, y + (is_black ? -1 : 1) }); //move two squares ahead
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

std::vector<struct Vector> getMoves(struct State* state, struct Vector from)
{
	std::vector<struct Vector> moves;

	char square = state->tiles[from.x][from.y];

	switch (getType(square))
	{
		case KING:		moves = getKingMoves(state, from);		break;
		case QUEEN:		moves = getQueenMoves(state, from);		break;
		case ROOK:		moves = getRookMoves(state, from);		break;
		case BISHOP:	moves = getBishopMoves(state, from);	break;
		case KNIGHT:	moves = getKnightMoves(state, from);	break;
		case PAWN:		moves = getPawnMoves(state, from);		break;
	}

	std::vector<struct Vector> validated;

	for (struct Vector to : moves)
	{
		State temp = state->simulate({ from, to });

		if (!temp.isCheck(getColor(square)))
			validated.push_back(to);
	}

	return validated;
}

std::vector<struct Move> getMoves(struct State* state, bool color)
{
	std::vector<struct Move> moves;

	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
			if (getColor(state->tiles[x][y]) == color) //AI can only move the color assigned
				for (struct Vector vector : getMoves(state, { x, y }))
					moves.push_back({ x, y, vector.x, vector.y });

	return moves;
}