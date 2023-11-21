#ifndef BOARD_PIECES_H
#define BOARD_PIECES_H

#include "board.h"

/**
 * Initializes the board pieces for the given side.
 *
 * @param board_pieces The board pieces to initialize.
 * @param side The side to initialize the board pieces for.
 */
void board_pieces_init(board_pieces_t *board_pieces, side_t side);

/**
 * Prints the board pieces to the stream.
 *
 * @param board_pieces The board pieces to print.
 * @param stream The stream to print to.
 */
void board_pieces_print(const board_pieces_t *board_pieces, FILE *stream);

/**
 * Loop through all the squares on the board and updates piece positions.
 *
 * @param board The board to scan.
 */
void board_pieces_full_update(board_t *board);

/**
 * Updates the piece positions after a move.
 *
 * @param board The board to update the piece positions on, right before the move is made.
 * @param move The move which will be made.
 */
void board_pieces_move_update(board_t *board, move_t move);

/**
 * Updates the piece positions after a move is unmade.
 *
 * @param board The board to update the piece positions on, right before the move is unmade.
 * @param move The move which will be unmade.
 */
void board_pieces_unmove_update(board_t *board, move_t move);

#endif
