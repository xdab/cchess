#include "movegen_common.h"

#include <stdbool.h>

void movegen_add(move_t *out_moves, int *out_move_count, move_t move)
{
	out_moves[*out_move_count] = move;
	(*out_move_count)++;
}

int movegen_add_cond(const board_t *board, move_t *out_moves, int *out_move_count, move_t move, int condition)
{
	if (move == MOVE_NULL)
		return MG_NOT_ADDED;

	piece_t target = board_get(board, move_get_to(move));
	bool target_empty = (target == PIECE_NONE);

	if ((condition & MG_EMPTY) && target_empty)
	{
		movegen_add(out_moves, out_move_count, move);
		return MG_EMPTY;
	}

	if ((condition & MG_ENEMY) && (!target_empty) && ((target & SIDE_MASK) != board->side_to_move))
	{
		movegen_add(out_moves, out_move_count, move);
		return MG_ENEMY;
	}

	return MG_NOT_ADDED;
}
