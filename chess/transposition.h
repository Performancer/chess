#pragma once
#include "state.h"

struct Transposition
{
	bool color;
	int depth;
	EvaluatedMove move;
};

namespace transposition
{
	typedef unsigned long long int UINT64;

	void initialize_zobrist();
	UINT64 hash(struct State* state);

	bool exists(int key);
	Transposition get(int key);
	void add(int key, Transposition value);
}