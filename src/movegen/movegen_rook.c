#include "movegen_rook.h"
#include "movegen_common.h"

#define MG_COND_ROOK (MG_EMPTY | MG_ENEMY)

void movegen_generate_rook_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
    file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (MG_EMPTY != movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file, rank + distance)), MG_COND_ROOK))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (MG_EMPTY != movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file + distance, rank)), MG_COND_ROOK))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (MG_EMPTY != movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file, rank - distance)), MG_COND_ROOK))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (MG_EMPTY != movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file - distance, rank)), MG_COND_ROOK))
            break;
}