#ifndef BOARD_FEN_H
#define BOARD_FEN_H

#include "board.h"

#define FEN_MAX_PIECE_POSITIONS_LENGTH (64 + 7)
#define FEN_MAX_SIDE_TO_MOVE_LENGTH 1
#define FEN_MAX_CASTLING_RIGHTS_LENGTH 4
#define FEN_MAX_EN_PASSANT_TARGET_SQUARE_LENGTH 2
#define FEN_MAX_HALFMOVE_CLOCK_LENGTH 2	 // 50-move rule
#define FEN_MAX_FULLMOVE_NUMBER_LENGTH 4 // Game cannot last longer than ~6k moves
#define FEN_MAX_LENGTH (FEN_MAX_PIECE_POSITIONS_LENGTH + 1 + FEN_MAX_SIDE_TO_MOVE_LENGTH + 1 + FEN_MAX_CASTLING_RIGHTS_LENGTH + 1 + FEN_MAX_EN_PASSANT_TARGET_SQUARE_LENGTH + 1 + FEN_MAX_HALFMOVE_CLOCK_LENGTH + 1 + FEN_MAX_FULLMOVE_NUMBER_LENGTH)
#define FEN_BUF_LENGTH (FEN_MAX_LENGTH + 1)

/**
 * Gets the FEN string for the board.
 *
 * @param board The board to get the FEN string for.
 * @param fen The array to store the FEN string in.
 */
void board_get_fen(const board_t *board, char *fen);

/**
 * Sets the board to the position specified by the FEN string.
 *
 * @param board The board to set.
 * @param fen The FEN string.
 */
void board_set_fen(board_t *board, const char *fen);

#endif