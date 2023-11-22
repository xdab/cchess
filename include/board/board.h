#ifndef BOARD_H
#define BOARD_H

#include "rank.h"
#include "file.h"
#include "piece.h"
#include "side.h"
#include "castling.h"
#include "move.h"
#include "square.h"
#include "zobrist.h"

#include <stdio.h>
#include <stdbool.h>

#define BOARD_HISTORY_SIZE 128
#define BOARD_PIECE_COUNT L_PIECE_COUNT

#define MAX_PAWNS 8
#define MAX_PROMOTED_PIECES 8

typedef struct board_event
{
    move_t move;
    piece_t captured_piece;
    castling_rights_t white_castling_rights;
    castling_rights_t black_castling_rights;
    square_t en_passant_square;
    int halfmove_clock;

} board_event_t;

typedef struct board_pieces
{
    square_t king;
    square_t queen;
    square_t kings_rook;
    square_t queens_rook;
    square_t kings_knight;
    square_t queens_knight;
    square_t kings_bishop;  // Light square bishop
    square_t queens_bishop; // Dark square bishop

    bool has_pawns;
    square_t pawns[MAX_PAWNS];

    bool has_promoted_pieces;
    bool has_promoted_queens;
    square_t promoted_queens[MAX_PROMOTED_PIECES];
    bool has_promoted_rooks;
    square_t promoted_rooks[MAX_PROMOTED_PIECES];
    bool has_promoted_knights;
    square_t promoted_knights[MAX_PROMOTED_PIECES];
    bool has_promoted_bishops;
    square_t promoted_bishops[MAX_PROMOTED_PIECES];
} board_pieces_t;

typedef struct board
{
    // Squares with pieces representation
    piece_t squares[SQUARE_COUNT];

    // Pieces with squares representation
    board_pieces_t white_pieces;
    board_pieces_t black_pieces;

    side_t side_to_move;
    castling_rights_t white_castling_rights;
    castling_rights_t black_castling_rights;
    square_t en_passant_square;
    int halfmove_clock;
    int fullmove_number;
    zobrist_t hash;

    // Board history
    board_event_t history[BOARD_HISTORY_SIZE];
    int history_size;
} board_t;

/**
 * Initializes the board to the starting position.
 *
 * @note This function must be called before any other board functions.
 *
 * @param board The board to initialize.
 */
void board_init(board_t *board);

/**
 * Clones the board.
 *
 * @param board The board to clone.
 * @param clone The board to clone to.
 */
void board_clone(const board_t *board, board_t *clone);

/**
 * Prints the board to the specified stream.
 *
 * @param board The board to print.
 * @param stream The stream to print to.
 */
void board_print(const board_t *board, FILE *stream);

/**
 * Prints the board history to the specified stream.
 *
 * @param board The board to print the history from.
 * @param stream The stream to print to.
 */
void board_print_history(const board_t *board, FILE *stream);

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

#endif