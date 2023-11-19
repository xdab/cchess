#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "move.h"

#define MAX_MOVES 256 // True number is ~218

/**
 * Generates all pseudo-legal moves for the specified board.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 */
void movegen_generate(const board_t *board, move_t *out_moves, int *out_move_count);

#endif