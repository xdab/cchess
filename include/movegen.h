#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "move.h"

#define MAX_MOVES 256 // True number is ~218

void movegen_generate(board_t *board, move_t *out_moves, int *out_move_count);

#endif