#ifndef MOVEGEN_KING_H
#define MOVEGEN_KING_H

#include "board.h"
#include "move.h"
#include "square.h"

/**
 * Generates all king moves for the given square.
 *
 * @param board The board.
 * @param out_moves The array to write the moves to.
 * @param out_move_count The number of moves written to the array.
 * @param square The square to generate moves for.
 */
void movegen_generate_king_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square);

#endif