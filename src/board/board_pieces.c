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
	if (side & WHITE)
	{
		pieces->kings_rook = H1;
		pieces->kings_knight = G1;
		pieces->kings_bishop = F1;
		pieces->king = E1;
		pieces->queen = D1;
		pieces->queens_bishop = C1;
		pieces->queens_knight = B1;
		pieces->queens_rook = A1;

		pieces->has_pawns = true;
		pieces->pawns[0] = A2;
		pieces->pawns[1] = B2;
		pieces->pawns[2] = C2;
		pieces->pawns[3] = D2;
		pieces->pawns[4] = E2;
		pieces->pawns[5] = F2;
		pieces->pawns[6] = G2;
		pieces->pawns[7] = H2;
	}
	else
	{
		pieces->kings_rook = H8;
		pieces->kings_knight = G8;
		pieces->kings_bishop = F8;
		pieces->king = E8;
		pieces->queen = D8;
		pieces->queens_bishop = C8;
		pieces->queens_knight = B8;
		pieces->queens_rook = A8;

		pieces->has_pawns = true;
		pieces->pawns[0] = A7;
		pieces->pawns[1] = B7;
		pieces->pawns[2] = C7;
		pieces->pawns[3] = D7;
		pieces->pawns[4] = E7;
		pieces->pawns[5] = F7;
		pieces->pawns[6] = G7;
		pieces->pawns[7] = H7;
	}

	pieces->has_promoted_pieces = false;
	memset(pieces->promoted_queens, SQUARE_NONE, sizeof(pieces->promoted_queens));
	memset(pieces->promoted_rooks, SQUARE_NONE, sizeof(pieces->promoted_rooks));
	memset(pieces->promoted_bishops, SQUARE_NONE, sizeof(pieces->promoted_bishops));
	memset(pieces->promoted_knights, SQUARE_NONE, sizeof(pieces->promoted_knights));
	pieces->has_promoted_queens = false;
	pieces->has_promoted_rooks = false;
	pieces->has_promoted_bishops = false;
	pieces->has_promoted_knights = false;
}

void board_pieces_print(const board_pieces_t *pieces, FILE *stream)
{
	fprintf(stream, "King: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->king)), RANK_SYMBOL(SQUARE_RANK(pieces->king)));

	if (pieces->queen != SQUARE_NONE)
		fprintf(stream, "Queen: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->queen)), RANK_SYMBOL(SQUARE_RANK(pieces->queen)));

	if (pieces->kings_rook != SQUARE_NONE)
		fprintf(stream, "Kings rook: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->kings_rook)), RANK_SYMBOL(SQUARE_RANK(pieces->kings_rook)));

	if (pieces->queens_rook != SQUARE_NONE)
		fprintf(stream, "Queens rook: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->queens_rook)), RANK_SYMBOL(SQUARE_RANK(pieces->queens_rook)));

	if (pieces->kings_knight != SQUARE_NONE)
		fprintf(stream, "Kings knight: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->kings_knight)), RANK_SYMBOL(SQUARE_RANK(pieces->kings_knight)));

	if (pieces->queens_knight != SQUARE_NONE)
		fprintf(stream, "Queens knight: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->queens_knight)), RANK_SYMBOL(SQUARE_RANK(pieces->queens_knight)));

	if (pieces->kings_bishop != SQUARE_NONE)
		fprintf(stream, "Kings bishop: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->kings_bishop)), RANK_SYMBOL(SQUARE_RANK(pieces->kings_bishop)));

	if (pieces->queens_bishop != SQUARE_NONE)
		fprintf(stream, "Queens bishop: %c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->queens_bishop)), RANK_SYMBOL(SQUARE_RANK(pieces->queens_bishop)));

	if (pieces->has_pawns)
	{
		fprintf(stream, "Pawns: ");
		for (int i = 0; i < 8; i++)
			if (pieces->pawns[i] != SQUARE_NONE)
				fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(pieces->pawns[i])), RANK_SYMBOL(SQUARE_RANK(pieces->pawns[i])));
		fputc('\n', stream);
	}

	if (pieces->has_promoted_pieces)
	{
		fprintf(stream, "Promoted pieces:\n");
		if (pieces->has_promoted_queens)
		{
			fprintf(stream, "Queens: ");
			for (int i = 0; i < 8; i++)
				if (pieces->promoted_queens[i] != SQUARE_NONE)
					fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(pieces->promoted_queens[i])), RANK_SYMBOL(SQUARE_RANK(pieces->promoted_queens[i])));
			fputc('\n', stream);
		}

		if (pieces->has_promoted_rooks)
		{
			fprintf(stream, "Rooks: ");
			for (int i = 0; i < 8; i++)
				if (pieces->promoted_rooks[i] != SQUARE_NONE)
					fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(pieces->promoted_rooks[i])), RANK_SYMBOL(SQUARE_RANK(pieces->promoted_rooks[i])));
			fputc('\n', stream);
		}

		if (pieces->has_promoted_bishops)
		{
			fprintf(stream, "Bishops: ");
			for (int i = 0; i < 8; i++)
				if (pieces->promoted_bishops[i] != SQUARE_NONE)
					fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(pieces->promoted_bishops[i])), RANK_SYMBOL(SQUARE_RANK(pieces->promoted_bishops[i])));
			fputc('\n', stream);
		}

		if (pieces->has_promoted_knights)
		{
			fprintf(stream, "Knights: ");
			for (int i = 0; i < 8; i++)
				if (pieces->promoted_knights[i] != SQUARE_NONE)
					fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(pieces->promoted_knights[i])), RANK_SYMBOL(SQUARE_RANK(pieces->promoted_knights[i])));
			fputc('\n', stream);
		}
	}
}

