#ifndef PIECEPOS_H
#define PIECEPOS_H

#include "square.h"
#include "piece.h"
#include "side.h"

#include <stdio.h>
#include <stdbool.h>

typedef struct piecepos
{
	square_t king;
	square_t queen;
	square_t kings_rook;
	square_t queens_rook;
	square_t kings_knight;
	square_t queens_knight;
	square_t kings_bishop;	// Light square bishop
	square_t queens_bishop; // Dark square bishop
	square_t pawns[8];
	bool has_pawns;

	bool has_promoted_pieces;
	square_t promoted_queens[8];
	square_t promoted_rooks[8];
	square_t promoted_bishops[8];
	square_t promoted_knights[8];
	bool has_promoted_queens;
	bool has_promoted_rooks;
	bool has_promoted_bishops;
	bool has_promoted_knights;
} piecepos_t;

/**
 * Initializes the piece positions for white.
 *
 * @param piece_positions The piece positions to initialize.
 */
void piecepos_init_white(piecepos_t *piece_positions);

/**
 * Initializes the piece positions for black.
 *
 * @param piece_positions The piece positions to initialize.
 */
void piecepos_init_black(piecepos_t *piece_positions);

/**
 * Prints the piece positions to the stream.
 *
 * @param piece_positions The piece positions to print.
 * @param stream The stream to print to.
 */
void piecepos_print(const piecepos_t *piece_positions, FILE *stream);

/**
 * Updates the piece positions after a move.
 *
 * @param white The white piece positions.
 * @param black The black piece positions.
 * @param from The square the piece moved from.
 * @param target The square the piece moved to.
 * @param moved_piece The piece that moved.
 * @param target_piece The piece that was captured.
 */
void piecepos_update(piecepos_t *white, piecepos_t *black, square_t from, square_t target, piece_t moved_piece, piece_t promotion_piece, piece_t target_piece);

/**
 * Sets likely piece positions based on squares with pieces.
 *
 * @param piece_positions The piece positions to set.
 * @param squares The squares with pieces on the board.
 * @param side The side to set the piece positions for.
 */
void piecepos_from_squares(piecepos_t *piece_positions, const piece_t squares[SQUARE_COUNT], side_t side);

#endif