#include "transposition.h"
#include <random>
#include <map>

namespace transposition
{
	static UINT64 zobrist_table[64][12];
	static UINT64 special_cases[5];

	static bool initialized;

	void initialize_zobrist()
	{
		std::random_device rd;
		std::mt19937_64 eng(rd());

		std::uniform_int_distribution<UINT64> distr;

		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 12; j++)
				zobrist_table[i][j] = distr(eng);

		for (int i = 0; i < 5; i++)
			special_cases[i] = distr(eng);

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

				if(state->en_passant.isEqual(x, y))
					h ^= special_cases[4];
				else if (tile != EMPTY)
					h ^= zobrist_table[x * 8 + y][getType(tile) + (getColor(tile) ? 6 : 0)];
			}

		if (state->black_can_castle_king_side)
			h ^= special_cases[0];

		if (state->black_can_castle_queen_side)
			h ^= special_cases[1];

		if (state->white_can_castle_king_side)
			h ^= special_cases[2];

		if (state->white_can_castle_queen_side)
			h ^= special_cases[3];

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