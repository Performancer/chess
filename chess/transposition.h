#pragma once
#include "state.h"

struct Transposition
{
	int depth;
	int upper;
	int lower;
};

namespace transposition
{
	void initialize_zobrist();
	int hash(struct State* state);

	bool exists(int key);
	Transposition get(int key);
	void add(int key, Transposition value);
}