#pragma once
#include <list>
#include "state.h"
#include "vector.h"

std::list<struct Vector> getMoves(struct State* state, struct Vector from);
bool isThreatened(struct State* state, struct Vector square, bool color);
bool isCheck(struct State* state, bool color);