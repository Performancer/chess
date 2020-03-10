#include "state.h"
#include "input.h"
#include "output.h"
#include "generator.h"
#include "move.h"

#include <iostream>
#include <math.h>

void movePiece(struct State* state, struct Vector from, struct Vector to)
{
	char piece = state->tiles[from.x][from.y];
	char destination = state->tiles[to.x][to.y];

	if (destination != EMPTY)
	{
		getColor(destination) ? state->blacks-- : state->whites--;

		if (getType(destination) == KING)
		{
			if (getColor(destination))
				state->black_king = { -1, -1 };
			else
				state->white_king = { -1, -1 };
		}
	}

	if (getType(piece) == KING)
	{
		if (getColor(piece))
			state->black_king = { to.x, to.y };
		else
			state->white_king = { to.x, to.y };
	}

	state->tiles[from.x][from.y] = EMPTY;
	state->tiles[to.x][to.y] = piece;
}

void checkEnPasse(struct State* state, Move move, bool inform)
{
	state->en_passant = { -1, -1 };

	char piece = state->tiles[move.from.x][move.from.y];
	if (getType(piece) == PAWN)
	{
		if (move.from.x == move.to.x)
		{
			if (abs(move.from.y - move.to.y) == 2)
				state->en_passant = move.to; //record a pawn's move of two squares
		}
		else if (state->tiles[move.to.x][move.to.y] == EMPTY)
		{
			if(inform)
				std::wcout << "It seems we are executing En Passant" << std::endl;

			char passer = state->tiles[move.to.x][move.from.y];

			if (getType(passer) == PAWN && !isAlly(piece, passer))
			{
				if (inform)
					std::wcout << "En Passant executed" << std::endl;

				if (getColor(passer))
					state->blacks--;
				else
					state->whites--;

				state->tiles[move.to.x][move.from.y] = EMPTY;
			}
		}
	}
}

void checkCastling(struct State* state, Move move)
{
	char piece = state->tiles[move.from.x][move.from.y];
	char type = getType(piece);
	bool color = getColor(piece); //white = 0, black = 1

	if (type == KING)
	{
		if (color)
		{
			state->black_can_castle_king_side = false;
			state->black_can_castle_queen_side = false;
		}
		else
		{
			state->white_can_castle_king_side = false;
			state->white_can_castle_queen_side = false;
		}

		int y = color ? 7 : 0;

		//are we castling?
		if (move.from.x == 4 && move.from.y == y && move.to.y == y)
		{
			if (move.to.x == 2) //queen side
				movePiece(state, { 0, y }, { 3, y });
			else if (move.to.x == 6) //king side
				movePiece(state, { 7, y }, { 5, y });
		}
	}
	else if (type == ROOK)
	{
		if (move.from.y == (color ? 7 : 0))
		{
			if (move.from.x == 0) //queen side
			{
				if (color)
					state->black_can_castle_queen_side = false;
				else
					state->white_can_castle_queen_side = false;
			}
			else if (move.from.x == 7) //king side
			{
				if (color)
					state->black_can_castle_king_side = false;
				else
					state->white_can_castle_king_side = false;
			}
		}
	}
}

void executeMove(struct State* state, Move move, bool inform = false)
{
	if(inform)
		std::wcout << "Moving (" << move.from.x << ", " << move.from.y  <<  ") to (" << move.to.x << ", " << move.to.y << ")" << std::endl;

	checkEnPasse(state, move, inform);
	checkCastling(state, move);

	movePiece(state, move.from, move.to);

	char piece = state->tiles[move.to.x][move.to.y];
	char type = getType(piece);
	bool color = getColor(piece); //white = 0, black = 1

	if (type == PAWN && move.to.y == (color ? 0 : 7))
	{
		//TODO: choosing promotion

		if(color)
			state->tiles[move.to.x][move.to.y] = QUEEN | BLACK;
		else
			state->tiles[move.to.x][move.to.y] = QUEEN;
	}
}

bool isLegalMove(struct State* state, Move move)
{
	for (struct Vector vector : getMoves(state, move.from))
		if (vector == move.to)
			return true;

	std::wcout << "It is not a legal move." << std::endl;
	std::wcout << "Legal moves from (" << move.from.x << "," << move.from.y << ") are:" << std::endl;
	
	for (struct Vector vector : getMoves(state, move.from))
		std::wcout << "   to: " << vector.x << ","<< vector.y << std::endl;

	return false;
}