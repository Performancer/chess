#include <stdlib.h>
#include <map>
#include "transposition.h"

int zobrist_table[64][12];

void initialize_zobrist()
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 12; j++)
			zobrist_table[i][j] = rand();
}

int hash(struct State* state)
{
	int h = 0;

	for (int x = 0; x < BOARD_SIZE; x++)
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			char tile = state->tiles[x][y];

			if (tile != EMPTY)
			{
				char value = getType(tile) + (getColor(tile) ? 6 : 0);
				h ^= zobrist_table[x * 8 + y][value];
			}
		}

	return h;
}

std::map<int, Transposition> table;

bool transpositionExists(int key)
{
	return table.find(key) != table.end();
}

Transposition getTransposition(int key)
{
	return table[key];
}

void addTransposition(int key, Transposition value)
{
	table[key] = value;
}
