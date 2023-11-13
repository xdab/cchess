#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "side.h"
#include "castling.h"
#include "move.h"
#include "square.h"

#include <stdio.h>
#include <stdbool.h>

#define FILE_A 0
#define FILE_B 1
#define FILE_C 2
#define FILE_D 3
#define FILE_E 4
#define FILE_F 5
#define FILE_G 6
#define FILE_H 7

#define RANK_1 0
#define RANK_2 1
#define RANK_3 2
#define RANK_4 3
#define RANK_5 4
#define RANK_6 5
#define RANK_7 6
#define RANK_8 7

#define FILE_COUNT 8
#define RANK_COUNT 8

#define FILE(c) (c - 'a')
#define RANK(c) (c - '1')

#define FILE_SYMBOL(f) (f + 'a')
#define RANK_SYMBOL(r) (r + '1')

typedef struct board
{
    piece_t pieces[FILE_COUNT][RANK_COUNT];
    side_t side_to_move;
    castling_rights_t white_castling_rights;
    castling_rights_t black_castling_rights;
    square_t en_passant_square;
    int halfmove_clock;
    int fullmove_number;
} board_t;

/**
 * Initializes the board to the starting position.
 *
 * @param board The board to initialize.
 */
void board_init(board_t *board);

/**
 * Prints the board to the specified stream.
 *
 * @param board The board to print.
 * @param stream The stream to print to.
 */
void board_print(board_t *board, FILE *stream);

/**
 * Gets the piece at the specified square.
 *
 * @param board The board to get the piece from.
 * @param file The file of the square.
 * @param rank The rank of the square.
 * @return The piece at the specified square.
 */
piece_t board_get(const board_t *board, square_t square);

/**
 * Sets the piece at the specified square.
 *
 * @param board The board to set the piece on.
 * @param file The file of the square.
 * @param rank The rank of the square.
 * @param piece The piece to set.
 */
void board_set(board_t *board, square_t square, piece_t piece);

/**
 * Moves a piece on the board.
 *
 * @param board The board to move the piece on.
 * @param move The move to make.
 */
void board_make_move(board_t *board, move_t move);

/**
 * Undoes a move on the board.
 *
 * @param board The board to undo the move on.
 * @param move The move to undo.
 */
void board_undo_move(board_t *board, move_t move);

#endif