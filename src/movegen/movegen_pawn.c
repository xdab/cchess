#include "movegen_pawn.h"
#include "movegen_common.h"

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
			// Push 1
			out_moves[*out_move_count] = move_regular(square, in_front);
			(*out_move_count)++;

			square_t two_in_front = SQUARE_OF(file, RANK_4);
			if (board_get(board, two_in_front) == PIECE_NONE)
			{
				// Push 2
				out_moves[*out_move_count] = move_regular(square, two_in_front);
				(*out_move_count)++;
			}
		}
	}
	else if (rank < RANK_7)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			// Push 1
			out_moves[*out_move_count] = move_regular(square, in_front);
			(*out_move_count)++;
		}
	}
	else if (rank == RANK_7)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			// Push 1 and promote to queen
			out_moves[*out_move_count] = move_promotion(square, in_front, QUEEN);
			(*out_move_count)++;

			// ... and promote to rook
			out_moves[*out_move_count] = move_promotion(square, in_front, ROOK);
			(*out_move_count)++;

			// ... and promote to bishop
			out_moves[*out_move_count] = move_promotion(square, in_front, BISHOP);
			(*out_move_count)++;

			// ... and promote to knight
			out_moves[*out_move_count] = move_promotion(square, in_front, KNIGHT);
			(*out_move_count)++;
		}
	}
}

void _movegen_generate_white_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
	rank_t rank = SQUARE_RANK(square);
	int target_rank = rank + 1;

	// Regular captures
	movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, target_rank));
	movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, target_rank));

	// En passant capture
	int en_passant_rank = SQUARE_RANK(board->en_passant_square);
	int en_passant_file = SQUARE_FILE(board->en_passant_square);
	if (en_passant_rank == target_rank && (en_passant_file == file + 1 || en_passant_file == file - 1))
	{
		out_moves[*out_move_count] = move_regular(square, board->en_passant_square);
		(*out_move_count)++;
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
			// Push 1
			out_moves[*out_move_count] = move_regular(square, in_front);
			(*out_move_count)++;

			square_t two_in_front = SQUARE_OF(file, RANK_5);
			if (board_get(board, two_in_front) == PIECE_NONE)
			{
				// Push 2
				out_moves[*out_move_count] = move_regular(square, two_in_front);
				(*out_move_count)++;
			}
		}
	}
	else if (rank > RANK_2)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			// Push 1
			out_moves[*out_move_count] = move_regular(square, in_front);
			(*out_move_count)++;
		}
	}
	else if (rank == RANK_2)
	{
		if (board_get(board, in_front) == PIECE_NONE)
		{
			// Push 1 and promote to queen
			out_moves[*out_move_count] = move_promotion(square, in_front, QUEEN);
			(*out_move_count)++;

			// ... and promote to rook
			out_moves[*out_move_count] = move_promotion(square, in_front, ROOK);
			(*out_move_count)++;

			// ... and promote to bishop
			out_moves[*out_move_count] = move_promotion(square, in_front, BISHOP);
			(*out_move_count)++;

			// ... and promote to knight
			out_moves[*out_move_count] = move_promotion(square, in_front, KNIGHT);
			(*out_move_count)++;
		}
	}
}

void _movegen_generate_black_pawn_captures(const board_t *board, move_t *out_moves, int *out_move_count, square_t square)
{
	file_t file = SQUARE_FILE(square);
	rank_t rank = SQUARE_RANK(square);
	int target_rank = rank - 1;

	// Regular captures
	movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file + 1, target_rank));
	movegen_add_if_target_enemy(board, out_moves, out_move_count, square, SQUARE_OF(file - 1, target_rank));

	// En passant capture
	int en_passant_rank = SQUARE_RANK(board->en_passant_square);
	int en_passant_file = SQUARE_FILE(board->en_passant_square);
	if (en_passant_rank == target_rank && (en_passant_file == file + 1 || en_passant_file == file - 1))
	{
		out_moves[*out_move_count] = move_regular(square, board->en_passant_square);
		(*out_move_count)++;
	}
}