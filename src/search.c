#include "search.h"
#include "board_move.h"
#include "move.h"
#include "movegen.h"
#include "ttable.h"

#include <stdbool.h>

score_t alpha_beta(board_t *board, int depth, score_t alpha, score_t beta);
score_t quiescence_search(board_t *board, score_t alpha, score_t beta);

score_t search(board_t *board, int depth, move_t *best_move)
{
	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	score_t best_score = -VALUE_CHECKMATE;
	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];

		board_make_move(board, move);
		score_t score = -alpha_beta(board, depth - 1, -VALUE_CHECKMATE, VALUE_CHECKMATE);
		board_unmake_move(board);

		if (score > best_score)
		{
			best_score = score;
			if (best_move != NULL)
				*best_move = move;
		}
	}

	return best_score;
}

score_t alpha_beta(board_t *board, int depth, score_t alpha, score_t beta)
{
	if (depth <= 0)
		return quiescence_search(board, alpha, beta);

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	score_t best_score = -VALUE_CHECKMATE;

	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];

		// Check for checkmate
		piece_t captured_piece = board_get(board, move_get_to(move));
		if (captured_piece & KING)
			return VALUE_CHECKMATE;

		// Search deeper
		board_make_move(board, move);
		score_t score = -alpha_beta(board, depth - 1, -beta, -alpha);
		board_unmake_move(board);

		// Update best score if needed
		if (score > best_score)
			best_score = score;

		// Alpha-beta pruning
		if (best_score >= beta)
			break;

		// Search window tightening
		if (best_score > alpha)
			alpha = best_score;
	}

	return best_score;
}

score_t quiescence_search(board_t *board, score_t alpha, score_t beta)
{
	score_t stand_pat = evaluate_relative(board);

	if (stand_pat >= beta)
		return beta;

	if (stand_pat > alpha)
		alpha = stand_pat;

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	bool no_captures = true;
	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];
		piece_t captured_piece = board_get(board, move_get_to(move));

		bool is_capture = captured_piece != PIECE_NONE;
		if (!is_capture)
		{
			no_captures = false;
			continue;
		}

		if (captured_piece & KING)
			return VALUE_CHECKMATE;

		board_make_move(board, move);
		score_t score = -quiescence_search(board, -beta, -alpha);
		board_unmake_move(board);

		if (score >= beta)
			return beta;

		if (score > alpha)
			alpha = score;
	}

	if (no_captures)
		return stand_pat;

	return alpha;
}