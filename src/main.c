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
    board_print(&board, stderr);

    move_t moves[256];
    int move_count = 0;
    movegen_generate(&board, moves, &move_count);

    char uci_move[6];
    for (int i = 0; i < move_count; i++)
    {
        move_to_uci(moves[i], uci_move);
        fprintf(stderr, "%s ", uci_move);
    }
    fputs("\n", stderr);

    fprintf(stderr, "Move count: %d\n", move_count);

    return EXIT_SUCCESS;
#endif
}
