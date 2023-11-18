#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

#define CCHESS_DEBUG

#ifdef CCHESS_DEBUG
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "fen.h"
#include "search.h"
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
    board_print(&board, stderr);

    score_t static_score = 0;
    while ((static_score > -10000) && (static_score < 10000))
    {
        static_score = evaluate(&board);
        printf("Static score: %+d cp\n", static_score);

        score_t search_score;
        move_t best_move;
        const int depth = 6;
        search_score = search(&board, depth, &best_move);
        printf("Score: %+d cp\n", search_score);

        char best_move_uci[6];
        move_to_uci(best_move, best_move_uci);
        printf("Best move: %s\n", best_move_uci);

        board_make_move(&board, best_move);
        board_print(&board, stdout);
    }

    return EXIT_SUCCESS;
#endif
}
