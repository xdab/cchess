#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"

typedef unsigned int move_t;

#define MOVE_NULL (move_regular(SQUARE_NONE, SQUARE_NONE))

move_t move_regular(square_t from, square_t to);
move_t move_promotion(square_t from, square_t to, piece_t promote_to);

move_t move_uci(const char *uci);
void move_to_uci(move_t move, char *uci);

square_t move_get_from(move_t move);
square_t move_get_to(move_t move);
piece_t move_get_promoted_piece(move_t move);

#endif