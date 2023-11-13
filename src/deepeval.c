#include "deepeval.h"
#include "movegen.h"

#define ALPHA_BETA
#define LARGE_INT 1000000

#ifdef ALPHA_BETA
centipawns_t _alpha_beta_max(const board_t *board, int depth, centipawns_t alpha, centipawns_t beta);
centipawns_t _alpha_beta_min(const board_t *board, int depth, centipawns_t alpha, centipawns_t beta);
#else
centipawns_t _minimax_max(const board_t *board, int depth);
centipawns_t _minimax_min(const board_t *board, int depth);
#endif

centipawns_t deep_eval(const board_t *board, int depth)
{
#ifdef ALPHA_BETA
	if (board->side_to_move == SIDE_WHITE)
		return _alpha_beta_max(board, depth, -LARGE_INT, LARGE_INT);
	else
		return _alpha_beta_min(board, depth, -LARGE_INT, LARGE_INT);
#else
	if (board->side_to_move == SIDE_WHITE)
		return _minimax_max(board, depth);
	else
		return _minimax_min(board, depth);
#endif
}

#ifdef ALPHA_BETA
centipawns_t _alpha_beta_max(const board_t *board, int depth, centipawns_t alpha, centipawns_t beta)
{
	if (depth == 0)
		return eval(board);

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	for (int i = 0; i < move_count; i++)
	{
		board_t new_board;
		board_clone(board, &new_board);
		board_make_move(&new_board, moves[i]);

		centipawns_t score = _alpha_beta_min(&new_board, depth - 1, alpha, beta);

		if (score >= beta)
			return beta;
		if (score > alpha)
			alpha = score;
	}

	return alpha;
}

centipawns_t _alpha_beta_min(const board_t *board, int depth, centipawns_t alpha, centipawns_t beta)
{
	if (depth == 0)
		return eval(board);

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	for (int i = 0; i < move_count; i++)
	{
		board_t new_board;
		board_clone(board, &new_board);
		board_make_move(&new_board, moves[i]);

		centipawns_t score = _alpha_beta_max(&new_board, depth - 1, alpha, beta);

		if (score <= alpha)
			return alpha;
		if (score < beta)
			beta = score;
	}

	return beta;
}
#else
centipawns_t _minimax_max(const board_t *board, int depth)
{
	if (depth == 0)
		return eval(board);

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	centipawns_t best_score = -LARGE_INT;
	for (int i = 0; i < move_count; i++)
	{
		board_t new_board;
		board_clone(board, &new_board);
		board_make_move(&new_board, moves[i]);

		centipawns_t score = _minimax_min(&new_board, depth - 1);

		if (score > best_score)
			best_score = score;
	}

	return best_score;
}

centipawns_t _minimax_min(const board_t *board, int depth)
{
	if (depth == 0)
		return eval(board);

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	centipawns_t best_score = LARGE_INT;
	for (int i = 0; i < move_count; i++)
	{
		board_t new_board;
		board_clone(board, &new_board);
		board_make_move(&new_board, moves[i]);

		centipawns_t score = _minimax_max(&new_board, depth - 1);

		if (score < best_score)
			best_score = score;
	}

	return best_score;
}
#endif
