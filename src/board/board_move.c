#include "board_move.h"
#include "board_pieces.h"

#include <string.h>

void board_move(board_t *board, move_t move)
{
	square_t from = move_get_from(move);
	piece_t moved_piece = board_get(board, from);
	piece_t promoted_piece = move_get_promoted_piece(move);

	square_t to = move_get_to(move);
	piece_t captured_piece = board_get(board, to);

	// Make sure the history array doesn't get too big
	if (board->history_size == BOARD_HISTORY_SIZE)
	{
		memmove(&board->history[0], &board->history[1], (BOARD_HISTORY_SIZE - 1) * sizeof(board_event_t));
		board->history_size--;
	}

	board->history[board->history_size].move = move;
	board->history[board->history_size].captured_piece = captured_piece;
	board->history[board->history_size].white_castling_rights = board->white_castling_rights;
	board->history[board->history_size].black_castling_rights = board->black_castling_rights;
	board->history[board->history_size].en_passant_square = board->en_passant_square;
	board->history[board->history_size].halfmove_clock = board->halfmove_clock;
	board->history_size++;

	board_pieces_move_update(board, move);

	if (moved_piece & KING)
	{
		if (move == MOVE_WHITE_OO)
		{
			board_set(board, H1, PIECE_NONE);
			board_set(board, F1, WHITE | ROOK);
			board->white_castling_rights = CASTLING_RIGHTS_NONE;
		}
		else if (move == MOVE_BLACK_OO)
		{
			board_set(board, H8, PIECE_NONE);
			board_set(board, F8, BLACK | ROOK);
			board->black_castling_rights = CASTLING_RIGHTS_NONE;
		}
		else if (move == MOVE_WHITE_OOO)
		{
			board_set(board, A1, PIECE_NONE);
			board_set(board, D1, WHITE | ROOK);
			board->white_castling_rights = CASTLING_RIGHTS_NONE;
		}
		else if (move == MOVE_BLACK_OOO)
		{
			board_set(board, A8, PIECE_NONE);
			board_set(board, D8, BLACK | ROOK);
			board->black_castling_rights = CASTLING_RIGHTS_NONE;
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

	board->side_to_move = (moved_piece & WHITE) ? BLACK : WHITE;
	board->halfmove_clock = ((captured_piece != PIECE_NONE) || (moved_piece & PAWN)) ? 0 : (board->halfmove_clock + 1);
	board->fullmove_number += (board->side_to_move == BLACK);
}

void board_unmove(board_t *board)
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

	board_pieces_unmove_update(board, move);

	if (promoted_piece != PIECE_NONE)
		moved_piece = PAWN | (moved_piece & SIDE_MASK);

	if (moved_piece & KING)
	{
		if (move == MOVE_WHITE_OO)
		{
			board_set(board, H1, WHITE | ROOK);
			board_set(board, F1, PIECE_NONE);
		}
		else if (move == MOVE_BLACK_OO)
		{
			board_set(board, H8, BLACK | ROOK);
			board_set(board, F8, PIECE_NONE);
		}
		else if (move == MOVE_WHITE_OOO)
		{
			board_set(board, A1, WHITE | ROOK);
			board_set(board, D1, PIECE_NONE);
		}
		else if (move == MOVE_BLACK_OOO)
		{
			board_set(board, A8, BLACK | ROOK);
			board_set(board, D8, PIECE_NONE);
		}
	}

	board_set(board, from, moved_piece);
	board_set(board, to, captured_piece);

	board->side_to_move = (moved_piece & WHITE) ? WHITE : BLACK;
	board->white_castling_rights = board->history[board->history_size].white_castling_rights;
	board->black_castling_rights = board->history[board->history_size].black_castling_rights;
	board->en_passant_square = board->history[board->history_size].en_passant_square;
	board->halfmove_clock = board->history[board->history_size].halfmove_clock;
	board->fullmove_number -= (board->side_to_move == BLACK);
}
