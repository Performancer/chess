#include "state.h"
#include "input.h"
#include "output.h"
#include "generator.h"
#include "move.h"

#include <iostream>

void movePiece(struct State* state, struct Vector from, struct Vector to)
{
	char piece = state->tiles[from.x][from.y];
	state->tiles[from.x][from.y] = EMPTY;
	state->tiles[to.x][to.y] = piece;
}

void executeMove(struct State* state, Move move, bool color)
{
	if (move.type == NORMAL_MOVE)
	{
		std::wcout << "moving (" << move.from.x << ", " << move.from.y  <<  ") to (" << move.to.x << ", " << move.to.y << ")" << std::endl;
		movePiece(state, move.from, move.to);

		char piece = state->tiles[move.from.x][move.from.y];

		//EN PASSANT
		if (getType(piece) == PAWN && move.from.x != move.to.x && state->tiles[move.to.x][move.to.y] == EMPTY)
		{
			//it seems we are doing en passant
			char passer = state->tiles[move.to.x][move.from.y];

			if (getType(passer) == PAWN && !isAlly(piece, passer))
				state->tiles[move.to.x][move.from.y] = EMPTY;
		}
	}
	else if (move.type == CASTLING_QUEEN_SIDE)
	{
		int y = color ? 7 : 0;
		movePiece(state, { 4, y }, { 2, y });
		movePiece(state, { 0, y }, { 3, y });

		if (color) //white = 0, black = 1
		{
			state->black_can_castle_king_side = false;
			state->black_can_castle_queen_side = false;
		}
		else
		{
			state->white_can_castle_king_side = false;
			state->white_can_castle_queen_side = false;
		}
	}
	else if (move.type == CASTLING_KING_SIDE)
	{
		int y = color ? 7 : 0;
		movePiece(state, { 4, y }, { 6, y });
		movePiece(state, { 7, y }, { 5, y });

		if (color) //white = 0, black = 1
		{
			state->black_can_castle_king_side = false;
			state->black_can_castle_queen_side = false;
		}
		else
		{
			state->white_can_castle_king_side = false;
			state->white_can_castle_queen_side = false;
		}
	}
}

bool isLegalMove(struct State* state, Move move)
{
	for (Vector* vector : getMoves(state, move.from, EMPTY, 8))
		if (*vector == move.to)
		{
			bool color = getColor(state->tiles[move.from.x][move.from.y]); //white = 0, black = 1

			State temp;
			state->copyState(&temp);
			executeMove(&temp, move, color);

			if (isCheck(&temp, color))
			{
				std::wcout << "This would be a check!" << std::endl;
				return false;
			}
					
			return true;
		}

	std::wcout << "It is not a legal move." << std::endl;
	return false;
}