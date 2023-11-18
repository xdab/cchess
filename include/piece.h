#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

typedef uint8_t piece_t;

#define PIECE_NONE ((piece_t)0b00000000)
#define PAWN ((piece_t)0b00000001)
#define KNIGHT ((piece_t)0b00000010)
#define BISHOP ((piece_t)0b00000100)
#define ROOK ((piece_t)0b00001000)
#define QUEEN ((piece_t)0b00010000)
#define KING ((piece_t)0b00100000)

#define PIECE_MASK 0b00111111

#define SYMBOL_NONE ' '
#define SYMBOL_PAWN 'P'
#define SYMBOL_KNIGHT 'N'
#define SYMBOL_BISHOP 'B'
#define SYMBOL_ROOK 'R'
#define SYMBOL_QUEEN 'Q'
#define SYMBOL_KING 'K'

char piece_symbol(piece_t piece);

#endif