#pragma once
#include <list>
#include "state.h"
#include "vector.h"

std::list<struct Vector*> getMoves(struct State* state, struct Vector from, char type, char range);
bool isCheck(struct State* state, bool color);