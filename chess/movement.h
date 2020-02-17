#pragma once
#include "move.h"

void movePiece(struct State* state, struct Vector from, struct Vector to);
void executeMove(struct State* state, Move move, bool is_white);
bool isLegalMove(struct State* state, Move move);