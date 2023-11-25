#include "search.h"
#include "board_move.h"
#include "move.h"
#include "movegen.h"
#include "ttable.h"
#include "board_pieces.h"

#include <stdbool.h>

score_t alpha_beta(board_t *board, int depth, score_t alpha, score_t beta);
score_t quiescence_search(board_t *board, int depth, score_t alpha, score_t beta);

score_t search(board_t *board, int depth, move_t *best_move)
{
	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	score_t best_score = -VALUE_CHECKMATE;
	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];

		board_move(board, move);
		score_t score = -alpha_beta(board, depth - 1, -VALUE_CHECKMATE, VALUE_CHECKMATE);
		board_unmove(board);

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
		return quiescence_search(board, 4, alpha, beta);

	ttable_entry_t retrieved_entry;
	ttable_retrieve(board->hash, &retrieved_entry);
	if ((retrieved_entry.hash == board->hash) && (retrieved_entry.depth >= depth))
	{
		if (retrieved_entry.type == TTABLE_ENTRY_EXACT)
			return retrieved_entry.score;
	}

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	score_t best_score = -VALUE_CHECKMATE;
	move_t best_move = MOVE_NULL;

	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];

		// Check for checkmate
		piece_t captured_piece = board_get(board, move_get_to(move));
		if (captured_piece & KING)
			return VALUE_CHECKMATE;

		// Search deeper
		board_move(board, move);
		score_t score = -alpha_beta(board, depth - 1, -beta, -alpha);
		board_unmove(board);

		// Update best score if needed
		if (score > best_score)
		{
			best_score = score;
			best_move = move;
		}

		// Alpha-beta pruning
		if (best_score >= beta)
			break;

		// Search window tightening
		if (best_score > alpha)
			alpha = best_score;
	}

	ttable_entry_t entry_to_store;
	entry_to_store.hash = board->hash;
	entry_to_store.type = TTABLE_ENTRY_EXACT;
	entry_to_store.best_move = best_move;
	entry_to_store.score = best_score;
	entry_to_store.depth = depth;
	ttable_store(&entry_to_store);

	return best_score;
}

score_t quiescence_search(board_t *board, int depth, score_t alpha, score_t beta)
{
	score_t stand_pat = evaluate_relative(board);

	if (depth <= 0)
		return stand_pat;

	if (stand_pat >= beta)
		return beta;

	if (alpha < stand_pat)
		alpha = stand_pat;

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];
		piece_t captured_piece = board_get(board, move_get_to(move));

		bool is_capture = captured_piece != PIECE_NONE;
		if (!is_capture)
			continue;

		if (captured_piece & KING)
			return VALUE_CHECKMATE;

		board_move(board, move);
		score_t score = -quiescence_search(board, depth - 1, -beta, -alpha);
		board_unmove(board);

		if (score >= beta)
			return beta;

		if (score > alpha)
			alpha = score;
	}

	return alpha;
}