#ifndef PIECE_H
#define PIECE_H

#define PIECE_NONE ((piece_t)0b00000000)

#define PIECE_PAWN ((piece_t)0b00000001)
#define PIECE_KNIGHT ((piece_t)0b00000010)
#define PIECE_BISHOP ((piece_t)0b00000100)
#define PIECE_ROOK ((piece_t)0b00001000)
#define PIECE_QUEEN ((piece_t)0b00010000)
#define PIECE_KING ((piece_t)0b00100000)

#define PIECE_MASK 0b00111111

#define SYMBOL_NONE ' '
#define SYMBOL_PAWN 'P'
#define SYMBOL_KNIGHT 'N'
#define SYMBOL_BISHOP 'B'
#define SYMBOL_ROOK 'R'
#define SYMBOL_QUEEN 'Q'
#define SYMBOL_KING 'K'

typedef unsigned char piece_t;

char piece_symbol(piece_t piece);

#endif