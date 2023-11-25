#include "board_pieces.h"

#include <string.h>

static inline void _board_pieces_insert_at_free_square(square_t *squares, int square_count, square_t value)
{
	for (int i = 0; i < square_count; i++)
		if (squares[i] == SQUARE_NONE)
		{
			squares[i] = value;
			break;
		}
}

static inline void _board_pieces_insert_at_specific_square(square_t *squares, int square_count, square_t prev, square_t new)
{
	for (int i = 0; i < square_count; i++)
		if (squares[i] == prev)
		{
			squares[i] = new;
			break;
		}
}

static inline void _board_pieces_remove_at_specific_square(square_t *squares, int square_count, square_t value)
{
	for (int i = 0; i < square_count; i++)
		if (squares[i] == value)
		{
			squares[i] = SQUARE_NONE;
			break;
		}
}

static inline bool _board_pieces_any(square_t *squares, int square_count)
{
	for (int i = 0; i < square_count; i++)
		if (squares[i] != SQUARE_NONE)
			return true;
	return false;
}

void board_pieces_init(board_pieces_t *pieces, side_t side)
{
	memset(pieces, SQUARE_NONE, sizeof(board_pieces_t));

	if (side & WHITE)
	{
		pieces->pawns[0] = A2;
		pieces->pawns[1] = B2;
		pieces->pawns[2] = C2;
		pieces->pawns[3] = D2;
		pieces->pawns[4] = E2;
		pieces->pawns[5] = F2;
		pieces->pawns[6] = G2;
		pieces->pawns[7] = H2;

		pieces->knights[0] = B1;
		pieces->knights[1] = G1;

		pieces->bishops[0] = C1;
		pieces->bishops[1] = F1;

		pieces->rooks[0] = A1;
		pieces->rooks[1] = H1;

		pieces->queens[0] = D1;

		pieces->king = E1;
	}
	else
	{
		pieces->pawns[0] = A7;
		pieces->pawns[1] = B7;
		pieces->pawns[2] = C7;
		pieces->pawns[3] = D7;
		pieces->pawns[4] = E7;
		pieces->pawns[5] = F7;
		pieces->pawns[6] = G7;
		pieces->pawns[7] = H7;

		pieces->knights[0] = B8;
		pieces->knights[1] = G8;

		pieces->bishops[0] = C8;
		pieces->bishops[1] = F8;

		pieces->rooks[0] = A8;
		pieces->rooks[1] = H8;

		pieces->queens[0] = D8;

		pieces->king = E8;
	}
}

