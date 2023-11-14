#include "movegen.h"
#include "side.h"

#define MIN_MOVE_DISTANCE 1
#define MAX_MOVE_DISTANCE 7

#define NOT_ADDED 0
#define ADDED_EMPTY 1
#define ADDED_ENEMY 2

void _movegen_generate_pawn_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_white_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_black_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);

void _movegen_generate_knight_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_bishop_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_rook_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_queen_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);

void _movegen_generate_king_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_king_regular(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_king_castling(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);

/**
 * Adds a move to the array of moves if the target square is empty or contains an enemy piece.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 * @param from The square the piece is moving from.
 * @param to The square the piece is moving to.
 *
 * @return NOT_ADDED if the move was not added,
 *         ADDED_EMPTY if the move was added because the target square was empty,
 *         ADDED_ENEMY if the move was added because the target square contained an enemy piece.
 */
int _movegen_add_if_target_empty_or_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to);

/**
 * Adds a move to the array of moves if the target square contains an enemy piece.
 *
 * @param board The board to generate moves for.
 * @param out_moves The array to store the generated moves in.
 * @param out_move_count The integer to store the number of moves generated.
 * @param from The square the piece is moving from.
 * @param to The square the piece is moving to.
 */
void _movegen_add_if_target_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to);

void movegen_generate(const board_t *board, move_t *out_moves, int *out_move_count)
{
    *out_move_count = 0;
    side_t side = board->side_to_move;

    for (int file = FILE_A; file <= FILE_H; file++)
        for (int rank = RANK_1; rank <= RANK_8; rank++)
        {
            square_t square = square_of(file, rank);
            piece_t piece = board_get(board, square);
            if (!piece)
                continue;
            if (!(piece & side))
                continue;
            if (piece & PIECE_PAWN)
                _movegen_generate_pawn_moves(board, out_moves, out_move_count, square);
            else if (piece & PIECE_KNIGHT)
                _movegen_generate_knight_moves(board, out_moves, out_move_count, square);
            else if (piece & PIECE_BISHOP)
                _movegen_generate_bishop_moves(board, out_moves, out_move_count, square);
            else if (piece & PIECE_ROOK)
                _movegen_generate_rook_moves(board, out_moves, out_move_count, square);
            else if (piece & PIECE_QUEEN)
                _movegen_generate_queen_moves(board, out_moves, out_move_count, square);
            else if (piece & PIECE_KING)
                _movegen_generate_king_moves(board, out_moves, out_move_count, square);
        }
}

void _movegen_generate_pawn_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    piece_t pawn = board_get(board, square);
    if (pawn & SIDE_WHITE)
    {
        _movegen_generate_white_pawn_pushes(board, out_moves, out_move_count, square);
        _movegen_generate_white_pawn_captures(board, out_moves, out_move_count, square);
    }
    else if (pawn & SIDE_BLACK)
    {
        _movegen_generate_black_pawn_pushes(board, out_moves, out_move_count, square);
        _movegen_generate_black_pawn_captures(board, out_moves, out_move_count, square);
    }
}

void _movegen_generate_knight_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);

    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + 1, rank + 2));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + 2, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + 2, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + 1, rank - 2));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - 1, rank - 2));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - 2, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - 2, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - 1, rank + 2));
}

void _movegen_generate_bishop_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + distance, rank + distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - distance, rank + distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + distance, rank - distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - distance, rank - distance)))
            break;
}

void _movegen_generate_rook_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file, rank + distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + distance, rank)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file, rank - distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - distance, rank)))
            break;
}

void _movegen_generate_queen_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    _movegen_generate_bishop_moves(board, out_moves, out_move_count, square);
    _movegen_generate_rook_moves(board, out_moves, out_move_count, square);
}

void _movegen_generate_king_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    _movegen_generate_king_regular(board, out_moves, out_move_count, square);
    _movegen_generate_king_castling(board, out_moves, out_move_count, square);
}

void _movegen_generate_white_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);
    square_t in_front = square_of(file, rank + 1);

    if (rank == RANK_2)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(square, in_front);
            (*out_move_count)++;

            square_t two_in_front = square_of(file, rank + 2);
            if (board_get(board, two_in_front) == PIECE_NONE)
            {
                // Push 2
                out_moves[*out_move_count] = move_regular(square, two_in_front);
                (*out_move_count)++;
            }
        }
    }
    else if (rank < RANK_7)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(square, in_front);
            (*out_move_count)++;
        }
    }
    else if (rank == RANK_7)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1 and promote to queen
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_QUEEN);
            (*out_move_count)++;

            // ... and promote to rook
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_ROOK);
            (*out_move_count)++;

            // ... and promote to bishop
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_BISHOP);
            (*out_move_count)++;

            // ... and promote to knight
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_KNIGHT);
            (*out_move_count)++;
        }
    }
}

void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);
    int target_rank = rank + 1;

    // Regular captures
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, square_of(file + 1, target_rank));
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, square_of(file - 1, target_rank));

    // En passant capture
    int en_passant_rank = square_rank(board->en_passant_square);
    int en_passant_file = square_file(board->en_passant_square);
    if (en_passant_rank == target_rank && (en_passant_file == file + 1 || en_passant_file == file - 1))
    {
        out_moves[*out_move_count] = move_regular(square, board->en_passant_square);
        (*out_move_count)++;
    }
}

