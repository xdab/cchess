#include "uci.h"

#include <stdio.h>
#include <stdlib.h>

#define CCHESS_DEBUG

#ifdef CCHESS_DEBUG
#include "board.h"
#include "move.h"
#include "movegen.h"
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

    move_t moves[MAX_MOVES];
    int move_count;
    movegen_generate(&board, moves, &move_count);

    fprintf(stderr, "move_count: %d\n", move_count);
    for (int i = 0; i < move_count; i++)
    {
        char move_str[6];
        move_to_uci(moves[i], move_str);
        fprintf(stderr, "%s ", move_str);
    }
    fprintf(stderr, "\n");

    return EXIT_SUCCESS;
#endif
}
