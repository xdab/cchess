#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "fen.h"
#include "eval.h"

void board_print_with_fen(board_t *board);

int main(int argc, char *argv[])
{
    const char *fen_str = "r1bq1rk1/2p2ppp/p4n2/1pbP4/2PpN3/1B6/PP1P1PPP/R1BQ1RK1 b - c3 0 11";

    board_t board;
    fen_put(&board, fen_str);
    board_print_with_fen(&board);

    move_t en_passant = move_regular(FILE_D, RANK_4, FILE_C, RANK_3);
    board_make_move(&board, en_passant);
    board_print_with_fen(&board);

    move_t luft = move_uci("h2h3");
    board_make_move(&board, luft);
    board_print_with_fen(&board);

    centipawns_t score = eval(&board);
    printf("\nMaterial: %+.2f\n", (double)score / CENTIPAWN_MULTIPLIER);

    return EXIT_SUCCESS;
}

void board_print_with_fen(board_t *board)
{
    char fen[FEN_BUF_LENGTH];
    fen_get(board, fen);
    printf("\n%s\n", fen);
    board_print(board, stdout);
}
