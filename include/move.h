#ifndef MOVE_H
#define MOVE_H

#include "square.h"
#include "piece.h"

#include <stdint.h>

typedef uint16_t move_t;

#define FROM_SQUARE_OFFSET 0
#define TO_SQUARE_OFFSET 6
#define PROMOTION_OFFSET 12

#define SQUARE_MASK 0x3f
#define PROMOTION_MASK 0x05

#define MOVE_NULL 0xffff

static inline move_t move_regular(square_t from, square_t to)
{
	if (!SQUARE_VALID(from) || !SQUARE_VALID(to) || from == to)
		return MOVE_NULL;

	return (from << FROM_SQUARE_OFFSET) | (to << TO_SQUARE_OFFSET);
}

static inline move_t move_promotion(square_t from, square_t to, piece_t promote_to)
{
	if (!SQUARE_VALID(from) || !SQUARE_VALID(to) || from == to)
		return MOVE_NULL;

	int encoded_promote_to;
	switch (promote_to & PIECE_MASK)
	{
	case QUEEN:
		encoded_promote_to = 1;
		break;
	case ROOK:
		encoded_promote_to = 2;
		break;
	case BISHOP:
		encoded_promote_to = 3;
		break;
	case KNIGHT:
		encoded_promote_to = 4;
		break;
	default:
		return MOVE_NULL;
	}

	return (from << FROM_SQUARE_OFFSET) | (to << TO_SQUARE_OFFSET) | (encoded_promote_to << PROMOTION_OFFSET);
}

static inline square_t move_get_from(move_t move)
{
	return (move >> FROM_SQUARE_OFFSET) & SQUARE_MASK;
}

static inline square_t move_get_to(move_t move)
{
	return (move >> TO_SQUARE_OFFSET) & SQUARE_MASK;
}

static inline piece_t move_get_promoted_piece(move_t move)
{
	int encoded_promote_to = (move >> PROMOTION_OFFSET) & PROMOTION_MASK;
	switch (encoded_promote_to)
	{
	case 1:
		return QUEEN;
	case 2:
		return ROOK;
	case 3:
		return BISHOP;
	case 4:
		return KNIGHT;
	default:
		return PIECE_NONE;
	}
}

move_t move_uci(const char *uci);
void move_to_uci(move_t move, char *uci);

#define MOVE_WHITE_OO (move_regular(E1, G1))
#define MOVE_WHITE_OOO (move_regular(E1, C1))
#define MOVE_BLACK_OO (move_regular(E8, G8))
#define MOVE_BLACK_OOO (move_regular(E8, C8))

#endif