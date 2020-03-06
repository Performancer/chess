#pragma once
#include "state.h"

void initialize_zobrist();
int hash(struct State* state);