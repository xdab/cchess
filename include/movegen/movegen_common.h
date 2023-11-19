#ifndef MOVEGEN_COMMON_H
#define MOVEGEN_COMMON_H

#include "board.h"
#include "move.h"
#include "square.h"

#define MG_NOT_ADDED 0
#define MG_EMPTY 1
#define MG_ENEMY 2

/**
 * Adds a move to the array of moves.
 *
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 * @param move The move to add.
 */
void movegen_add(move_t *out_moves, int *out_move_count, move_t move);

/**
 * Adds a move to the array of moves given the specified conditions.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 * @param move The move to add.
 * @param condition The condition to add the move under (MG_EMPTY |? MG_ENEMY)
 *
 * @return MG_NOT_ADDED if the move was not added,
 *         MG_EMPTY if the move was added because the target square was empty,
 *         MG_ENEMY if the move was added because the target square contained an enemy piece.
 */
int movegen_add_cond(const board_t *board, move_t *out_moves, int *out_move_count, move_t move, int condition);

#define MIN_MOVE_DISTANCE 1
#define MAX_MOVE_DISTANCE 7

#endif