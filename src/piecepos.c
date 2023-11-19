#include "piecepos.h"
#include "side.h"

#include <string.h>

void _piecepos_update_current(piecepos_t *current, square_t from, square_t target, piece_t moved_piece, piece_t promotion_piece);
void _piecepos_update_current_pawn(piecepos_t *current, square_t from, square_t target, piece_t moved_piece, piece_t promotion_piece);
void _piecepos_update_promoted_piece(piece_t pieces[8], square_t from, square_t target);
void _piecepos_update_opponent(piecepos_t *opponent, square_t target, piece_t captured_piece);

void piecepos_init_white(piecepos_t *piece_positions)
{
	piece_positions->king = E1;
	piece_positions->queen = D1;
	piece_positions->kings_rook = H1;
	piece_positions->queens_rook = A1;
	piece_positions->kings_knight = G1;
	piece_positions->queens_knight = B1;
	piece_positions->kings_bishop = F1;
	piece_positions->queens_bishop = C1;

	piece_positions->has_pawns = true;
	piece_positions->pawns[0] = A2;
	piece_positions->pawns[1] = B2;
	piece_positions->pawns[2] = C2;
	piece_positions->pawns[3] = D2;
	piece_positions->pawns[4] = E2;
	piece_positions->pawns[5] = F2;
	piece_positions->pawns[6] = G2;
	piece_positions->pawns[7] = H2;

	piece_positions->has_promoted_pieces = false;
	memset(piece_positions->promoted_queens, SQUARE_NONE, sizeof(piece_positions->promoted_queens));
	memset(piece_positions->promoted_rooks, SQUARE_NONE, sizeof(piece_positions->promoted_rooks));
	memset(piece_positions->promoted_bishops, SQUARE_NONE, sizeof(piece_positions->promoted_bishops));
	memset(piece_positions->promoted_knights, SQUARE_NONE, sizeof(piece_positions->promoted_knights));
	piece_positions->has_promoted_queens = false;
	piece_positions->has_promoted_rooks = false;
	piece_positions->has_promoted_bishops = false;
	piece_positions->has_promoted_knights = false;
}

void piecepos_init_black(piecepos_t *piece_positions)
{
	piece_positions->king = E8;
	piece_positions->queen = D8;
	piece_positions->kings_rook = H8;
	piece_positions->queens_rook = A8;
	piece_positions->kings_knight = G8;
	piece_positions->queens_knight = B8;
	piece_positions->kings_bishop = F8;
	piece_positions->queens_bishop = C8;

	piece_positions->has_pawns = true;
	piece_positions->pawns[0] = A7;
	piece_positions->pawns[1] = B7;
	piece_positions->pawns[2] = C7;
	piece_positions->pawns[3] = D7;
	piece_positions->pawns[4] = E7;
	piece_positions->pawns[5] = F7;
	piece_positions->pawns[6] = G7;
	piece_positions->pawns[7] = H7;

	piece_positions->has_promoted_pieces = false;
	memset(piece_positions->promoted_queens, SQUARE_NONE, sizeof(piece_positions->promoted_queens));
	memset(piece_positions->promoted_rooks, SQUARE_NONE, sizeof(piece_positions->promoted_rooks));
	memset(piece_positions->promoted_bishops, SQUARE_NONE, sizeof(piece_positions->promoted_bishops));
	memset(piece_positions->promoted_knights, SQUARE_NONE, sizeof(piece_positions->promoted_knights));
	piece_positions->has_promoted_queens = false;
	piece_positions->has_promoted_rooks = false;
	piece_positions->has_promoted_bishops = false;
	piece_positions->has_promoted_knights = false;
}

