#include "test_util.h"
#include "board.h"

void _check_white_pieces(const board_t *board);
void _check_white_pawns(const board_t *board);
void _check_black_pieces(const board_t *board);
void _check_black_pawns(const board_t *board);
void _check_empty_squares(const board_t *board);

int main(int argc, char const *argv[])
{
	test_init();

	board_t board;
	board_init(&board);
	_check_white_pieces(&board);
	_check_white_pawns(&board);
	_check_black_pieces(&board);
	_check_black_pawns(&board);
	_check_empty_squares(&board);

	test_exit();
}

void _check_white_pieces(const board_t *board)
{
	piece_t a1 = board_get(board, A1);
	if (!(a1 & (ROOK | WHITE)))
		test_error("Starting position must have a white rook on a1");

	piece_t b1 = board_get(board, B1);
	if (!(b1 & (KNIGHT | WHITE)))
		test_error("Starting position must have a white knight on b1");

	piece_t c1 = board_get(board, C1);
	if (!(c1 & (BISHOP | WHITE)))
		test_error("Starting position must have a white bishop on c1");

	piece_t d1 = board_get(board, D1);
	if (!(d1 & (QUEEN | WHITE)))
		test_error("Starting position must have a white queen on d1");

	piece_t e1 = board_get(board, E1);
	if (!(e1 & (KING | WHITE)))
		test_error("Starting position must have a white king on e1");

	piece_t f1 = board_get(board, F1);
	if (!(f1 & (BISHOP | WHITE)))
		test_error("Starting position must have a white bishop on f1");

	piece_t g1 = board_get(board, G1);
	if (!(g1 & (KNIGHT | WHITE)))
		test_error("Starting position must have a white knight on g1");

	piece_t h1 = board_get(board, H1);
	if (!(h1 & (ROOK | WHITE)))
		test_error("Starting position must have a white rook on h1");
}

void _check_white_pawns(const board_t *board)
{
	for (file_t file = FILE_A; file <= FILE_H; file++)
	{
		square_t square = SQUARE_OF(file, RANK_2);
		piece_t piece = board_get(board, square);
		if (!(piece & (PAWN | WHITE)))
			test_error("Starting position must have a white pawn on the second rank");
	}
}

void _check_black_pieces(const board_t *board)
{
	piece_t a8 = board_get(board, A8);
	if (!(a8 & (ROOK | BLACK)))
		test_error("Starting position must have a black rook on a8");

	piece_t b8 = board_get(board, B8);
	if (!(b8 & (KNIGHT | BLACK)))
		test_error("Starting position must have a black knight on b8");

	piece_t c8 = board_get(board, C8);
	if (!(c8 & (BISHOP | BLACK)))
		test_error("Starting position must have a black bishop on c8");

	piece_t d8 = board_get(board, D8);
	if (!(d8 & (QUEEN | BLACK)))
		test_error("Starting position must have a black queen on d8");

	piece_t e8 = board_get(board, E8);
	if (!(e8 & (KING | BLACK)))
		test_error("Starting position must have a black king on e8");

	piece_t f8 = board_get(board, F8);
	if (!(f8 & (BISHOP | BLACK)))
		test_error("Starting position must have a black bishop on f8");

	piece_t g8 = board_get(board, G8);
	if (!(g8 & (KNIGHT | BLACK)))
		test_error("Starting position must have a black knight on g8");

	piece_t h8 = board_get(board, H8);
	if (!(h8 & (ROOK | BLACK)))
		test_error("Starting position must have a black rook on h8");
}

void _check_black_pawns(const board_t *board)
{
	for (file_t file = FILE_A; file <= FILE_H; file++)
	{
		square_t square = SQUARE_OF(file, RANK_7);
		piece_t piece = board_get(board, square);
		if (!(piece & (PAWN | BLACK)))
			test_error("Starting position must have a black pawn on the seventh rank");
	}
}

void _check_empty_squares(const board_t *board)
{
	for (file_t file = FILE_A; file <= FILE_H; file++)
		for (rank_t rank = RANK_3; rank <= RANK_6; rank++)
		{
			square_t square = SQUARE_OF(file, rank);
			piece_t piece = board_get(board, square);
			if (piece != PIECE_NONE)
				test_error("Starting position must have empty squares between the third and sixth ranks");
		}
}