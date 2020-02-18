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
	state->tiles[from.x][from.y] = EMPTY;
	state->tiles[to.x][to.y] = piece;
}

void checkEnPasse(struct State* state, Move move)
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
			std::wcout << "It seems we are executing En Passant" << std::endl;
			char passer = state->tiles[move.to.x][move.from.y];

			if (getType(passer) == PAWN && !isAlly(piece, passer))
			{
				std::wcout << "En Passant executed" << std::endl;
				state->tiles[move.to.x][move.from.y] = EMPTY;
			}
		}
	}
}

void checkCastling(struct State* state, Move move)
{
	char piece = state->tiles[move.from.x][move.from.y];
	char type = getType(piece);
	bool color = getColor(piece); // white = 0, black = 1

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

		// are we castling?
		if (move.from.x == 4 && move.from.y == y && move.to.y == y)
		{
			if (move.to.x == 2) // queen side
				movePiece(state, { 0, y }, { 3, y });
			else if (move.to.x == 6) // king side
				movePiece(state, { 7, y }, { 5, y });
		}
	}
	else if (type == ROOK)
	{
		if (move.from.y == (color ? 7 : 0))
		{
			if (move.from.x == 0) // queen side
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

void executeMove(struct State* state, Move move)
{
	std::wcout << "moving (" << move.from.x << ", " << move.from.y  <<  ") to (" << move.to.x << ", " << move.to.y << ")" << std::endl;

	checkEnPasse(state, move);
	checkCastling(state, move);

	movePiece(state, move.from, move.to);
}

bool isLegalMove(struct State* state, Move move)
{
	for (Vector* vector : getMoves(state, move.from, EMPTY, 8))
		if (*vector == move.to)
		{
			bool color = getColor(state->tiles[move.from.x][move.from.y]); //white = 0, black = 1

			State temp;
			state->copyState(&temp);
			executeMove(&temp, move);

			if (isCheck(&temp, color))
			{
				std::wcout << "This would result in a check!" << std::endl;
				return false;
			}

			if (move.isCastling(state))
			{
				//1. The king and the chosen rook are on the player's first rank.
				//2. Neither the king nor the chosen rook has previously moved.
				//3. There are no pieces between the king and the chosen rook.
				//4. The king is not currently in check.
				//5. The king does not pass through a square that is attacked by an enemy piece.
				//6. The king does not end up in check. (True of any legal move.)

				if (isCheck(state, color))
				{
					std::wcout << "Cannot castle: the king is currently in check!" << std::endl;
					return false;
				}
				else if (move.to.x == 2 )
				{
					if (state->tiles[1][move.from.y] != EMPTY)
					{
						std::wcout << "Cannot castle: there is another piece in the way!" << std::endl;
						return false;
					}
					else if (state->tiles[2][move.from.y] != EMPTY)
					{
						std::wcout << "Cannot castle: there is another piece in the way!" << std::endl;
						return false;
					}
					else if (state->tiles[3][move.from.y] != EMPTY)
					{
						std::wcout << "Cannot castle: there is another piece in the way!" << std::endl;
						return false;
					}
					else if (isThreatened(state, { 3, move.from.y }, color))
					{
						std::wcout << "Cannot castle: the square between is threatened!" << std::endl;
						return false;
					}
				}
				else if (move.to.x == 6 )
				{
					if (state->tiles[5][move.from.y] != EMPTY)
					{
						std::wcout << "Cannot castle: there is another piece in the way!" << std::endl;
						return false;
					}
					else if (state->tiles[6][move.from.y] != EMPTY)
					{
						std::wcout << "Cannot castle: there is another piece in the way!" << std::endl;
						return false;
					}
					else if (isThreatened(state, { 5, move.from.y }, color))
					{
						std::wcout << "Cannot castle: the square between is threatened!" << std::endl;
						return false;
					}
				}
			}

			return true;
		}

	std::wcout << "It is not a legal move." << std::endl;
	return false;
}