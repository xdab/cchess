#ifndef EVAL_H
#define EVAL_H

#include "board.h"

#define CENTIPAWN_MULTIPLIER 100.0

typedef int centipawns_t;

/**
 * Evaluates the board.
 *
 * @param board The board to evaluate.
 * @return The evaluation of the board in centipawns.
 */
centipawns_t eval(board_t *board);

#endif