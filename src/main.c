#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

#define CCHESS_DEBUG

#ifdef CCHESS_DEBUG
#include "board.h"
#include "board_fen.h"
#include "board_move.h"
#include "board_pieces.h"
#include "move.h"
#include "movegen.h"
#include "random.h"
#include "zobrist.h"
#include "ttable.h"
#include "search.h"
#endif

void nice_print(const board_t *board)
{
    board_print(board, stderr);
    printf("------------------------\n\n");
}

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

    const unsigned long tt_megabyte_factor = (1 << 20) / sizeof(ttable_entry_t);
    const unsigned long transposition_table_size = 1024 * tt_megabyte_factor;
    ttable_init(transposition_table_size);

    board_init(&board);

    board_move(&board, move_regular(E2, E4));
    board_move(&board, move_regular(E7, E5));
    board_move(&board, move_regular(D1, H5));
    board_move(&board, move_regular(B8, C6));
    board_move(&board, move_regular(F1, C4));

    score_t static_score = 0;
    while ((static_score > -10000) && (static_score < 10000))
    {
        static_score = evaluate(&board);
        printf("Static score: %+.1f \n", static_score / CENTIPAWN_MULTIPLIER);

        score_t search_score;
        move_t best_move;
        const int depth = 5;

        search_score = search(&board, depth, &best_move);
        printf("AB-Nodes searched: %d \n", nodes);
        printf("QS-Nodes searched: %d \n", qs_nodes);
        printf("TT hits: %d \n", ttable_hits);
        printf("Score: %+.1f \n", search_score / CENTIPAWN_MULTIPLIER);

        char best_move_uci[6];
        move_to_uci(best_move, best_move_uci);
        printf("Making move: %s\n", best_move_uci);

        board_move(&board, best_move);
        nice_print(&board);
    }

    return EXIT_SUCCESS;
#endif
}
