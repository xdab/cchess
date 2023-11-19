#ifndef MOVEGEN_COMMON_H
#define MOVEGEN_COMMON_H

#include "board.h"
#include "move.h"
#include "square.h"

#define NOT_ADDED 0
#define ADDED_EMPTY 1
#define ADDED_ENEMY 2

/**
 * Adds a move to the array of moves if the target square is empty or contains an enemy piece.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 * @param from The square the piece is moving from.
 * @param to The square the piece is moving to.
 *
 * @return NOT_ADDED if the move was not added,
 *         ADDED_EMPTY if the move was added because the target square was empty,
 *         ADDED_ENEMY if the move was added because the target square contained an enemy piece.
 */
int movegen_add_if_target_empty_or_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to);

/**
 * Adds a move to the array of moves if the target square contains an enemy piece.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 * @param from The square the piece is moving from.
 * @param to The square the piece is moving to.
 */
void movegen_add_if_target_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to);

#endif