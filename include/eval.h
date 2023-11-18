#ifndef EVAL_H
#define EVAL_H

#include "board.h"

#define CENTIPAWN_MULTIPLIER 100.0

#define VALUE_CHECKMATE 100000

typedef int score_t;

/**
 * Evaluates the board.
 *
 * @param board The board to evaluate.
 * @return The evaluation of the board in centipawns.
 */
score_t evaluate(const board_t *board);

/**
 * Evaluates the board relative to the side to move.
 *
 * @param board The board to evaluate.
 * @return The evaluation of the board in centipawns.
 */
score_t evaluate_relative(const board_t *board);

#endif