void _movegen_generate_black_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);
    square_t in_front = square_of(file, rank - 1);

    if (rank == RANK_7)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(square, in_front);
            (*out_move_count)++;

            square_t two_in_front = square_of(file, rank - 2);
            if (board_get(board, two_in_front) == PIECE_NONE)
            {
                // Push 2
                out_moves[*out_move_count] = move_regular(square, two_in_front);
                (*out_move_count)++;
            }
        }
    }
    else if (rank > RANK_2)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(square, in_front);
            (*out_move_count)++;
        }
    }
    else if (rank == RANK_2)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1 and promote to queen
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_QUEEN);
            (*out_move_count)++;

            // ... and promote to rook
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_ROOK);
            (*out_move_count)++;

            // ... and promote to bishop
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_BISHOP);
            (*out_move_count)++;

            // ... and promote to knight
            out_moves[*out_move_count] = move_promotion(square, in_front, PIECE_KNIGHT);
            (*out_move_count)++;
        }
    }
}

void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);
    int target_rank = rank - 1;

    // Regular captures
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, square_of(file + 1, target_rank));
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, square_of(file - 1, target_rank));

    // En passant capture
    int en_passant_rank = square_rank(board->en_passant_square);
    int en_passant_file = square_file(board->en_passant_square);
    if (en_passant_rank == target_rank && (en_passant_file == file + 1 || en_passant_file == file - 1))
    {
        out_moves[*out_move_count] = move_regular(square, board->en_passant_square);
        (*out_move_count)++;
    }
}

void _movegen_generate_king_regular(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    int file = square_file(square);
    int rank = square_rank(square);

    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + 1, rank));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - 1, rank));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + 1, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - 1, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file + 1, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, square_of(file - 1, rank - 1));
}

void _movegen_generate_king_castling(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    if (board->side_to_move == SIDE_WHITE)
    {
        const square_t WHITE_KINGSIDE_CASTLING_SQUARE = square_of(FILE_G, RANK_1);
        const square_t WHITE_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE = square_of(FILE_F, RANK_1);

        const square_t WHITE_QUEENSIDE_CASTLING_SQUARE = square_of(FILE_C, RANK_1);
        const square_t WHITE_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE = square_of(FILE_D, RANK_1);
        const square_t WHITE_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE_2 = square_of(FILE_B, RANK_1);

        if (board->white_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        {
            bool is_intermediate_square_empty = board_get(board, WHITE_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE) == PIECE_NONE;
            bool is_target_square_empty = board_get(board, WHITE_KINGSIDE_CASTLING_SQUARE) == PIECE_NONE;

            if (is_intermediate_square_empty && is_target_square_empty)
            {
                out_moves[*out_move_count] = move_regular(square, WHITE_KINGSIDE_CASTLING_SQUARE);
                (*out_move_count)++;
            }
        }

        if (board->white_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
        {
            bool is_intermediate_square_empty = board_get(board, WHITE_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE) == PIECE_NONE;
            bool is_intermediate_square_2_empty = board_get(board, WHITE_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE_2) == PIECE_NONE;
            bool is_target_square_empty = board_get(board, WHITE_QUEENSIDE_CASTLING_SQUARE) == PIECE_NONE;

            if (is_intermediate_square_empty && is_intermediate_square_2_empty && is_target_square_empty)
            {
                out_moves[*out_move_count] = move_regular(square, WHITE_QUEENSIDE_CASTLING_SQUARE);
                (*out_move_count)++;
            }
        }
    }

    else if (board->side_to_move == SIDE_BLACK)
    {
        const square_t BLACK_KINGSIDE_CASTLING_SQUARE = square_of(FILE_G, RANK_8);
        const square_t BLACK_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE = square_of(FILE_F, RANK_8);

        const square_t BLACK_QUEENSIDE_CASTLING_SQUARE = square_of(FILE_C, RANK_8);
        const square_t BLACK_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE = square_of(FILE_D, RANK_8);
        const square_t BLACK_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE_2 = square_of(FILE_B, RANK_8);

        if (board->black_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        {
            bool is_intermediate_square_empty = board_get(board, BLACK_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE) == PIECE_NONE;
            bool is_target_square_empty = board_get(board, BLACK_KINGSIDE_CASTLING_SQUARE) == PIECE_NONE;

            if (is_intermediate_square_empty && is_target_square_empty)
            {
                out_moves[*out_move_count] = move_regular(square, BLACK_KINGSIDE_CASTLING_SQUARE);
                (*out_move_count)++;
            }
        }

        if (board->black_castling_rights & CASTLING_RIGHTS_QUEENSIDE)
        {
            bool is_intermediate_square_empty = board_get(board, BLACK_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE) == PIECE_NONE;
            bool is_intermediate_square_2_empty = board_get(board, BLACK_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE_2) == PIECE_NONE;
            bool is_target_square_empty = board_get(board, BLACK_QUEENSIDE_CASTLING_SQUARE) == PIECE_NONE;

            if (is_intermediate_square_empty && is_intermediate_square_2_empty && is_target_square_empty)
            {
                out_moves[*out_move_count] = move_regular(square, BLACK_QUEENSIDE_CASTLING_SQUARE);
                (*out_move_count)++;
            }
        }
    }
}

int _movegen_add_if_target_empty_or_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to)
{
    if (square_valid(to))
    {
        piece_t target = board_get(board, to);
        bool is_empty = target == PIECE_NONE;
        bool is_enemy = (target & SIDE_MASK) != board->side_to_move;
        if (is_empty || is_enemy)
        {
            out_moves[*out_move_count] = move_regular(from, to);
            (*out_move_count)++;
            return is_empty ? ADDED_EMPTY : ADDED_ENEMY;
        }
    }

    return NOT_ADDED;
}

void _movegen_add_if_target_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to)
{
    if (square_valid(to))
    {
        piece_t target = board_get(board, to);
        bool is_empty = target == PIECE_NONE;
        bool is_enemy = (target & SIDE_MASK) != board->side_to_move;
        if (!is_empty && is_enemy)
        {
            out_moves[*out_move_count] = move_regular(from, to);
            (*out_move_count)++;
        }
    }
}