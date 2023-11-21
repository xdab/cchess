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

    if (pieces->has_pawns)
        for (int i = 0; i < 8; i++)
            if (pieces->pawns[i] != SQUARE_NONE)
                movegen_generate_pawn_moves(board, out_moves, &move_count, pieces->pawns[i]);

    if (pieces->has_promoted_pieces)
    {
        if (pieces->has_promoted_queens)
            for (int i = 0; i < 8; i++)
                if (pieces->promoted_queens[i] != SQUARE_NONE)
                    movegen_generate_queen_moves(board, out_moves, &move_count, pieces->promoted_queens[i]);

        if (pieces->has_promoted_rooks)
            for (int i = 0; i < 8; i++)
                if (pieces->promoted_rooks[i] != SQUARE_NONE)
                    movegen_generate_rook_moves(board, out_moves, &move_count, pieces->promoted_rooks[i]);

        if (pieces->has_promoted_bishops)
            for (int i = 0; i < 8; i++)
                if (pieces->promoted_bishops[i] != SQUARE_NONE)
                    movegen_generate_bishop_moves(board, out_moves, &move_count, pieces->promoted_bishops[i]);

        if (pieces->has_promoted_knights)
            for (int i = 0; i < 8; i++)
                if (pieces->promoted_knights[i] != SQUARE_NONE)
                    movegen_generate_knight_moves(board, out_moves, &move_count, pieces->promoted_knights[i]);
    }

    if (pieces->queen != SQUARE_NONE)
        movegen_generate_queen_moves(board, out_moves, &move_count, pieces->queen);

    if (pieces->kings_rook != SQUARE_NONE)
        movegen_generate_rook_moves(board, out_moves, &move_count, pieces->kings_rook);

    if (pieces->queens_rook != SQUARE_NONE)
        movegen_generate_rook_moves(board, out_moves, &move_count, pieces->queens_rook);

    if (pieces->kings_bishop != SQUARE_NONE)
        movegen_generate_bishop_moves(board, out_moves, &move_count, pieces->kings_bishop);

    if (pieces->queens_bishop != SQUARE_NONE)
        movegen_generate_bishop_moves(board, out_moves, &move_count, pieces->queens_bishop);

    if (pieces->kings_knight != SQUARE_NONE)
        movegen_generate_knight_moves(board, out_moves, &move_count, pieces->kings_knight);

    if (pieces->queens_knight != SQUARE_NONE)
        movegen_generate_knight_moves(board, out_moves, &move_count, pieces->queens_knight);

    if (pieces->king != SQUARE_NONE)
        movegen_generate_king_moves(board, out_moves, &move_count, pieces->king);

    return move_count;
}
