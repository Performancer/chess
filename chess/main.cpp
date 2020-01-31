#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include "input.h"

#define WHITE 0x0
#define BLACK 0x1

#define KING	0x0
#define QUEEN	0x1
#define ROOK	0x2
#define BISHOP	0x3
#define KNIGHT	0x4
#define PAWN	0x5
#define EMPTY	0x6

struct tile
{
	char type = EMPTY;
	bool color;
};

tile board_state[8][8];
bool white_can_castle_king_side = true;
bool white_can_castle_queen_side = true;
bool black_can_castle_king_side = true;
bool black_can_castle_queen_side = true;

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
		std::wcout << " " << (char)('A' + i) << " ";

	std::wcout << std::endl << std::endl;
	SetConsoleTextAttribute(out, BLACK_CONSOLE);
}

void movePiece(int from_x, int from_y, int to_x, int to_y)
{
	char type = board_state[from_x][from_y].type;
	bool color = board_state[from_x][from_y].color;

	board_state[from_x][from_y].type = EMPTY;

	board_state[to_x][to_y].type = type;
	board_state[to_x][to_y].color = color;
}

void executeMove(move* move, bool color)
{
	if (move->type == OFFICER_MOVE)
	{
		movePiece(move->from_x, move->from_y, move->to_x, move->to_y);
	}
	else if (move->type == CASTLING_QUEEN_SIDE)
	{
		int y = color == WHITE ? 0 : 7;
		movePiece(4, y, 2, y);
		movePiece(0, y, 3, y);
	}
	else if (move->type == CASTLING_KING_SIDE)
	{
		int y = color == WHITE ? 0 : 7;
		movePiece(4, y, 6, y);
		movePiece(7, y, 5, y);
	}
	else if (move->type == PAWN_MOVE)
	{
		//TODO: find out which pawn is moving
	}

	delete move;
}

int main()
{
	initializeBoard();

	while(true)
	{
		draw();
		executeMove(getMove(), WHITE);
		system("cls");
	}

	std::cin.get();
}