void board_pieces_full_update(board_t *board)
{
	memset(&board->white_pieces, SQUARE_NONE, sizeof(board_pieces_t));
	memset(&board->black_pieces, SQUARE_NONE, sizeof(board_pieces_t));
	board->white_pieces.has_pawns = board->black_pieces.has_pawns = false;
	board->white_pieces.has_promoted_pieces = board->black_pieces.has_promoted_pieces = false;
	board->white_pieces.has_promoted_queens = board->black_pieces.has_promoted_queens = false;
	board->white_pieces.has_promoted_rooks = board->black_pieces.has_promoted_rooks = false;
	board->white_pieces.has_promoted_bishops = board->black_pieces.has_promoted_bishops = false;
	board->white_pieces.has_promoted_knights = board->black_pieces.has_promoted_knights = false;

	for (square_t square = SQUARE_MIN; square <= SQUARE_MAX; square++)
	{
		piece_t piece = board->squares[square];

		if (piece == PIECE_NONE)
			continue;

		board_pieces_t *pieces = (piece & BLACK) ? &board->black_pieces : &board->white_pieces;

		if (piece & KING)
			pieces->king = square;

		else if (piece & PAWN)
		{
			pieces->has_pawns = true;
			for (int i = 0; i < 8; i++)
				if (pieces->pawns[i] == SQUARE_NONE)
				{
					pieces->pawns[i] = square;
					break;
				}
		}

		else if (piece & KNIGHT)
		{
			if (pieces->kings_knight == SQUARE_NONE)
				pieces->kings_knight = square;
			else if (pieces->queens_knight == SQUARE_NONE)
				pieces->queens_knight = square;
			else
			{
				pieces->has_promoted_pieces = true;
				pieces->has_promoted_knights = true;
				for (int i = 0; i < 8; i++)
					if (pieces->promoted_knights[i] == SQUARE_NONE)
					{
						pieces->promoted_knights[i] = square;
						break;
					}
			}
		}

		else if (piece & BISHOP)
		{
			bool is_dark_square = SQUARE_DARK(square);
			bool is_light_square = SQUARE_LIGHT(square);

			if (is_light_square && (pieces->kings_bishop == SQUARE_NONE))
				pieces->kings_bishop = square;
			else if (is_dark_square && (pieces->queens_bishop == SQUARE_NONE))
				pieces->queens_bishop = square;
			else
			{
				pieces->has_promoted_pieces = true;
				pieces->has_promoted_bishops = true;
				for (int i = 0; i < 8; i++)
					if (pieces->promoted_bishops[i] == SQUARE_NONE)
					{
						pieces->promoted_bishops[i] = square;
						break;
					}
			}
		}

		else if (piece & ROOK)
		{
			if (pieces->kings_rook == SQUARE_NONE)
				pieces->kings_rook = square;
			else if (pieces->queens_rook == SQUARE_NONE)
				pieces->queens_rook = square;
			else
			{
				pieces->has_promoted_pieces = true;
				pieces->has_promoted_rooks = true;
				for (int i = 0; i < 8; i++)
					if (pieces->promoted_rooks[i] == SQUARE_NONE)
					{
						pieces->promoted_rooks[i] = square;
						break;
					}
			}
		}

		else if (piece & QUEEN)
		{
			if (pieces->queen == SQUARE_NONE)
				pieces->queen = square;
			else
			{
				pieces->has_promoted_pieces = true;
				pieces->has_promoted_queens = true;
				for (int i = 0; i < 8; i++)
					if (pieces->promoted_queens[i] == SQUARE_NONE)
					{
						pieces->promoted_queens[i] = square;
						break;
					}
			}
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
					char uci[6];
					player->has_promoted_pieces = true;
					player->pawns[i] = SQUARE_NONE; // Upon promotion the pawn is removed from the board
					switch (promoted_piece)			// Instead, the promoted piece is added to the board at the square that the pawn would have moved to
					{
					case QUEEN:
						player->has_promoted_queens = true;
						_board_pieces_insert_at_free_square(player->promoted_queens, MAX_PROMOTED_PIECES, to);
						break;
					case ROOK:
						player->has_promoted_rooks = true;
						_board_pieces_insert_at_free_square(player->promoted_rooks, MAX_PROMOTED_PIECES, to);
						break;
					case BISHOP:
						player->has_promoted_bishops = true;
						_board_pieces_insert_at_free_square(player->promoted_bishops, MAX_PROMOTED_PIECES, to);
						break;
					case KNIGHT:
						player->has_promoted_knights = true;
						_board_pieces_insert_at_free_square(player->promoted_knights, MAX_PROMOTED_PIECES, to);
						break;
					}
				}
				else // Otherwise, just move the pawn
					player->pawns[i] = to;
				break;
			}
		break;
	case KNIGHT:
		if (player->kings_knight == from)
			player->kings_knight = to;
		else if (player->queens_knight == from)
			player->queens_knight = to;
		else
			_board_pieces_insert_at_specific_square(player->promoted_knights, MAX_PROMOTED_PIECES, from, to);
		break;
	case BISHOP:
		if (player->kings_bishop == from)
			player->kings_bishop = to;
		else if (player->queens_bishop == from)
			player->queens_bishop = to;
		else
			_board_pieces_insert_at_specific_square(player->promoted_bishops, MAX_PROMOTED_PIECES, from, to);
		break;
	case ROOK:
		if (player->kings_rook == from)
			player->kings_rook = to;
		else if (player->queens_rook == from)
			player->queens_rook = to;
		else
			_board_pieces_insert_at_specific_square(player->promoted_rooks, MAX_PROMOTED_PIECES, from, to);
		break;
	case QUEEN:
		if (player->queen == from)
			player->queen = to;
		else
			_board_pieces_insert_at_specific_square(player->promoted_queens, MAX_PROMOTED_PIECES, from, to);
		break;
	case KING:
		player->king = to;
		if (move == MOVE_WHITE_OO)
			player->kings_rook = F1;
		else if (move == MOVE_BLACK_OO)
			player->kings_rook = F8;
		else if (move == MOVE_WHITE_OOO)
			player->queens_rook = D1;
		else if (move == MOVE_BLACK_OOO)
			player->queens_rook = D8;
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
			if (opponent->kings_knight == to)
				opponent->kings_knight = SQUARE_NONE;
			else if (opponent->queens_knight == to)
				opponent->queens_knight = SQUARE_NONE;
			else
				_board_pieces_remove_at_specific_square(opponent->promoted_knights, MAX_PROMOTED_PIECES, to);
			break;
		case BISHOP:
			if (opponent->kings_bishop == to)
				opponent->kings_bishop = SQUARE_NONE;
			else if (opponent->queens_bishop == to)
				opponent->queens_bishop = SQUARE_NONE;
			else
				_board_pieces_remove_at_specific_square(opponent->promoted_bishops, MAX_PROMOTED_PIECES, to);
			break;
		case ROOK:
			if (opponent->kings_rook == to)
				opponent->kings_rook = SQUARE_NONE;
			else if (opponent->queens_rook == to)
				opponent->queens_rook = SQUARE_NONE;
			else
				_board_pieces_remove_at_specific_square(opponent->promoted_rooks, MAX_PROMOTED_PIECES, to);
			break;
		case QUEEN:
			if (opponent->queen == to)
				opponent->queen = SQUARE_NONE;
			else
				_board_pieces_remove_at_specific_square(opponent->promoted_queens, MAX_PROMOTED_PIECES, to);
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
				_board_pieces_remove_at_specific_square(player->promoted_queens, MAX_PROMOTED_PIECES, to);
				if (!_board_pieces_any(player->promoted_queens, MAX_PROMOTED_PIECES))
					player->has_promoted_queens = false;
				break;
			case ROOK:
				_board_pieces_remove_at_specific_square(player->promoted_rooks, MAX_PROMOTED_PIECES, to);
				if (!_board_pieces_any(player->promoted_rooks, MAX_PROMOTED_PIECES))
					player->has_promoted_rooks = false;
				break;
			case BISHOP:
				_board_pieces_remove_at_specific_square(player->promoted_bishops, MAX_PROMOTED_PIECES, to);
				if (!_board_pieces_any(player->promoted_bishops, MAX_PROMOTED_PIECES))
					player->has_promoted_bishops = false;
				break;
			case KNIGHT:
				_board_pieces_remove_at_specific_square(player->promoted_knights, MAX_PROMOTED_PIECES, to);
				if (!_board_pieces_any(player->promoted_knights, MAX_PROMOTED_PIECES))
					player->has_promoted_knights = false;
				break;
			}
			player->has_promoted_pieces = player->has_promoted_queens || player->has_promoted_rooks || player->has_promoted_bishops || player->has_promoted_knights;

			// ...and restore the pawn to the board
			_board_pieces_insert_at_free_square(player->pawns, MAX_PAWNS, from);
		}
		break;
	case KNIGHT:
		if (player->kings_knight == to)
			player->kings_knight = from;
		else if (player->queens_knight == to)
			player->queens_knight = from;
		else
			_board_pieces_insert_at_specific_square(player->promoted_knights, MAX_PROMOTED_PIECES, to, from);
		break;
	case BISHOP:
		if (player->kings_bishop == to)
			player->kings_bishop = from;
		else if (player->queens_bishop == to)
			player->queens_bishop = from;
		else
			_board_pieces_insert_at_specific_square(player->promoted_bishops, MAX_PROMOTED_PIECES, to, from);
		break;
	case ROOK:
		if (player->kings_rook == to)
			player->kings_rook = from;
		else if (player->queens_rook == to)
			player->queens_rook = from;
		else
			_board_pieces_insert_at_specific_square(player->promoted_rooks, MAX_PROMOTED_PIECES, to, from);
		break;
	case QUEEN:
		if (player->queen == to)
			player->queen = from;
		else
			_board_pieces_insert_at_specific_square(player->promoted_queens, MAX_PROMOTED_PIECES, to, from);
		break;
	case KING:
		player->king = from;
		if (move == MOVE_WHITE_OO)
			player->kings_rook = H1;
		else if (move == MOVE_BLACK_OO)
			player->kings_rook = H8;
		else if (move == MOVE_WHITE_OOO)
			player->queens_rook = A1;
		else if (move == MOVE_BLACK_OOO)
			player->queens_rook = A8;
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
			// Unfortunatelly, the knights are indistinguishable at this level
			if (opponent->kings_knight == SQUARE_NONE)
				opponent->kings_knight = to;
			else if (opponent->queens_knight == SQUARE_NONE)
				opponent->queens_knight = to;
			else
			{
				_board_pieces_insert_at_free_square(opponent->promoted_knights, MAX_PROMOTED_PIECES, to);
				opponent->has_promoted_knights = true;
			}
			break;
		case BISHOP:
			if (opponent->kings_bishop == SQUARE_NONE)
				opponent->kings_bishop = to;
			else if (opponent->queens_bishop == SQUARE_NONE)
				opponent->queens_bishop = to;
			else
			{
				_board_pieces_insert_at_free_square(opponent->promoted_bishops, MAX_PROMOTED_PIECES, to);
				opponent->has_promoted_bishops = true;
			}
			break;
		case ROOK:
			// Unfortunatelly, the rooks are indistinguishable at this level
			if (opponent->kings_rook == SQUARE_NONE)
				opponent->kings_rook = to;
			else if (opponent->queens_rook == SQUARE_NONE)
				opponent->queens_rook = to;
			else
			{
				_board_pieces_insert_at_free_square(opponent->promoted_rooks, MAX_PROMOTED_PIECES, to);
				opponent->has_promoted_rooks = true;
			}
			break;
		case QUEEN:
			if (opponent->queen == SQUARE_NONE)
				opponent->queen = to;
			else
			{
				_board_pieces_insert_at_free_square(opponent->promoted_queens, MAX_PROMOTED_PIECES, to);
				opponent->has_promoted_queens = true;
			}
			break;
		case KING: // Explained in board_pieces_move_update()
			opponent->king = to;
			break;
		}
		opponent->has_promoted_pieces = opponent->has_promoted_queens || opponent->has_promoted_rooks || opponent->has_promoted_bishops || opponent->has_promoted_knights;
	}
}