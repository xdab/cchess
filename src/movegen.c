#include "movegen.h"
#include "side.h"

#define MIN_MOVE_DISTANCE 1
#define MAX_MOVE_DISTANCE 7

#define NOT_ADDED 0
#define ADDED_EMPTY 1
#define ADDED_ENEMY 2

void _movegen_generate_pawn_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_white_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_black_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);

void _movegen_generate_knight_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_bishop_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_rook_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_queen_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);

void _movegen_generate_king_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_king_regular(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);
void _movegen_generate_king_castling(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file);

/**
 * Adds a move to the array of moves if the target square is empty or contains an enemy piece.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 * @param rank The rank of the piece to generate moves for.
 * @param file The file of the piece to generate moves for.
 * @param target_rank The rank of the target square.
 * @param target_file The file of the target square.
 *
 * @return NOT_ADDED if the move was not added,
 *         ADDED_EMPTY if the move was added because the target square was empty,
 *         ADDED_ENEMY if the move was added because the target square contained an enemy piece.
 */
int _movegen_add_if_target_empty_or_enemy(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file, int target_rank, int target_file);

void _movegen_add_if_target_enemy(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file, int target_rank, int target_file);

void movegen_generate(const board_t *board, move_t *out_moves, int *out_move_count)
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

void _movegen_generate_pawn_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    piece_t pawn = board_get(board, file, rank);
    if (pawn & SIDE_WHITE)
    {
        _movegen_generate_white_pawn_pushes(board, out_moves, out_move_count, rank, file);
        _movegen_generate_white_pawn_captures(board, out_moves, out_move_count, rank, file);
    }
    else if (pawn & SIDE_BLACK)
    {
        _movegen_generate_black_pawn_pushes(board, out_moves, out_move_count, rank, file);
        _movegen_generate_black_pawn_captures(board, out_moves, out_move_count, rank, file);
    }
}

void _movegen_generate_knight_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
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

void _movegen_generate_bishop_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + distance, file + distance))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + distance, file - distance))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - distance, file + distance))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - distance, file - distance))
            break;
}

void _movegen_generate_rook_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank, file + distance))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank, file - distance))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + distance, file))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - distance, file))
            break;
}

void _movegen_generate_queen_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    _movegen_generate_bishop_moves(board, out_moves, out_move_count, rank, file);
    _movegen_generate_rook_moves(board, out_moves, out_move_count, rank, file);
}

void _movegen_generate_king_moves(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    _movegen_generate_king_regular(board, out_moves, out_move_count, rank, file);
    _movegen_generate_king_castling(board, out_moves, out_move_count, rank, file);
}

void _movegen_generate_white_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
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

void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    int target_rank = rank + 1;
    int target_file;
    piece_t target;

    // Regular captures
    target_file = file + 1;
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, rank, file, target_rank, target_file);

    target_file = file - 1;
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, rank, file, target_rank, target_file);

    // En passant capture
    if (board->en_passant_rank == target_rank && (board->en_passant_file == file + 1 || board->en_passant_file == file - 1))
    {
        out_moves[*out_move_count] = move_regular(file, rank, board->en_passant_file, board->en_passant_rank);
        (*out_move_count)++;
    }
}

void _movegen_generate_black_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    if (rank == RANK_7)
    {
        if (board_get(board, file, rank - 1) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(file, rank, file, rank - 1);
            (*out_move_count)++;

            if (board_get(board, file, rank - 2) == PIECE_NONE)
            {
                // Push 2
                out_moves[*out_move_count] = move_regular(file, rank, file, rank - 2);
                (*out_move_count)++;
            }
        }
    }
    else if (rank > RANK_2)
    {
        if (board_get(board, file, rank - 1) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(file, rank, file, rank - 1);
            (*out_move_count)++;
        }
    }
    else if (rank == RANK_2)
    {
        if (board_get(board, file, rank - 1) == PIECE_NONE)
        {
            // Push 1 and promote to queen
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank - 1, PROMOTION_QUEEN);
            (*out_move_count)++;

            // ... and promote to rook
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank - 1, PROMOTION_ROOK);
            (*out_move_count)++;

            // ... and promote to bishop
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank - 1, PROMOTION_BISHOP);
            (*out_move_count)++;

            // ... and promote to knight
            out_moves[*out_move_count] = move_promotion(file, rank, file, rank - 1, PROMOTION_KNIGHT);
            (*out_move_count)++;
        }
    }
}

