#pragma once
#include "state.h"
#include "vector.h"
#include "move.h"
#include <vector>

bool outsideBoard(struct Vector vector);

void getKnightMoves(std::vector<struct Vector> &moves, struct State* state, struct Vector from);
void getBishopMoves(std::vector<struct Vector> &moves, struct State* state, struct Vector from, int range = BOARD_SIZE);
void getRookMoves(std::vector<struct Vector> &moves, struct State* state, struct Vector from, int range = BOARD_SIZE);

void getMoves(std::vector<struct Vector> &moves, struct State* state, struct Vector from);
std::vector<struct Move> getMoves(struct State* state, bool color);