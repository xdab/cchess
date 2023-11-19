#include "movegen_knight.h"
#include "movegen_common.h"

#define MG_COND_KNIGHT (MG_EMPTY | MG_ENEMY)

void movegen_generate_knight_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
	rank_t rank = SQUARE_RANK(square);

	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file + 1, rank + 2)), MG_COND_KNIGHT);
	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file + 2, rank + 1)), MG_COND_KNIGHT);
	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file + 2, rank - 1)), MG_COND_KNIGHT);
	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file + 1, rank - 2)), MG_COND_KNIGHT);
	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file - 1, rank - 2)), MG_COND_KNIGHT);
	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file - 2, rank - 1)), MG_COND_KNIGHT);
	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file - 2, rank + 1)), MG_COND_KNIGHT);
	movegen_add_cond(board, out_moves, out_move_count, move_regular(square, SQUARE_OF(file - 1, rank + 2)), MG_COND_KNIGHT);
}
