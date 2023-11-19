#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

#define CCHESS_DEBUG

#ifdef CCHESS_DEBUG
#include "board.h"
#include "board_fen.h"
#include "board_move.h"
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
    board_t work_board;

    zobrist_init();

    const int transposition_table_size = 1 << 18;
    ttable_init(transposition_table_size);

    board_init(&board);

    board_make_move(&board, move_regular(E2, E4));
    board_make_move(&board, move_regular(E7, E5));
    board_make_move(&board, move_regular(D1, H5));
    board_make_move(&board, move_regular(B8, C6));

    score_t static_score = 0;
    while ((static_score > -10000) && (static_score < 10000))
    {
        static_score = evaluate(&board);
        printf("Static score: %+d cp\n", static_score);

        score_t search_score;
        move_t best_move;
        const int depth = 4;
        board_clone(&board, &work_board);
        search_score = search(&work_board, depth, &best_move);
        printf("Score: %+d cp\n", search_score);

        char best_move_uci[6];
        move_to_uci(best_move, best_move_uci);
        printf("Making move: %s\n", best_move_uci);

        board_make_move(&board, best_move);
        nice_print(&board);
    }

    return EXIT_SUCCESS;
#endif
}
