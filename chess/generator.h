#pragma once
#include "state.h"
#include "vector.h"
#include "move.h"
#include <vector>

std::vector<struct Vector> getMoves(struct State* state, struct Vector from);
std::vector<struct Move> getMoves(struct State* state, bool color);
bool isThreatened(struct State* state, struct Vector square, bool color);
bool isCheck(struct State* state, bool color);