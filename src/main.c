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

void nice_print(const board_t *board)
{
    printf("\n\n\n---------Board---------\n");
    board_print(board, stderr);
    printf("---------White---------\n");
    piecepos_print(&board->white_piece_positions, stderr);
    printf("---------Black---------\n");
    piecepos_print(&board->black_piece_positions, stderr);
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

    board_init(&board);

    board_make_move(&board, move_regular(E7, E5));
    nice_print(&board);

    score_t static_score = 0;
    while ((static_score > -10000) && (static_score < 10000))
    {
        static_score = evaluate(&board);
        printf("Static score: %+d cp\n", static_score);

        score_t search_score;
        move_t best_move;
        const int depth = 6;
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
