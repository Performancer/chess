#pragma once
#include "state.h"

struct Transposition
{
	int depth;
	int upper;
	int lower;
};

void initialize_zobrist();
int hash(struct State* state);

bool transpositionExists(int key);
Transposition getTransposition(int key);
void addTransposition(int key, Transposition value);