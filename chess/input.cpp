#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include "input.h"

bool isValidOfficer(char c)
{
	if (c == 'K' || c == 'Q' || c == 'R' || c == 'B' || c == 'N' || c == 'P')
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

Move getMove(bool is_black)
{
	std::wcout << "Enter " << (is_black ? "black" : "white" ) << " move: ";
	std::wstring input;
	std::wcin >> input;

	if (input.size() == 6 && isValidOfficer(input[0]))
	{
		int from[2];
		if (parseCoordinates(from, input[1], input[2]))
		{
			int to[2];
			if (parseCoordinates(to, input[4], input[5]))
				return { from[0], from[1], to[0], to[1] };
		}
	}
	else if (input.size() == 5 && input == L"O-O-O")
	{
		int y = is_black ? 7 : 0;
		return { 4, y, 2, y };
	}
	else if (input.size() == 3 && input == L"O-O")
	{
		int y = is_black ? 7 : 0;
		return { 4, y, 6, y };
	}

	std::wcout << "Invalid move. Try again." << std::endl;
	return getMove(is_black);
}
