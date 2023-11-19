#include "movegen.h"
#include "movegen_pawn.h"
#include "movegen_knight.h"
#include "movegen_bishop.h"
#include "movegen_rook.h"
#include "movegen_queen.h"
#include "movegen_king.h"
#include "movegen_common.h"
#include "side.h"

int _movegen_generate(const board_t *board, const piecepos_t *piece_positions, move_t *out_moves);

void movegen_generate(const board_t *board, move_t *out_moves, int *out_move_count)
{
    const piecepos_t *piece_positions = (board->side_to_move == WHITE) ? &board->white_piece_positions : &board->black_piece_positions;
    *out_move_count = _movegen_generate(board, piece_positions, out_moves);
}

int _movegen_generate(const board_t *board, const piecepos_t *piece_positions, move_t *out_moves)
{
    int move_count = 0;

    if (piece_positions->has_pawns)
        for (int i = 0; i < 8; i++)
            if (piece_positions->pawns[i] != SQUARE_NONE)
                movegen_generate_pawn_moves(board, out_moves, &move_count, piece_positions->pawns[i]);

    if (piece_positions->has_promoted_pieces)
    {
        if (piece_positions->has_promoted_queens)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_queens[i] != SQUARE_NONE)
                    movegen_generate_queen_moves(board, out_moves, &move_count, piece_positions->promoted_queens[i]);

        if (piece_positions->has_promoted_rooks)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_rooks[i] != SQUARE_NONE)
                    movegen_generate_rook_moves(board, out_moves, &move_count, piece_positions->promoted_rooks[i]);

        if (piece_positions->has_promoted_bishops)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_bishops[i] != SQUARE_NONE)
                    movegen_generate_bishop_moves(board, out_moves, &move_count, piece_positions->promoted_bishops[i]);

        if (piece_positions->has_promoted_knights)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_knights[i] != SQUARE_NONE)
                    movegen_generate_knight_moves(board, out_moves, &move_count, piece_positions->promoted_knights[i]);
    }

    if (piece_positions->queen != SQUARE_NONE)
        movegen_generate_queen_moves(board, out_moves, &move_count, piece_positions->queen);

    if (piece_positions->kings_rook != SQUARE_NONE)
        movegen_generate_rook_moves(board, out_moves, &move_count, piece_positions->kings_rook);

    if (piece_positions->queens_rook != SQUARE_NONE)
        movegen_generate_rook_moves(board, out_moves, &move_count, piece_positions->queens_rook);

    if (piece_positions->kings_bishop != SQUARE_NONE)
        movegen_generate_bishop_moves(board, out_moves, &move_count, piece_positions->kings_bishop);

    if (piece_positions->queens_bishop != SQUARE_NONE)
        movegen_generate_bishop_moves(board, out_moves, &move_count, piece_positions->queens_bishop);

    if (piece_positions->kings_knight != SQUARE_NONE)
        movegen_generate_knight_moves(board, out_moves, &move_count, piece_positions->kings_knight);

    if (piece_positions->queens_knight != SQUARE_NONE)
        movegen_generate_knight_moves(board, out_moves, &move_count, piece_positions->queens_knight);

    if (piece_positions->king != SQUARE_NONE)
        movegen_generate_king_moves(board, out_moves, &move_count, piece_positions->king);

    return move_count;
}
