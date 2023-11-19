#include "board_move.h"

#include <string.h>

void board_make_move(board_t *board, move_t move)
{
	square_t from = move_get_from(move);
	piece_t moved_piece = board_get(board, from);
	piece_t promoted_piece = move_get_promoted_piece(move);

	square_t to = move_get_to(move);
	piece_t captured_piece = board_get(board, to);

	// Update piece positions
	piecepos_update(&board->white_piece_positions, &board->black_piece_positions, from, to, moved_piece, promoted_piece, captured_piece);

	// Castling
	if (moved_piece & KING)
	{
		if (moved_piece & WHITE)
		{
			board->white_castling_rights = CASTLING_RIGHTS_NONE;
			if (from == E1)
			{
				if (to == G1)
				{
					board_set(board, H1, PIECE_NONE);
					board_set(board, F1, WHITE | ROOK);
				}
				else if (to == C1)
				{
					board_set(board, A1, PIECE_NONE);
					board_set(board, D1, WHITE | ROOK);
				}
			}
		}
		else if (moved_piece & BLACK)
		{
			board->black_castling_rights = CASTLING_RIGHTS_NONE;
			if (from == E8)
			{
				if (to == G8)
				{
					board_set(board, H8, PIECE_NONE);
					board_set(board, F8, BLACK | ROOK);
				}
				else if (to == C8)
				{
					board_set(board, A8, PIECE_NONE);
					board_set(board, D8, BLACK | ROOK);
				}
			}
		}
	}
	else if (moved_piece & ROOK)
	{
		if (moved_piece & WHITE)
		{
			if (from == A1)
				board->white_castling_rights &= ~CASTLING_RIGHTS_QUEENSIDE;
			else if (from == H1)
				board->white_castling_rights &= ~CASTLING_RIGHTS_KINGSIDE;
		}
		else if (moved_piece & BLACK)
		{
			if (from == A8)
				board->black_castling_rights &= ~CASTLING_RIGHTS_QUEENSIDE;
			else if (from == H8)
				board->black_castling_rights &= ~CASTLING_RIGHTS_KINGSIDE;
		}
	}

	board_set(board, from, PIECE_NONE);
	board_set(board, to, (promoted_piece != PIECE_NONE) ? (promoted_piece | (moved_piece & SIDE_MASK)) : moved_piece);

	// Store event in history
	if (board->history_size == BOARD_HISTORY_SIZE)
	{
		for (int i = 0; i < BOARD_HISTORY_SIZE - 1; i++)
			board->history[i] = board->history[i + 1];
		board->history_size--;
	}

	// TODO remove
	memcpy(&board->history[board->history_size].white_piece_positions, &board->white_piece_positions, sizeof(piecepos_t));
	memcpy(&board->history[board->history_size].black_piece_positions, &board->black_piece_positions, sizeof(piecepos_t));

	board->history[board->history_size].move = move;
	board->history[board->history_size].captured_piece = captured_piece;
	board->history[board->history_size].white_castling_rights = board->white_castling_rights;
	board->history[board->history_size].black_castling_rights = board->black_castling_rights;
	board->history[board->history_size].en_passant_square = board->en_passant_square;
	board->history[board->history_size].halfmove_clock = board->halfmove_clock;
	board->history_size++;

	board->side_to_move = (moved_piece & WHITE) ? BLACK : WHITE;
	board->halfmove_clock = ((captured_piece != PIECE_NONE) || (moved_piece & PAWN)) ? 0 : (board->halfmove_clock + 1);
	board->fullmove_number += (board->side_to_move == BLACK);
}

void board_unmake_move(board_t *board)
{
	if (board->history_size == 0)
		return;

	board->history_size--;
	move_t move = board->history[board->history_size].move;

	square_t from = move_get_from(move);
	square_t to = move_get_to(move);

	piece_t moved_piece = board_get(board, to);
	piece_t captured_piece = board->history[board->history_size].captured_piece;

	piece_t promoted_piece = move_get_promoted_piece(move);
	if (promoted_piece != PIECE_NONE)
		moved_piece = PAWN | (moved_piece & SIDE_MASK);

	// Castling
	if (moved_piece & KING)
	{
		if (moved_piece & WHITE)
		{
			if (from == E1)
			{
				if (to == G1)
				{
					board_set(board, H1, WHITE | ROOK);
					board_set(board, F1, PIECE_NONE);
				}
				else if (to == C1)
				{
					board_set(board, A1, WHITE | ROOK);
					board_set(board, D1, PIECE_NONE);
				}
			}
		}
		else if (moved_piece & BLACK)
		{
			if (from == E8)
			{
				if (to == G8)
				{
					board_set(board, H8, BLACK | ROOK);
					board_set(board, F8, PIECE_NONE);
				}
				else if (to == C8)
				{
					board_set(board, A8, BLACK | ROOK);
					board_set(board, D8, PIECE_NONE);
				}
			}
		}
	}

	board_set(board, from, moved_piece);
	board_set(board, to, captured_piece);

	// TODO remove
	memcpy(&board->white_piece_positions, &board->history[board->history_size].white_piece_positions, sizeof(piecepos_t));
	memcpy(&board->black_piece_positions, &board->history[board->history_size].black_piece_positions, sizeof(piecepos_t));

	board->side_to_move = (moved_piece & WHITE) ? WHITE : BLACK;
	board->white_castling_rights = board->history[board->history_size].white_castling_rights;
	board->black_castling_rights = board->history[board->history_size].black_castling_rights;
	board->en_passant_square = board->history[board->history_size].en_passant_square;
	board->halfmove_clock = board->history[board->history_size].halfmove_clock;
	board->fullmove_number -= (board->side_to_move == BLACK);
}
