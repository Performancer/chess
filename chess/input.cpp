#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include "input.h"

bool isValidOfficer(char c)
{
	if (c == 'K' || c == 'Q' || c == 'R' || c == 'B' || c == 'N')
		return true;

	return false;
}

bool parseCoordinates(int* arr, char x, char y)
{
	if (!isalpha(x) || x < 'a' || x > 'h')
		return false;

	if (!isdigit(y) || y < '1' || y > '8')
		return false;

	arr[0] = x - 'a';
	arr[1] = y - '1';

	return true;
}

move* getMove()
{
	std::wcout << "Enter your move: ";
	std::wstring input;
	std::wcin >> input;

	move* m = new move;

	if (input.size() == 6 && isValidOfficer(input[0]))
	{
		int from[2];
		if (parseCoordinates(from, input[1], input[2]))
		{
			int to[2];
			if (parseCoordinates(to, input[4], input[5]))
				*m = { OFFICER_MOVE, from[0], from[1], to[0], to[1] };
		}
	}
	else if (input.size() == 5 && input == L"O-O-O")
		*m = {CASTLING_QUEEN_SIDE};
	else if (input.size() == 3 && input == L"O-O")
		*m = {CASTLING_KING_SIDE};
	else if (input.size() == 2)
	{
		int to[2];
		if (parseCoordinates(to, input[0], input[1]))
			*m = { PAWN_MOVE, 0, 0, to[0], to[1] };
	}

	if (m->type == INVALID_MOVE)
	{
		std::wcout << "Invalid move. Try again." << std::endl;
		return getMove();
	}

	return m;
}
