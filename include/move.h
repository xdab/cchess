#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"

typedef enum promotion {
	PROMOTION_NONE = 0,
	PROMOTION_QUEEN = 1,
	PROMOTION_ROOK = 2,
	PROMOTION_BISHOP = 4,
	PROMOTION_KNIGHT = 8,
} promotion_t;

typedef unsigned int move_t;

#define MOVE_NULL (move_regular(SQUARE_NULL, SQUARE_NULL))

move_t move_regular(square_t from, square_t to);
move_t move_promotion(square_t from, square_t to, promotion_t promotion);

move_t move_uci(const char *uci);
void move_to_uci(move_t move, char *uci);

square_t move_get_from(move_t move);
square_t move_get_to(move_t move);
piece_t move_get_promoted_piece(move_t move);

#endif