#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "state.h"

#define BLACK_CONSOLE		0x0F
#define DARK_GREY_CONSOLE	0x80
#define LIGHT_GREY_CONSOLE	0x70
#define WHITE_CONSOLE		0xF0

void draw(struct State* state)
{
	system("cls");

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	_setmode(_fileno(stdout), _O_U16TEXT);

	for (int y = 7; y >= 0; y--)
	{
		SetConsoleTextAttribute(out, DARK_GREY_CONSOLE);
		std::wcout << y + 1;

		for (int x = 0; x < 8; x++)
		{
			if (y % 2 != 0 && x % 2 == 0 || y % 2 == 0 && x % 2 != 0)
				SetConsoleTextAttribute(out, LIGHT_GREY_CONSOLE);
			else
				SetConsoleTextAttribute(out, WHITE_CONSOLE);

			char tile = state->tiles[x][y];

			if (tile != EMPTY)
			{
				char piece = (tile | BLACK) ^ BLACK;
				bool black = (tile & BLACK) == BLACK;

				std::wcout << " " << static_cast<wchar_t>(9812 + piece + black * 6) << " ";
			}
			else
				std::wcout << "   ";
		}

		std::wcout << std::endl;
	}

	SetConsoleTextAttribute(out, DARK_GREY_CONSOLE);
	std::wcout << " ";

	for (int i = 0; i < 8; i++)
		std::wcout << " " << (char)('A' + i) << " ";

	std::wcout << std::endl << std::endl;
	SetConsoleTextAttribute(out, BLACK_CONSOLE);
}