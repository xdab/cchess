#ifndef SEARCH_H
#define SEARCH_H

#include "eval.h"
#include "board.h"
#include "move.h"

extern int nodes;
extern int qs_nodes;
extern int ttable_hits;

score_t search(board_t *board, int depth, move_t *best_move);

#endif