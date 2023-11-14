#ifndef ZOBRIST_H
#define ZOBRIST_H

typedef struct board board_t;
typedef unsigned long zobrist_t;

#include "board.h"

void zobrist_init();

zobrist_t zobrist_hash(const board_t *board);

zobrist_t zobrist_update_piece(const board_t *board, square_t square, piece_t old_piece, piece_t new_piece);

#endif