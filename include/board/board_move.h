#ifndef BOARD_MOVE_H
#define BOARD_MOVE_H

#include "board.h"

/**
 * Executes the given move on the board.
 *
 * @param board The board to move the piece on.
 * @param move The move to make.
 */
void board_make_move(board_t *board, move_t move);

/**
 * Undoes the last move made on the board.
 *
 * @param board The board to undo the last move on.
 */
void board_unmake_move(board_t *board);

#endif