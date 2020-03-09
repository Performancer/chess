#include "validation.h"
#include "generator.h"
#include <vector>

bool isThreatened(struct State* state, struct Vector square, bool color)
{
	if (outsideBoard(square))
		return false;

	std::vector<struct Vector> vector = getBishopMoves(state, square);
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

	int direction = color ? -1 : 1;

	char tile = state->tiles[square.x - 1][square.y + direction];
	if (tile != EMPTY && getColor(tile) != color && getType(tile) == PAWN)
		return true;

	tile = state->tiles[square.x + 1][square.y + direction];
	if (tile != EMPTY && getColor(tile) != color && getType(tile) == PAWN)
		return true;

	//We do not take en passe into account, because we never call this function for a pawn

	return false;
}
