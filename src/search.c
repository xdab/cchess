#include "search.h"
#include "movegen.h"

score_t alpha_beta(board_t *board, int depth, score_t alpha, score_t beta, move_t *best_move);

score_t search(board_t *board, int depth, move_t *best_move)
{
	int multiplier = (depth % 2 == 0) ? -1 : 1;
	return multiplier * alpha_beta(board, depth, -VALUE_CHECKMATE, VALUE_CHECKMATE, best_move);
}

score_t alpha_beta(board_t *board, int depth, score_t alpha, score_t beta, move_t *best_move)
{
	score_t static_eval = evaluate_relative(board);

	if (depth == 0)
		return static_eval;

	if ((static_eval >= VALUE_CHECKMATE) || (static_eval <= -VALUE_CHECKMATE))
		return static_eval;

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];
		zobrist_t initial_hash = board->hash;
		board_make_move(board, move);
		move_t best_move_child;
		score_t score = -alpha_beta(board, depth - 1, -beta, -alpha, &best_move_child);
		board_unmake_move(board);

		if (board->hash != initial_hash)
		{
			char move_uci[6];
			move_to_uci(move, move_uci);
			fprintf(stderr, "Hash mismatch after move %s\n", move_uci);
		}

		if (score >= beta)
		{
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