void piecepos_print(const piecepos_t *piece_positions, FILE *stream)
{
	fprintf(stream, "King: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->king)), RANK_SYMBOL(SQUARE_RANK(piece_positions->king)));

	if (piece_positions->queen != SQUARE_NONE)
		fprintf(stream, "Queen: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->queen)), RANK_SYMBOL(SQUARE_RANK(piece_positions->queen)));

	if (piece_positions->kings_rook != SQUARE_NONE)
		fprintf(stream, "Kings rook: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->kings_rook)), RANK_SYMBOL(SQUARE_RANK(piece_positions->kings_rook)));

	if (piece_positions->queens_rook != SQUARE_NONE)
		fprintf(stream, "Queens rook: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->queens_rook)), RANK_SYMBOL(SQUARE_RANK(piece_positions->queens_rook)));

	if (piece_positions->kings_knight != SQUARE_NONE)
		fprintf(stream, "Kings knight: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->kings_knight)), RANK_SYMBOL(SQUARE_RANK(piece_positions->kings_knight)));

	if (piece_positions->queens_knight != SQUARE_NONE)
		fprintf(stream, "Queens knight: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->queens_knight)), RANK_SYMBOL(SQUARE_RANK(piece_positions->queens_knight)));

	if (piece_positions->kings_bishop != SQUARE_NONE)
		fprintf(stream, "Kings bishop: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->kings_bishop)), RANK_SYMBOL(SQUARE_RANK(piece_positions->kings_bishop)));

	if (piece_positions->queens_bishop != SQUARE_NONE)
		fprintf(stream, "Queens bishop: %c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->queens_bishop)), RANK_SYMBOL(SQUARE_RANK(piece_positions->queens_bishop)));

	if (piece_positions->has_pawns)
	{
		fprintf(stream, "Pawns: ");
		for (int i = 0; i < 8; i++)
			if (piece_positions->pawns[i] != SQUARE_NONE)
				fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(piece_positions->pawns[i])), RANK_SYMBOL(SQUARE_RANK(piece_positions->pawns[i])));
		fputc('\n', stream);
	}

	if (piece_positions->has_promoted_pieces)
	{
		fprintf(stream, "Promoted pieces:\n");
		if (piece_positions->has_promoted_queens)
		{
			fprintf(stream, "Queens: ");
			for (int i = 0; i < 8; i++)
				if (piece_positions->promoted_queens[i] != SQUARE_NONE)
					fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(piece_positions->promoted_queens[i])), RANK_SYMBOL(SQUARE_RANK(piece_positions->promoted_queens[i])));
			fputc('\n', stream);
		}

		if (piece_positions->has_promoted_rooks)
		{
			fprintf(stream, "Rooks: ");
			for (int i = 0; i < 8; i++)
				if (piece_positions->promoted_rooks[i] != SQUARE_NONE)
					fprintf(stream, "%c%c ", FILE_SYMBOL(SQUARE_FILE(piece_positions->promoted_rooks[i])), RANK_SYMBOL(SQUARE_RANK(piece_positions->promoted_rooks[i])));
			fputc('\n', stream);
		}

		if (piece_positions->has_promoted_bishops)
		{
			fprintf(stream, "Bishops: ");
			for (int i = 0; i < 8; i++)
				if (piece_positions->promoted_bishops[i] != SQUARE_NONE)
					fprintf(stream, "%c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->promoted_bishops[i])), RANK_SYMBOL(SQUARE_RANK(piece_positions->promoted_bishops[i])));
			fputc('\n', stream);
		}

		if (piece_positions->has_promoted_knights)
		{
			fprintf(stream, "Knights: ");
			for (int i = 0; i < 8; i++)
				if (piece_positions->promoted_knights[i] != SQUARE_NONE)
					fprintf(stream, "%c%c\n", FILE_SYMBOL(SQUARE_FILE(piece_positions->promoted_knights[i])), RANK_SYMBOL(SQUARE_RANK(piece_positions->promoted_knights[i])));
			fputc('\n', stream);
		}
	}
}

void piecepos_update(piecepos_t *white, piecepos_t *black, square_t from, square_t target, piece_t moved_piece, piece_t promotion_piece, piece_t captured_piece)
{
	if (moved_piece & WHITE)
	{
		_piecepos_update_current(white, from, target, moved_piece, promotion_piece);
		_piecepos_update_opponent(black, target, captured_piece);
	}
	else
	{
		_piecepos_update_current(black, from, target, moved_piece, promotion_piece);
		_piecepos_update_opponent(white, target, captured_piece);
	}
}

void piecepos_from_squares(piecepos_t *piece_positions, const piece_t squares[SQUARE_COUNT], side_t side)
{
	memset(piece_positions, SQUARE_NONE, sizeof(piecepos_t));
	piece_positions->has_pawns = false;
	piece_positions->has_promoted_pieces = false;
	piece_positions->has_promoted_queens = false;
	piece_positions->has_promoted_rooks = false;
	piece_positions->has_promoted_bishops = false;
	piece_positions->has_promoted_knights = false;

	for (square_t sq = SQUARE_MIN; sq <= SQUARE_MAX; sq++)
	{
		piece_t piece = squares[sq];

		if (piece == PIECE_NONE)
			continue; // No piece on this square

		if (!(piece & side))
			continue; // Not the side we're looking for

		if (piece & KING)
			piece_positions->king = sq;

		else if (piece & PAWN)
		{
			piece_positions->has_pawns = true;
			for (int i = 0; i < 8; i++)
				if (piece_positions->pawns[i] == SQUARE_NONE)
				{
					piece_positions->pawns[i] = sq;
					break;
				}
		}

		else if (piece & KNIGHT)
		{
			if (piece_positions->kings_knight == SQUARE_NONE)
				piece_positions->kings_knight = sq;
			else if (piece_positions->queens_knight == SQUARE_NONE)
				piece_positions->queens_knight = sq;
			else
			{
				piece_positions->has_promoted_pieces = true;
				piece_positions->has_promoted_knights = true;
				for (int i = 0; i < 8; i++)
					if (piece_positions->promoted_knights[i] == SQUARE_NONE)
					{
						piece_positions->promoted_knights[i] = sq;
						break;
					}
			}
		}

		else if (piece & BISHOP)
		{
			bool is_dark_square = SQUARE_DARK(sq);
			bool is_light_square = SQUARE_LIGHT(sq);

			if (is_light_square && (piece_positions->kings_bishop == SQUARE_NONE))
				piece_positions->kings_bishop = sq;
			else if (is_dark_square && (piece_positions->queens_bishop == SQUARE_NONE))
				piece_positions->queens_bishop = sq;
			else
			{
				piece_positions->has_promoted_pieces = true;
				piece_positions->has_promoted_bishops = true;
				for (int i = 0; i < 8; i++)
					if (piece_positions->promoted_bishops[i] == SQUARE_NONE)
					{
						piece_positions->promoted_bishops[i] = sq;
						break;
					}
			}
		}

		else if (piece & ROOK)
		{
			if (piece_positions->kings_rook == SQUARE_NONE)
				piece_positions->kings_rook = sq;
			else if (piece_positions->queens_rook == SQUARE_NONE)
				piece_positions->queens_rook = sq;
			else
			{
				piece_positions->has_promoted_pieces = true;
				piece_positions->has_promoted_rooks = true;
				for (int i = 0; i < 8; i++)
					if (piece_positions->promoted_rooks[i] == SQUARE_NONE)
					{
						piece_positions->promoted_rooks[i] = sq;
						break;
					}
			}
		}

		else if (piece & QUEEN)
		{
			if (piece_positions->queen == SQUARE_NONE)
				piece_positions->queen = sq;
			else
			{
				piece_positions->has_promoted_pieces = true;
				piece_positions->has_promoted_queens = true;
				for (int i = 0; i < 8; i++)
					if (piece_positions->promoted_queens[i] == SQUARE_NONE)
					{
						piece_positions->promoted_queens[i] = sq;
						break;
					}
			}
		}
	}
}

void _piecepos_update_current(piecepos_t *current, square_t from, square_t target, piece_t moved_piece, piece_t promotion_piece)
{
	if (moved_piece & PAWN)
		_piecepos_update_current_pawn(current, from, target, moved_piece, promotion_piece);
	else if (moved_piece & KING)
	{
		current->king = target;
		// TODO Castling
	}
	else if (moved_piece & QUEEN)
	{
		if (from == current->queen)
			current->queen = target;
		else if (current->has_promoted_queens)
			_piecepos_update_promoted_piece(current->promoted_queens, from, target);
	}
	else if (moved_piece & ROOK)
	{
		if (from == current->kings_rook)
			current->kings_rook = target;
		else if (from == current->queens_rook)
			current->queens_rook = target;
		else if (current->has_promoted_rooks)
			_piecepos_update_promoted_piece(current->promoted_rooks, from, target);
	}
	else if (moved_piece & BISHOP)
	{
		if (from == current->kings_bishop)
			current->kings_bishop = target;
		else if (from == current->queens_bishop)
			current->queens_bishop = target;
		else if (current->has_promoted_bishops)
			_piecepos_update_promoted_piece(current->promoted_bishops, from, target);
	}
	else if (moved_piece & KNIGHT)
	{
		if (from == current->kings_knight)
			current->kings_knight = target;
		else if (from == current->queens_knight)
			current->queens_knight = target;
		else if (current->has_promoted_knights)
			_piecepos_update_promoted_piece(current->promoted_knights, from, target);
	}
}

void _piecepos_update_current_pawn(piecepos_t *current, square_t from, square_t target, piece_t moved_piece, piece_t promotion_piece)
{
	for (int i = 0; i < 8; i++)
	{
		if (current->pawns[i] == from)
		{
			if (promotion_piece == PIECE_NONE)
				current->pawns[i] = target;
			else
			{
				current->pawns[i] = SQUARE_NONE;
				current->has_promoted_pieces = true;
				if (promotion_piece & QUEEN)
				{
					current->promoted_queens[i] = target;
					current->has_promoted_queens = true;
				}
				else if (promotion_piece & ROOK)
				{
					current->promoted_rooks[i] = target;
					current->has_promoted_rooks = true;
				}
				else if (promotion_piece & BISHOP)
				{
					current->promoted_bishops[i] = target;
					current->has_promoted_bishops = true;
				}
				else if (promotion_piece & KNIGHT)
				{
					current->promoted_knights[i] = target;
					current->has_promoted_knights = true;
				}
			}

			return;
		}
	}
}

void _piecepos_update_promoted_piece(piece_t pieces[8], square_t from, square_t target)
{
	for (int i = 0; i < 8; i++)
	{
		if (pieces[i] == from)
		{
			pieces[i] = target;
			return;
		}
	}
}

void _piecepos_update_opponent(piecepos_t *opponent, square_t target, piece_t captured_piece)
{
	if (captured_piece & PAWN)
	{
		for (int i = 0; i < 8; i++)
		{
			if (opponent->pawns[i] == target)
			{
				opponent->pawns[i] = SQUARE_NONE;
				return;
			}
		}
	}
	else if (captured_piece & QUEEN)
	{
		if (opponent->queen == target)
			opponent->queen = SQUARE_NONE;
		else if (opponent->has_promoted_queens)
			_piecepos_update_promoted_piece(opponent->promoted_queens, target, SQUARE_NONE);
	}
	else if (captured_piece & ROOK)
	{
		if (opponent->kings_rook == target)
			opponent->kings_rook = SQUARE_NONE;
		else if (opponent->queens_rook == target)
			opponent->queens_rook = SQUARE_NONE;
		else if (opponent->has_promoted_rooks)
			_piecepos_update_promoted_piece(opponent->promoted_rooks, target, SQUARE_NONE);
	}
	else if (captured_piece & BISHOP)
	{
		if (opponent->kings_bishop == target)
			opponent->kings_bishop = SQUARE_NONE;
		else if (opponent->queens_bishop == target)
			opponent->queens_bishop = SQUARE_NONE;
		else if (opponent->has_promoted_bishops)
			_piecepos_update_promoted_piece(opponent->promoted_bishops, target, SQUARE_NONE);
	}
	else if (captured_piece & KNIGHT)
	{
		if (opponent->kings_knight == target)
			opponent->kings_knight = SQUARE_NONE;
		else if (opponent->queens_knight == target)
			opponent->queens_knight = SQUARE_NONE;
		else if (opponent->has_promoted_knights)
			_piecepos_update_promoted_piece(opponent->promoted_knights, target, SQUARE_NONE);
	}
}