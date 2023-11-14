#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

#define CCHESS_DEBUG

#ifdef CCHESS_DEBUG
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "fen.h"
#include "deepeval.h"
#include "random.h"
#include "zobrist.h"
#endif

int main(int argc, char *argv[])
{
#ifndef CCHESS_DEBUG
    fputs("+-------------------------------------+\n", stderr);
    fputs("| CChess          development version |\n", stderr);
    fputs("| Amateur UCI-compatible chess engine |\n", stderr);
    fputs("+-------------------------------------+\n", stderr);
    return uci_loop();
#else
    board_t board;
    zobrist_init();

    board_init(&board);
    board_make_move(&board, move_uci("e2e4"));  
    fprintf(stderr, "hash after e2e4: 0x%016lX\n", board.hash);
    board_make_move(&board, move_uci("e7e5")); 
    fprintf(stderr, "hash after e7e5: 0x%016lX\n", board.hash);
    board_make_move(&board, move_uci("d2d4"));
    fprintf(stderr, "hash after d2d4: 0x%016lX\n", board.hash);
    board_make_move(&board, move_uci("d7d5"));
    fprintf(stderr, "hash after d7d5: 0x%016lX\n", board.hash);

    board_print(&board, stderr);

    board_init(&board);
    board_make_move(&board, move_uci("d7d5"));
    fprintf(stderr, "hash after d2d4: 0x%016lX\n", board.hash);
    board_make_move(&board, move_uci("e2e4")); 
    fprintf(stderr, "hash after d7d5: 0x%016lX\n", board.hash);
    board_make_move(&board, move_uci("e7e5"));
    fprintf(stderr, "hash after e2e4: 0x%016lX\n", board.hash);
    board_make_move(&board, move_uci("d2d4"));
    fprintf(stderr, "hash after e7e5: 0x%016lX\n", board.hash);

    board_print(&board, stderr);

    centipawns_t score = eval(&board);
    fprintf(stderr, "\neval : %+f\n", score / CENTIPAWN_MULTIPLIER);

    const int depth = 4;
    move_t best_move;
    centipawns_t deep_score = deep_eval(&board, depth, &best_move);
    fprintf(stderr, "deep_eval: %+f\n", deep_score / CENTIPAWN_MULTIPLIER);

    char best_move_uci[6];
    move_to_uci(best_move, best_move_uci);
    fprintf(stderr, "best_move: %s\n", best_move_uci);

    return EXIT_SUCCESS;
#endif
}
