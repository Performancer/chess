#include "transposition.h"
#include <random>
#include <map>

namespace transposition
{
	static UINT64 zobrist_table[64][12];
	static bool initialized;

	void initialize_zobrist()
	{
		std::random_device rd;
		std::mt19937_64 eng(rd());

		std::uniform_int_distribution<UINT64> distr;

		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 12; j++)
				zobrist_table[i][j] = distr(eng);

		initialized = true;
	}

	UINT64 hash(struct State* state)
	{
		if (!initialized)
			initialize_zobrist();

		UINT64 h = 0;

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

	static std::map<int, Transposition> table;

	bool exists(int key)
	{
		return table.find(key) != table.end();
	}

	Transposition get(int key)
	{
		return table[key];
	}

	void add(int key, Transposition value)
	{
		table[key] = value;
	}
}