#include "movegen.h"
#include "movegen_pawn.h"
#include "movegen_knight.h"
#include "movegen_bishop.h"
#include "movegen_rook.h"
#include "movegen_queen.h"
#include "movegen_king.h"
#include "movegen_common.h"
#include "side.h"

int _movegen_generate(const board_t *board, const board_pieces_t *pieces, move_t *out_moves);

void movegen_generate(const board_t *board, move_t *out_moves, int *out_move_count)
{
    const board_pieces_t *pieces = (board->side_to_move == WHITE) ? &board->white_pieces : &board->black_pieces;
    *out_move_count = _movegen_generate(board, pieces, out_moves);
}

int _movegen_generate(const board_t *board, const board_pieces_t *pieces, move_t *out_moves)
{
    int move_count = 0;

    for (int i = 0; i < MAX_PAWNS; i++)
        if (pieces->pawns[i] != SQUARE_NONE)
            movegen_generate_pawn_moves(board, out_moves, &move_count, pieces->pawns[i]);

    for (int i = 0; i < MAX_QUEENS; i++)
        if (pieces->queens[i] != SQUARE_NONE)
            movegen_generate_queen_moves(board, out_moves, &move_count, pieces->queens[i]);

    for (int i = 0; i < MAX_ROOKS; i++)
        if (pieces->rooks[i] != SQUARE_NONE)
            movegen_generate_rook_moves(board, out_moves, &move_count, pieces->rooks[i]);

    for (int i = 0; i < MAX_BISHOPS; i++)
        if (pieces->bishops[i] != SQUARE_NONE)
            movegen_generate_bishop_moves(board, out_moves, &move_count, pieces->bishops[i]);

    for (int i = 0; i < MAX_KNIGHTS; i++)
        if (pieces->knights[i] != SQUARE_NONE)
            movegen_generate_knight_moves(board, out_moves, &move_count, pieces->knights[i]);

    if (pieces->king != SQUARE_NONE)
        movegen_generate_king_moves(board, out_moves, &move_count, pieces->king);

    return move_count;
}
