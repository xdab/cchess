#ifndef MOVEGEN_PAWN_H
#define MOVEGEN_PAWN_H

#include "board.h"
#include "move.h"
#include "square.h"

/**
 * Generates all pawn moves for the given square.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The number of moves generated.
 * @param square The square to generate moves for.
 */
void movegen_generate_pawn_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square);

#endif
