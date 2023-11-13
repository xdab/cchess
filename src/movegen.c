#include "movegen.h"
#include "side.h"

void _movegen_generate_pawn_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_white_pawn_pushes(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_white_pawn_captures(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_black_pawn_pushes(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_black_pawn_captures(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);

void _movegen_generate_knight_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_bishop_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_rook_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_queen_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);

void _movegen_generate_king_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_king_regular(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_king_castling(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);

void _movegen_add_if_target_empty_or_enemy(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file, int target_rank, int target_file);

void movegen_generate(board_t *board, move_t *out_moves, int *out_move_count)
{
    *out_move_count = 0;
    side_t side = board->side_to_move;

    for (int file = FILE_A; file <= FILE_H; file++)
        for (int rank = RANK_1; rank <= RANK_8; rank++)
        {
            piece_t piece = board_get(board, file, rank);
            if (!piece)
                continue;
            if (!(piece & side))
                continue;
            if (piece & PIECE_PAWN)
                _movegen_generate_pawn_moves(board, out_moves, out_move_count, rank, file);
            else if (piece & PIECE_KNIGHT)
                _movegen_generate_knight_moves(board, out_moves, out_move_count, rank, file);
            else if (piece & PIECE_BISHOP)
                _movegen_generate_bishop_moves(board, out_moves, out_move_count, rank, file);
            else if (piece & PIECE_ROOK)
                _movegen_generate_rook_moves(board, out_moves, out_move_count, rank, file);
            else if (piece & PIECE_QUEEN)
                _movegen_generate_queen_moves(board, out_moves, out_move_count, rank, file);
            else if (piece & PIECE_KING)
                _movegen_generate_king_moves(board, out_moves, out_move_count, rank, file);
        }
}

void _movegen_generate_pawn_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    piece_t pawn = board_get(board, file, rank);
    if (pawn & SIDE_WHITE)
    {
        _movegen_generate_white_pawn_pushes(board, out_moves, out_move_count, rank, file);
        // _movegen_generate_white_pawn_captures(board, out_moves, out_move_count, rank, file);
    }
    else if (pawn & SIDE_BLACK)
    {
        // _movegen_generate_black_pawn_pushes(board, out_moves, out_move_count, rank, file);
        // _movegen_generate_black_pawn_captures(board, out_moves, out_move_count, rank, file);
    }
}

void _movegen_generate_knight_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + 2, file + 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + 2, file - 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - 2, file + 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - 2, file - 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + 1, file + 2);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + 1, file - 2);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - 1, file + 2);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - 1, file - 2);
}

void _movegen_generate_bishop_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
}

void _movegen_generate_rook_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
}

void _movegen_generate_queen_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
}

void _movegen_generate_king_moves(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
}

void _movegen_generate_white_pawn_pushes(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    if (rank == RANK_2)
    {
        if (board_get(board, file, rank + 1) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(file, rank, file, rank + 1);
            (*out_move_count)++;

            if (board_get(board, file, rank + 2) == PIECE_NONE)
            {
                // Push 2
                out_moves[*out_move_count] = move_regular(file, rank, file, rank + 2);
                (*out_move_count)++;
            }
        }
    }
    else if (rank < RANK_7)
    {
        if (board_get(board, file, rank + 1) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(file, rank, file, rank + 1);
            (*out_move_count)++;
        }
    }
    else if (rank == RANK_7)
    {
        if (board_get(board, file, rank + 1) == PIECE_NONE)
        {
            // Push 1 and promote to queen
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank + 1, PROMOTION_QUEEN);
            (*out_move_count)++;

            // ... and promote to rook
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank + 1, PROMOTION_ROOK);
            (*out_move_count)++;

            // ... and promote to bishop
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank + 1, PROMOTION_BISHOP);
            (*out_move_count)++;

            // ... and promote to knight
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank + 1, PROMOTION_KNIGHT);
            (*out_move_count)++;
        }
    }
}

void _movegen_add_if_target_empty_or_enemy(board_t *board, move_t *out_moves, int *out_move_count, int rank, int file, int target_rank, int target_file)
{
    if (target_rank >= RANK_1 && target_rank <= RANK_8 && target_file >= FILE_A && target_file <= FILE_H)
    {
        piece_t target = board_get(board, target_file, target_rank);

        bool is_empty = target == PIECE_NONE;
        bool is_enemy = (target & SIDE_MASK) != board->side_to_move;
        bool is_king = target & PIECE_KING;

        if (is_empty || (is_enemy && !is_king))
        {
            out_moves[*out_move_count] = move_regular(file, rank, target_file, target_rank);
            (*out_move_count)++;
        }
    }
}