void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    int target_rank = rank - 1;
    int target_file;
    piece_t target;

    // Regular captures
    target_file = file + 1;
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, rank, file, target_rank, target_file);

    target_file = file - 1;
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, rank, file, target_rank, target_file);

    // En passant capture
    if (board->en_passant_rank == target_rank && (board->en_passant_file == file + 1 || board->en_passant_file == file - 1))
    {
        out_moves[*out_move_count] = move_regular(file, rank, board->en_passant_file, board->en_passant_rank);
        (*out_move_count)++;
    }
}

void _movegen_generate_king_regular(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + 1, file);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + 1, file + 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank + 1, file - 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank, file + 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank, file - 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - 1, file);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - 1, file + 1);
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, rank, file, rank - 1, file - 1);
}

void _movegen_generate_king_castling(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file)
{
    if (board->side_to_move == SIDE_WHITE)
    {
        if (board->white_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        {
            if (board_get(board, FILE_F, RANK_1) == PIECE_NONE && board_get(board, FILE_G, RANK_1) == PIECE_NONE)
            {
                out_moves[*out_move_count] = move_regular(file, rank, file + 2, rank);
                (*out_move_count)++;
            }
        }

        if (board->white_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
        {
            if (board_get(board, FILE_B, RANK_1) == PIECE_NONE && board_get(board, FILE_C, RANK_1) == PIECE_NONE && board_get(board, FILE_D, RANK_1) == PIECE_NONE)
            {
                out_moves[*out_move_count] = move_regular(file, rank, file - 2, rank);
                (*out_move_count)++;
            }
        }
    }

    else if (board->side_to_move == SIDE_BLACK)
    {
        if (board->black_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        {
            if (board_get(board, FILE_F, RANK_8) == PIECE_NONE && board_get(board, FILE_G, RANK_8) == PIECE_NONE)
            {
                out_moves[*out_move_count] = move_regular(file, rank, file + 2, rank);
                (*out_move_count)++;
            }
        }

        if (board->black_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
        {
            if (board_get(board, FILE_B, RANK_8) == PIECE_NONE && board_get(board, FILE_C, RANK_8) == PIECE_NONE && board_get(board, FILE_D, RANK_8) == PIECE_NONE)
            {
                out_moves[*out_move_count] = move_regular(file, rank, file - 2, rank);
                (*out_move_count)++;
            }
        }
    }
}

int _movegen_add_if_target_empty_or_enemy(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file, int target_rank, int target_file)
{
    if (target_rank >= RANK_1 && target_rank <= RANK_8 && target_file >= FILE_A && target_file <= FILE_H)
    {
        piece_t target = board_get(board, target_file, target_rank);
        bool is_empty = target == PIECE_NONE;
        bool is_enemy = (target & SIDE_MASK) != board->side_to_move;
        if (is_empty || is_enemy)
        {
            out_moves[*out_move_count] = move_regular(file, rank, target_file, target_rank);
            (*out_move_count)++;
            return is_empty ? ADDED_EMPTY : ADDED_ENEMY;
        }
    }

    return NOT_ADDED;
}

void _movegen_add_if_target_enemy(const board_t *board, move_t *out_moves, int *out_move_count, int rank, int file, int target_rank, int target_file)
{
    if (target_rank >= RANK_1 && target_rank <= RANK_8 && target_file >= FILE_A && target_file <= FILE_H)
    {
        piece_t target = board_get(board, target_file, target_rank);
        bool is_empty = target == PIECE_NONE;
        bool is_enemy = (target & SIDE_MASK) != board->side_to_move;
        if (!is_empty && is_enemy)
        {
            out_moves[*out_move_count] = move_regular(file, rank, target_file, target_rank);
            (*out_move_count)++;
        }
    }
}