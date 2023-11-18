#include "movegen.h"
#include "side.h"

#define MIN_MOVE_DISTANCE 1
#define MAX_MOVE_DISTANCE 7

#define NOT_ADDED 0
#define ADDED_EMPTY 1
#define ADDED_ENEMY 2

const square_t WHITE_KINGSIDE_CASTLING_SQUARE = G1;
const square_t WHITE_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE = F1;
const square_t WHITE_KINGSIDE_CASTLING_INITIAL_ROOK_SQUARE = H1;

const square_t WHITE_QUEENSIDE_CASTLING_SQUARE = C1;
const square_t WHITE_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE = B1;
const square_t WHITE_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE_2 = D1;
const square_t WHITE_QUEENSIDE_CASTLING_INITIAL_ROOK_SQUARE = A1;

const square_t BLACK_KINGSIDE_CASTLING_SQUARE = G8;
const square_t BLACK_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE = F8;
const square_t BLACK_KINGSIDE_CASTLING_INITIAL_ROOK_SQUARE = H8;

const square_t BLACK_QUEENSIDE_CASTLING_SQUARE = C8;
const square_t BLACK_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE = B8;
const square_t BLACK_QUEENSIDE_CASTLING_INTERMEDIATE_SQUARE_2 = D8;
const square_t BLACK_QUEENSIDE_CASTLING_INITIAL_ROOK_SQUARE = A8;

int _movegen_generate(const board_t *board, const piecepos_t *piece_positions, move_t *out_moves);

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
    const piecepos_t *piece_positions = (board->side_to_move == WHITE) ? &board->white_piece_positions : &board->black_piece_positions;
    *out_move_count = _movegen_generate(board, piece_positions, out_moves);
}

int _movegen_generate(const board_t *board, const piecepos_t *piece_positions, move_t *out_moves)
{
    int move_count = 0;

    if (piece_positions->has_promoted_pieces)
    {
        if (piece_positions->has_promoted_queens)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_queens[i] != SQUARE_NONE)
                    _movegen_generate_queen_moves(board, out_moves, &move_count, piece_positions->promoted_queens[i]);

        if (piece_positions->has_promoted_rooks)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_rooks[i] != SQUARE_NONE)
                    _movegen_generate_rook_moves(board, out_moves, &move_count, piece_positions->promoted_rooks[i]);

        if (piece_positions->has_promoted_bishops)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_bishops[i] != SQUARE_NONE)
                    _movegen_generate_bishop_moves(board, out_moves, &move_count, piece_positions->promoted_bishops[i]);

        if (piece_positions->has_promoted_knights)
            for (int i = 0; i < 8; i++)
                if (piece_positions->promoted_knights[i] != SQUARE_NONE)
                    _movegen_generate_knight_moves(board, out_moves, &move_count, piece_positions->promoted_knights[i]);
    }

    if (piece_positions->queen != SQUARE_NONE)
        _movegen_generate_queen_moves(board, out_moves, &move_count, piece_positions->queen);

    if (piece_positions->kings_rook != SQUARE_NONE)
        _movegen_generate_rook_moves(board, out_moves, &move_count, piece_positions->kings_rook);

    if (piece_positions->queens_rook != SQUARE_NONE)
        _movegen_generate_rook_moves(board, out_moves, &move_count, piece_positions->queens_rook);

    if (piece_positions->kings_bishop != SQUARE_NONE)
        _movegen_generate_bishop_moves(board, out_moves, &move_count, piece_positions->kings_bishop);

    if (piece_positions->queens_bishop != SQUARE_NONE)
        _movegen_generate_bishop_moves(board, out_moves, &move_count, piece_positions->queens_bishop);

    if (piece_positions->kings_knight != SQUARE_NONE)
        _movegen_generate_knight_moves(board, out_moves, &move_count, piece_positions->kings_knight);

    if (piece_positions->queens_knight != SQUARE_NONE)
        _movegen_generate_knight_moves(board, out_moves, &move_count, piece_positions->queens_knight);

    if (piece_positions->has_pawns)
        for (int i = 0; i < 8; i++)
            if (piece_positions->pawns[i] != SQUARE_NONE)
                _movegen_generate_pawn_moves(board, out_moves, &move_count, piece_positions->pawns[i]);

    if (piece_positions->king != SQUARE_NONE)
        _movegen_generate_king_moves(board, out_moves, &move_count, piece_positions->king);

    return move_count;
}

void _movegen_generate_pawn_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    piece_t pawn = board_get(board, square);
    if (pawn & WHITE)
    {
        _movegen_generate_white_pawn_pushes(board, out_moves, out_move_count, square);
        _movegen_generate_white_pawn_captures(board, out_moves, out_move_count, square);
    }
    else if (pawn & BLACK)
    {
        _movegen_generate_black_pawn_pushes(board, out_moves, out_move_count, square);
        _movegen_generate_black_pawn_captures(board, out_moves, out_move_count, square);
    }
}

void _movegen_generate_knight_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);

    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, rank + 2));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 2, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 2, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, rank - 2));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, rank - 2));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 2, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 2, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, rank + 2));
}

void _movegen_generate_bishop_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + distance, rank + distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - distance, rank + distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + distance, rank - distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - distance, rank - distance)))
            break;
}

