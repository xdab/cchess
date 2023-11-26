#include "search.h"
#include "board_move.h"
#include "move.h"
#include "movegen.h"
#include "ttable.h"
#include "board_pieces.h"

#include <stdbool.h>

int nodes;
int qs_nodes;
int ttable_hits;

score_t alpha_beta(board_t *board, int depth, score_t alpha, score_t beta);
score_t quiescence_search(board_t *board, int depth, score_t alpha, score_t beta);

score_t search(board_t *board, int depth, move_t *best_move)
{
	nodes = 0;
	qs_nodes = 0;
	ttable_hits = 0;

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	score_t best_score = -VALUE_CHECKMATE;
	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];
		nodes++;

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
		return quiescence_search(board, 2, alpha, beta);

	ttable_entry_t retrieved_entry;
	ttable_retrieve(board->hash, &retrieved_entry);
	if ((retrieved_entry.hash == board->hash) && (retrieved_entry.depth >= depth))
	{
		ttable_hits++;
		if (retrieved_entry.type == TTABLE_ENTRY_EXACT)
			return retrieved_entry.score;
		else if (retrieved_entry.type == TTABLE_ENTRY_ALPHA && retrieved_entry.score <= alpha)
			return alpha;
		else if (retrieved_entry.type == TTABLE_ENTRY_BETA && retrieved_entry.score >= beta)
			return beta;
	}

	move_t moves[MAX_MOVES];
	int move_count;
	movegen_generate(board, moves, &move_count);

	score_t best_score = -VALUE_CHECKMATE;
	move_t best_move = MOVE_NULL;
	ttable_entry_t tt_entry;

	for (int i = 0; i < move_count; i++)
	{
		move_t move = moves[i];
		nodes++;

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
		{
			tt_entry.hash = board->hash;
			tt_entry.type = TTABLE_ENTRY_BETA;
			tt_entry.best_move = best_move;
			tt_entry.score = best_score;
			tt_entry.depth = depth;
			ttable_store(&tt_entry);

			return best_score;
		}

		// Search window tightening
		if (best_score > alpha)
		{
			tt_entry.hash = board->hash;
			tt_entry.type = TTABLE_ENTRY_ALPHA;
			tt_entry.best_move = best_move;
			tt_entry.score = best_score;
			tt_entry.depth = depth;
			ttable_store(&tt_entry);

			alpha = best_score;
		}
	}

	tt_entry.hash = board->hash;
	tt_entry.type = TTABLE_ENTRY_EXACT;
	tt_entry.best_move = best_move;
	tt_entry.score = best_score;
	tt_entry.depth = depth;
	ttable_store(&tt_entry);

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

		qs_nodes++;

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