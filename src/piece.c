#include "piece.h"
#include "side.h"

#define SYMBOL_UNKNOWN '?'

char piece_symbol(piece_t piece)
{
	switch (piece & (~SIDE_MASK))
	{
		case PIECE_NONE: return SYMBOL_NONE;
		case PAWN: return SYMBOL_PAWN;
		case KNIGHT: return SYMBOL_KNIGHT;
		case BISHOP: return SYMBOL_BISHOP;
		case ROOK: return SYMBOL_ROOK;
		case QUEEN: return SYMBOL_QUEEN;
		case KING: return SYMBOL_KING;
		default: return SYMBOL_UNKNOWN;
	}
}