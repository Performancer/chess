#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>

#define WHITE 0
#define BLACK 1

#define KING	0
#define QUEEN	1
#define ROOK	2
#define BISHOP	3
#define KNIGHT	4
#define PAWN	5
#define EMPTY	6

struct tile
{
	char type = EMPTY;
	bool color;
};

tile board_state[8][8];

void initializeBoard()
{
	board_state[0][0] = { ROOK,		WHITE };
	board_state[1][0] = { KNIGHT,	WHITE };
	board_state[2][0] = { BISHOP,	WHITE };
	board_state[3][0] = { QUEEN,	WHITE };
	board_state[4][0] = { KING,		WHITE };
	board_state[5][0] = { BISHOP,	WHITE };
	board_state[6][0] = { KNIGHT,	WHITE };
	board_state[7][0] = { ROOK,		WHITE };

	for (int x = 0; x < 8; x++)
		board_state[x][1] = { PAWN, WHITE };

	board_state[0][7] = { ROOK,		BLACK };
	board_state[1][7] = { KNIGHT,	BLACK };
	board_state[2][7] = { BISHOP,	BLACK };
	board_state[3][7] = { QUEEN,	BLACK };
	board_state[4][7] = { KING,		BLACK };
	board_state[5][7] = { BISHOP,	BLACK };
	board_state[6][7] = { KNIGHT,	BLACK };
	board_state[7][7] = { ROOK,		BLACK };

	for (int x = 0; x < 8; x++)
		board_state[x][6] = { PAWN, BLACK };
}

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
#define BLACK_CONSOLE		0x0F
#define DARK_GREY_CONSOLE	0x80
#define LIGHT_GREY_CONSOLE	0x70
#define WHITE_CONSOLE		0xF0

void draw()
{
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

			tile* tile = &board_state[x][y];

			if (tile->type != EMPTY)
				std::wcout << " " << static_cast<wchar_t>(9812 + tile->type + tile->color * 6) << " ";
			else
				std::wcout << "   ";
		}

		std::wcout << std::endl;
	}

	SetConsoleTextAttribute(out, DARK_GREY_CONSOLE);
	std::wcout << " ";

	for (int i = 0; i < 8; i++)
		std::wcout << " " << (char)(65 + i) << " ";

	std::wcout << std::endl << std::endl;
	SetConsoleTextAttribute(out, BLACK_CONSOLE);
}

void getMove()
{
	std::wcout << "Enter your move: ";

	std::wstring input;
	std::wcin >> input;
}

int main()
{
	initializeBoard();

	draw();

	std::cin.get();
}