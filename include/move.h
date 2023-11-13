#ifndef MOVE_H
#define MOVE_H

#include "square.h"

typedef enum promotion {
	PROMOTION_NONE = 0,
	PROMOTION_QUEEN = 1,
	PROMOTION_ROOK = 2,
	PROMOTION_BISHOP = 4,
	PROMOTION_KNIGHT = 8,
} promotion_t;

typedef unsigned short move_t;

move_t move_regular(square_t from, square_t to);
move_t move_promotion(square_t from, square_t to, promotion_t promotion);

move_t move_uci(const char *uci);
void move_to_uci(move_t move, char *uci);

int move_get_from_file(move_t move);
int move_get_from_rank(move_t move);
int move_get_to_file(move_t move);
int move_get_to_rank(move_t move);
int move_get_promotion(move_t move);

#endif