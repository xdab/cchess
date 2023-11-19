#include "movegen_queen.h"
#include "movegen_rook.h"
#include "movegen_bishop.h"

void movegen_generate_queen_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	movegen_generate_rook_moves(board, out_moves, out_move_count, square);
	movegen_generate_bishop_moves(board, out_moves, out_move_count, square);
}