#include "movegen_pawn.h"
#include "movegen_common.h"

#define MG_PAWN_CAPTURE (MG_ENEMY)

void _movegen_generate_white_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_black_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);
void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t from);

void movegen_generate_pawn_moves(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	piece_t pawn = board_get(board, square);
	if (pawn & WHITE)
	{
		_movegen_generate_white_pawn_pushes(board, out_moves, out_move_count, square);
		_movegen_generate_white_pawn_captures(board, out_moves, out_move_count, square);
	}
	else if (pawn & BLACK)
	{
		_movegen_generate_black_pawn_pushes(board, out_moves, out_move_count, square);
		_movegen_generate_black_pawn_captures(board, out_moves, out_move_count, square);
	}
}

void _movegen_generate_white_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
	rank_t rank = SQUARE_RANK(square);
	square_t in_front = SQUARE_OF(file, rank + 1);

	if (rank == RANK_2)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			movegen_add(out_moves, out_move_count, move_regular(square, in_front));

			square_t two_in_front = SQUARE_OF(file, RANK_4);
			movegen_add_cond(board, out_moves, out_move_count, move_regular(square, two_in_front), MG_EMPTY);
		}
	}
	else if (rank < RANK_7)
	{
		movegen_add_cond(board, out_moves, out_move_count, move_regular(square, in_front), MG_EMPTY);
	}
	else if (rank == RANK_7)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, QUEEN));
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, ROOK));
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, BISHOP));
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, KNIGHT));
		}
	}
}

void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
	rank_t rank = SQUARE_RANK(square);

	square_t capture_square_1 = SQUARE_OF(file + 1, rank + 1);
	square_t capture_square_2 = SQUARE_OF(file - 1, rank + 1);

	if (rank == RANK_7)
	{
		// Captures with promotion
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, QUEEN), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, ROOK), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, BISHOP), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, KNIGHT), MG_PAWN_CAPTURE);

		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, QUEEN), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, ROOK), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, BISHOP), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, KNIGHT), MG_PAWN_CAPTURE);
	}
	else
	{
		// Regular captures
		movegen_add_cond(board, out_moves, out_move_count, move_regular(square, capture_square_1), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_regular(square, capture_square_2), MG_PAWN_CAPTURE);

		// En passant capture
		if (SQUARE_VALID(board->en_passant_square) && ((board->en_passant_square == capture_square_1) || (board->en_passant_square == capture_square_2)))
			movegen_add_cond(board, out_moves, out_move_count, move_regular(square, board->en_passant_square), MG_EMPTY);
	}
}

void _movegen_generate_black_pawn_pushes(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
	rank_t rank = SQUARE_RANK(square);
	square_t in_front = SQUARE_OF(file, rank - 1);

	if (rank == RANK_7)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			movegen_add(out_moves, out_move_count, move_regular(square, in_front));

			square_t two_in_front = SQUARE_OF(file, RANK_5);
			movegen_add_cond(board, out_moves, out_move_count, move_regular(square, two_in_front), MG_EMPTY);
		}
	}
	else if (rank > RANK_2)
	{
		movegen_add_cond(board, out_moves, out_move_count, move_regular(square, in_front), MG_EMPTY);
	}
	else if (rank == RANK_2)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, QUEEN));
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, ROOK));
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, BISHOP));
			movegen_add(out_moves, out_move_count, move_promotion(square, in_front, KNIGHT));
		}
	}
}

void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
	rank_t rank = SQUARE_RANK(square);

	square_t capture_square_1 = SQUARE_OF(file + 1, rank - 1);
	square_t capture_square_2 = SQUARE_OF(file - 1, rank - 1);

	if (rank == RANK_2)
	{
		// Captures with promotion
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, QUEEN), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, ROOK), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, BISHOP), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_1, KNIGHT), MG_PAWN_CAPTURE);

		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, QUEEN), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, ROOK), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, BISHOP), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_promotion(square, capture_square_2, KNIGHT), MG_PAWN_CAPTURE);
	}
	else
	{
		// Regular captures
		movegen_add_cond(board, out_moves, out_move_count, move_regular(square, capture_square_1), MG_PAWN_CAPTURE);
		movegen_add_cond(board, out_moves, out_move_count, move_regular(square, capture_square_2), MG_PAWN_CAPTURE);

		// En passant capture
		if (SQUARE_VALID(board->en_passant_square) && ((board->en_passant_square == capture_square_1) || (board->en_passant_square == capture_square_2)))
			movegen_add_cond(board, out_moves, out_move_count, move_regular(square, board->en_passant_square), MG_EMPTY);
	}
}