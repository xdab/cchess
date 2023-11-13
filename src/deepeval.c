#include "deepeval.h"
#include "movegen.h"

#define LARGE_INT 1000000

centipawns_t _alpha_beta_max(board_t *board, int depth, centipawns_t alpha, centipawns_t beta, move_t *best_move);
centipawns_t _alpha_beta_min(board_t *board, int depth, centipawns_t alpha, centipawns_t beta, move_t *best_move);

centipawns_t deep_eval(board_t *board, int depth, move_t *best_move)
{
	return ((board->side_to_move == SIDE_WHITE)
				? _alpha_beta_max
				: _alpha_beta_min)(board, depth, -LARGE_INT, LARGE_INT, best_move);
}

centipawns_t _alpha_beta_max(board_t *board, int depth, centipawns_t alpha, centipawns_t beta, move_t *best_move)
{
	if (depth == 0)
		return eval(board);

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];
		board_make_move(board, move);
		move_t best_move_child;
		centipawns_t score = _alpha_beta_min(board, depth - 1, alpha, beta, &best_move_child);
		board_unmake_move(board);

		if (score >= beta)
		{
			*best_move = move;
			return beta;
		}
		if (score > alpha)
		{
			*best_move = move;
			alpha = score;
		}
	}

	return alpha;
}

centipawns_t _alpha_beta_min(board_t *board, int depth, centipawns_t alpha, centipawns_t beta, move_t *best_move)
{
	if (depth == 0)
		return eval(board);

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];
		board_make_move(board, move);
		move_t best_move_child;
		centipawns_t score = _alpha_beta_max(board, depth - 1, alpha, beta, &best_move_child);
		board_unmake_move(board);

		if (score <= alpha)
		{
			*best_move = move;
			return alpha;
		}
		if (score < beta)
		{
			*best_move = move;
			beta = score;
		}
	}

	return beta;
}
