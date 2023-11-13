#include "piece.h"
#include "side.h"

#define SYMBOL_UNKNOWN '?'

char piece_symbol(piece_t piece)
{
	switch (piece & (~SIDE_MASK))
	{
		case PIECE_NONE: return SYMBOL_NONE;
		case PIECE_PAWN: return SYMBOL_PAWN;
		case PIECE_KNIGHT: return SYMBOL_KNIGHT;
		case PIECE_BISHOP: return SYMBOL_BISHOP;
		case PIECE_ROOK: return SYMBOL_ROOK;
		case PIECE_QUEEN: return SYMBOL_QUEEN;
		case PIECE_KING: return SYMBOL_KING;
		default: return SYMBOL_UNKNOWN;
	}
}