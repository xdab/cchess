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
    board_init(&board);

    board_make_move(&board, move_uci("e2e4")); // e4 - King's Pawn Opening
    board_make_move(&board, move_uci("e7e5")); // e5 - Open Game
    board_make_move(&board, move_uci("g1f3")); // Nf3 - King's Knight Variation
    board_make_move(&board, move_uci("b8c6")); // Nc6 - Queen's Knight defends
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
    // outputs f1b5 (Bb5)

    return EXIT_SUCCESS;
#endif
}