void _movegen_generate_rook_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file, rank + distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + distance, rank)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file, rank - distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - distance, rank)))
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
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);
    square_t in_front = SQUARE_OF(file, rank + 1);

    if (rank == RANK_2)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(square, in_front);
            (*out_move_count)++;

            square_t two_in_front = SQUARE_OF(file, RANK_4);
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
            out_moves[*out_move_count] = move_promotion(square, in_front, QUEEN);
            (*out_move_count)++;

            // ... and promote to rook
            out_moves[*out_move_count] = move_promotion(square, in_front, ROOK);
            (*out_move_count)++;

            // ... and promote to bishop
            out_moves[*out_move_count] = move_promotion(square, in_front, BISHOP);
            (*out_move_count)++;

            // ... and promote to knight
            out_moves[*out_move_count] = move_promotion(square, in_front, KNIGHT);
            (*out_move_count)++;
        }
    }
}

void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);
    int target_rank = rank + 1;

    // Regular captures
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, target_rank));
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, target_rank));

    // En passant capture
    int en_passant_rank = SQUARE_RANK(board->en_passant_square);
    int en_passant_file = SQUARE_FILE(board->en_passant_square);
    if (en_passant_rank == target_rank && (en_passant_file == file + 1 || en_passant_file == file - 1))
    {
        out_moves[*out_move_count] = move_regular(square, board->en_passant_square);
        (*out_move_count)++;
    }
}

void _movegen_generate_black_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);
    square_t in_front = SQUARE_OF(file, rank - 1);

    if (rank == RANK_7)
    {
        if (board_get(board, in_front) == PIECE_NONE)
        {
            // Push 1
            out_moves[*out_move_count] = move_regular(square, in_front);
            (*out_move_count)++;

            square_t two_in_front = SQUARE_OF(file, RANK_5);
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
            out_moves[*out_move_count] = move_promotion(square, in_front, QUEEN);
            (*out_move_count)++;

            // ... and promote to rook
            out_moves[*out_move_count] = move_promotion(square, in_front, ROOK);
            (*out_move_count)++;

            // ... and promote to bishop
            out_moves[*out_move_count] = move_promotion(square, in_front, BISHOP);
            (*out_move_count)++;

            // ... and promote to knight
            out_moves[*out_move_count] = move_promotion(square, in_front, KNIGHT);
            (*out_move_count)++;
        }
    }
}

void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);
    int target_rank = rank - 1;

    // Regular captures
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, target_rank));
    _movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, target_rank));

    // En passant capture
    int en_passant_rank = SQUARE_RANK(board->en_passant_square);
    int en_passant_file = SQUARE_FILE(board->en_passant_square);
    if (en_passant_rank == target_rank && (en_passant_file == file + 1 || en_passant_file == file - 1))
    {
        out_moves[*out_move_count] = move_regular(square, board->en_passant_square);
        (*out_move_count)++;
    }
}

void _movegen_generate_king_regular(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);

    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, rank));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, rank));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, rank + 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, rank - 1));
    _movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, rank - 1));
}

void _movegen_generate_king_castling(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    if (board->side_to_move == WHITE)
    {
        if (board->white_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        {
            bool is_intermediate_square_empty = board_get(board, WHITE_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE) == PIECE_NONE;
            bool is_target_square_empty = board_get(board, WHITE_KINGSIDE_CASTLING_SQUARE) == PIECE_NONE;
            piece_t initial_rook_square_piece = board_get(board, WHITE_KINGSIDE_CASTLING_INITIAL_ROOK_SQUARE);
            bool is_initial_rook_square_white_rook = initial_rook_square_piece == (ROOK | WHITE);

            if (is_intermediate_square_empty && is_target_square_empty & is_initial_rook_square_white_rook)
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
            piece_t initial_rook_square_piece = board_get(board, WHITE_QUEENSIDE_CASTLING_INITIAL_ROOK_SQUARE);
            bool is_initial_rook_square_white_rook = initial_rook_square_piece == (ROOK | WHITE);

            if (is_intermediate_square_empty && is_intermediate_square_2_empty && is_target_square_empty && is_initial_rook_square_white_rook)
            {
                out_moves[*out_move_count] = move_regular(square, WHITE_QUEENSIDE_CASTLING_SQUARE);
                (*out_move_count)++;
            }
        }
    }

    else if (board->side_to_move == BLACK)
    {
        if (board->black_castling_rights & CASTLING_RIGHTS_KINGSIDE)
        {
            bool is_intermediate_square_empty = board_get(board, BLACK_KINGSIDE_CASTLING_INTERMEDIATE_SQUARE) == PIECE_NONE;
            bool is_target_square_empty = board_get(board, BLACK_KINGSIDE_CASTLING_SQUARE) == PIECE_NONE;
            piece_t initial_rook_square_piece = board_get(board, BLACK_KINGSIDE_CASTLING_INITIAL_ROOK_SQUARE);
            bool is_initial_rook_square_black_rook = initial_rook_square_piece == (ROOK | BLACK);

            if (is_intermediate_square_empty && is_target_square_empty && is_initial_rook_square_black_rook)
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
            piece_t initial_rook_square_piece = board_get(board, BLACK_QUEENSIDE_CASTLING_INITIAL_ROOK_SQUARE);
            bool is_initial_rook_square_black_rook = initial_rook_square_piece == (ROOK | BLACK);

            if (is_intermediate_square_empty && is_intermediate_square_2_empty && is_target_square_empty && is_initial_rook_square_black_rook)
            {
                out_moves[*out_move_count] = move_regular(square, BLACK_QUEENSIDE_CASTLING_SQUARE);
                (*out_move_count)++;
            }
        }
    }
}

int _movegen_add_if_target_empty_or_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to)
{
    if (SQUARE_VALID(to))
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
    if (SQUARE_VALID(to))
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