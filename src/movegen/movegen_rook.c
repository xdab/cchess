#include "movegen_rook.h"
#include "movegen_common.h"

void movegen_generate_rook_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
    rank_t rank = SQUARE_RANK(square);

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file, rank + distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + distance, rank)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file, rank - distance)))
            break;

    for (int distance = MIN_MOVE_DISTANCE; distance <= MAX_MOVE_DISTANCE; distance++)
        if (ADDED_EMPTY != movegen_add_if_target_empty_or_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - distance, rank)))
            break;
}