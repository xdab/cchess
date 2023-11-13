#ifndef PIECE_H
#define PIECE_H

#define PIECE_NONE 0
#define PIECE_PAWN 1
#define PIECE_KNIGHT 2
#define PIECE_BISHOP 4
#define PIECE_ROOK 8  
#define PIECE_QUEEN 16
#define PIECE_KING 32

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