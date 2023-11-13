#ifndef DEEP_EVAL_H
#define DEEP_EVAL_H

#include "board.h"
#include "move.h"
#include "eval.h"

centipawns_t deep_eval(const board_t *board, int depth);

#endif