void board_pieces_print(const board_pieces_t *pieces, FILE *stream)
{
	fprintf(stream, "King: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->king)), RANK_SYMBOL(SQUARE_RANK(pieces->king)));

	for (int i = 0; i < MAX_QUEENS; i++)
		if (pieces->queens[i] != SQUARE_NONE)
			fprintf(stream, "Queen: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->queens[i])), RANK_SYMBOL(SQUARE_RANK(pieces->queens[i])));

	for (int i = 0; i < MAX_ROOKS; i++)
		if (pieces->rooks[i] != SQUARE_NONE)
			fprintf(stream, "Rook: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->rooks[i])), RANK_SYMBOL(SQUARE_RANK(pieces->rooks[i])));

	for (int i = 0; i < MAX_BISHOPS; i++)
		if (pieces->bishops[i] != SQUARE_NONE)
			fprintf(stream, "Bishop: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->bishops[i])), RANK_SYMBOL(SQUARE_RANK(pieces->bishops[i])));

	for (int i = 0; i < MAX_KNIGHTS; i++)
		if (pieces->knights[i] != SQUARE_NONE)
			fprintf(stream, "Knight: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->knights[i])), RANK_SYMBOL(SQUARE_RANK(pieces->knights[i])));

	fprintf(stream, "Pawns: ");
	for (int i = 0; i < MAX_PAWNS; i++)
		if (pieces->pawns[i] != SQUARE_NONE)
			fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(pieces->pawns[i])), RANK_SYMBOL(SQUARE_RANK(pieces->pawns[i])));
	fprintf(stream, "\n");
}

void board_pieces_full_update(board_t *board)
{
	memset(&board->white_pieces, SQUARE_NONE, sizeof(board_pieces_t));
	memset(&board->black_pieces, SQUARE_NONE, sizeof(board_pieces_t));

	for (square_t square = SQUARE_MIN; square <= SQUARE_MAX; square++)
	{
		piece_t piece = board->squares[square];
		if (piece == PIECE_NONE)
			continue;

		board_pieces_t *pieces = (piece & BLACK) ? &board->black_pieces : &board->white_pieces;

		switch (piece & PIECE_MASK)
		{
		case PAWN:
			_board_pieces_insert_at_free_square(pieces->pawns, MAX_PAWNS, square);
			break;
		case KNIGHT:
			_board_pieces_insert_at_free_square(pieces->knights, MAX_KNIGHTS, square);
			break;
		case BISHOP:
			_board_pieces_insert_at_free_square(pieces->bishops, MAX_BISHOPS, square);
			break;
		case ROOK:
			_board_pieces_insert_at_free_square(pieces->rooks, MAX_ROOKS, square);
			break;
		case QUEEN:
			_board_pieces_insert_at_free_square(pieces->queens, MAX_QUEENS, square);
			break;
		case KING:
			pieces->king = square;
			break;
		}
	}
}

void board_pieces_move_update(board_t *board, move_t move)
{
	square_t from = move_get_from(move);
	square_t to = move_get_to(move);
	piece_t moved_piece = board_get(board, from);
	piece_t captured_piece = board_get(board, to);
	piece_t promoted_piece = move_get_promoted_piece(move);

	board_pieces_t *player;
	board_pieces_t *opponent;
	if (moved_piece & WHITE)
	{
		player = &board->white_pieces;
		opponent = &board->black_pieces;
	}
	else
	{
		player = &board->black_pieces;
		opponent = &board->white_pieces;
	}

	// Update player's piece to the 'to' square
	switch (moved_piece & PIECE_MASK)
	{
	case PAWN:
		// Find the pawn that moved
		for (int i = 0; i < MAX_PAWNS; i++)
			if (player->pawns[i] == from)
			{
				int target_rank = SQUARE_RANK(to);
				if ((promoted_piece != PIECE_NONE) && (target_rank == RANK_1 || target_rank == RANK_8)) // Check for promotion
				{
					player->pawns[i] = SQUARE_NONE; // Upon promotion the pawn is removed from the board
					switch (promoted_piece)			// Instead, the promoted piece is added to the board at the square that the pawn would have moved to
					{
					case QUEEN:
						_board_pieces_insert_at_free_square(player->queens, MAX_QUEENS, to);
						break;
					case ROOK:
						_board_pieces_insert_at_free_square(player->rooks, MAX_ROOKS, to);
						break;
					case BISHOP:
						_board_pieces_insert_at_free_square(player->bishops, MAX_BISHOPS, to);
						break;
					case KNIGHT:
						_board_pieces_insert_at_free_square(player->knights, MAX_KNIGHTS, to);
						break;
					}
				}
				else // Otherwise, just move the pawn
					player->pawns[i] = to;
				break;
			}
		break;
	case KNIGHT:
		_board_pieces_insert_at_specific_square(player->knights, MAX_KNIGHTS, from, to);
		break;
	case BISHOP:
		_board_pieces_insert_at_specific_square(player->bishops, MAX_BISHOPS, from, to);
		break;
	case ROOK:
		_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, from, to);
		break;
	case QUEEN:
		_board_pieces_insert_at_specific_square(player->queens, MAX_QUEENS, from, to);
		break;
	case KING:
		player->king = to;
		if (move == MOVE_WHITE_OO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, H1, F1);
		else if (move == MOVE_BLACK_OO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, H8, F8);
		else if (move == MOVE_WHITE_OOO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, A1, D1);
		else if (move == MOVE_BLACK_OOO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, A8, D8);
		break;
	}

	// Remove opponent's captured piece from the 'to' square
	if (captured_piece != PIECE_NONE)
	{
		switch (captured_piece & PIECE_MASK)
		{
		case PAWN:
			_board_pieces_remove_at_specific_square(opponent->pawns, MAX_PAWNS, to);
			break;
		case KNIGHT:
			_board_pieces_remove_at_specific_square(opponent->knights, MAX_KNIGHTS, to);
			break;
		case BISHOP:
			_board_pieces_remove_at_specific_square(opponent->bishops, MAX_BISHOPS, to);
			break;
		case ROOK:
			_board_pieces_remove_at_specific_square(opponent->rooks, MAX_ROOKS, to);
			break;
		case QUEEN:
			_board_pieces_remove_at_specific_square(opponent->queens, MAX_QUEENS, to);
			break;
		case KING: // Only happens because this is a "king capture" engine
			opponent->king = SQUARE_NONE;
			break;
		}
	}
}

void board_pieces_unmove_update(board_t *board, move_t move)
{
	square_t from = move_get_from(move);
	square_t to = move_get_to(move);
	piece_t moved_piece = board_get(board, to);
	piece_t captured_piece = board->history[board->history_size].captured_piece;
	piece_t promoted_piece = move_get_promoted_piece(move);

	board_pieces_t *player;
	board_pieces_t *opponent;
	if (moved_piece & WHITE)
	{
		player = &board->white_pieces;
		opponent = &board->black_pieces;
	}
	else
	{
		player = &board->black_pieces;
		opponent = &board->white_pieces;
	}

	if (promoted_piece != PIECE_NONE)
		moved_piece = PAWN | (moved_piece & SIDE_MASK);

	// Restore player's piece to the 'from' square
	switch (moved_piece & PIECE_MASK)
	{
	case PAWN:
		if (promoted_piece == PIECE_NONE) // If the pawn was not promoted, just move it back
			_board_pieces_insert_at_specific_square(player->pawns, MAX_PAWNS, to, from);
		else
		{
			// Otherwise, remove the promoted piece from the board...
			switch (promoted_piece)
			{
			case QUEEN:
				_board_pieces_remove_at_specific_square(player->queens, MAX_QUEENS, to);
				break;
			case ROOK:
				_board_pieces_remove_at_specific_square(player->rooks, MAX_ROOKS, to);
				break;
			case BISHOP:
				_board_pieces_remove_at_specific_square(player->bishops, MAX_BISHOPS, to);
				break;
			case KNIGHT:
				_board_pieces_remove_at_specific_square(player->knights, MAX_KNIGHTS, to);
				break;
			}

			// ...and restore the pawn to the board
			_board_pieces_insert_at_free_square(player->pawns, MAX_PAWNS, from);
		}
		break;
	case KNIGHT:
		_board_pieces_insert_at_specific_square(player->knights, MAX_KNIGHTS, to, from);
		break;
	case BISHOP:
		_board_pieces_insert_at_specific_square(player->bishops, MAX_BISHOPS, to, from);
		break;
	case ROOK:
		_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, to, from);
		break;
	case QUEEN:
		_board_pieces_insert_at_specific_square(player->queens, MAX_QUEENS, to, from);
		break;
	case KING:
		player->king = from;
		if (move == MOVE_WHITE_OO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, F1, H1);
		else if (move == MOVE_BLACK_OO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, F8, H8);
		else if (move == MOVE_WHITE_OOO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, D1, A1);
		else if (move == MOVE_BLACK_OOO)
			_board_pieces_insert_at_specific_square(player->rooks, MAX_ROOKS, D8, A8);
		break;
	}

	// Restore opponent's captured piece to the 'to' square
	if (captured_piece != PIECE_NONE)
	{
		switch (captured_piece & PIECE_MASK)
		{
		case PAWN:
			_board_pieces_insert_at_free_square(opponent->pawns, MAX_PAWNS, to);
			break;
		case KNIGHT:
			_board_pieces_insert_at_free_square(opponent->knights, MAX_KNIGHTS, to);
			break;
		case BISHOP:
			_board_pieces_insert_at_free_square(opponent->bishops, MAX_BISHOPS, to);
			break;
		case ROOK:
			_board_pieces_insert_at_free_square(opponent->rooks, MAX_ROOKS, to);
			break;
		case QUEEN:
			_board_pieces_insert_at_free_square(opponent->queens, MAX_QUEENS, to);
			break;
		case KING: // Explained in board_pieces_move_update()
			opponent->king = to;
			break;
		}
	}
}