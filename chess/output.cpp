#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "state.h"
#include "move.h"

#define WHITE_TEXT			0x0F
#define FRAME_COLOR			0x80
#define MOVE_COLOR			0x60
#define BLACK_SQUARE		0x80
#define WHITE_SQUARE		0x70

void draw(struct State* state, struct Move last_move)
{
	//system("cls");

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	_setmode(_fileno(stdout), _O_U16TEXT);

	for (int y = BOARD_SIZE - 1; y >= 0; y--)
	{
		SetConsoleTextAttribute(out, FRAME_COLOR);
		std::wcout << y + 1 << " ";

		for (int x = 0; x < BOARD_SIZE; x++)
		{
			char tile = state->tiles[x][y];
			char type = getType(tile);
			bool black = getColor(tile);

			int color;

			if (x == last_move.from.x && y == last_move.from.y || x == last_move.to.x && y == last_move.to.y)
				color = MOVE_COLOR;
			else if (y % 2 != 0 && x % 2 == 0 || y % 2 == 0 && x % 2 != 0)
				color = WHITE_SQUARE;
			else
				color = BLACK_SQUARE;

			if (!black)
				color |= WHITE_TEXT;

			SetConsoleTextAttribute(out, color);

			if (type != EMPTY)
				std::wcout << " " << static_cast<wchar_t>(9818 + type) << " ";
			else
				std::wcout << "   ";
		}

		std::wcout << std::endl;
	}

	SetConsoleTextAttribute(out, FRAME_COLOR);
	std::wcout << "  ";

	for (int i = 0; i < BOARD_SIZE; i++)
		std::wcout << " " << (char)('A' + i) << " ";

	std::wcout << std::endl << std::endl;
	SetConsoleTextAttribute(out, WHITE_TEXT);
}
