#pragma once
#include "state.h"
#include "vector.h"
#include "move.h"
#include <vector>

bool outsideBoard(struct Vector vector);

std::vector<struct Vector> getKnightMoves(struct State* state, struct Vector from);
std::vector<struct Vector> getBishopMoves(struct State* state, struct Vector from, int range = BOARD_SIZE);
std::vector<struct Vector> getRookMoves(struct State* state, struct Vector from, int range = BOARD_SIZE);

std::vector<struct Vector> getMoves(struct State* state, struct Vector from);
std::vector<struct Move> getMoves(struct State* state, bool color);