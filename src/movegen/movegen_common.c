#include "movegen_common.h"

int movegen_add_if_target_empty_or_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to)
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

void movegen_add_if_target_enemy(const board_t *board, move_t *out_moves, int *out_move_count, square_t from, square_t to)
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