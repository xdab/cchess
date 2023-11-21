#include "board_pieces.h"

#include <string.h>

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
					fprintf(stream, "%c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->promoted_bishops[i])), RANK_SYMBOL(SQUARE_RANK(pieces->promoted_bishops[i])));
			fputc('\n', stream);
		}

		if (pieces->has_promoted_knights)
		{
			fprintf(stream, "Knights: ");
			for (int i = 0; i < 8; i++)
				if (pieces->promoted_knights[i] != SQUARE_NONE)
					fprintf(stream, "%c%c\n", FILE_SYMBOL(SQUARE_FILE(pieces->promoted_knights[i])), RANK_SYMBOL(SQUARE_RANK(pieces->promoted_knights[i])));
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
}