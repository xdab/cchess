#include "test_util.h"
#include "file.h"
#include "board.h"
#include "board_move.h"
#include "board_pieces.h"

void check_pawn_moves_from_initial_position();
void check_knight_moves_from_initial_position();

int main()
{
	test_init();

	check_pawn_moves_from_initial_position();

	test_exit();
}

void check_pawn_moves_from_initial_position()
{
	board_t board;
	board_init(&board);

	board_move(&board, move_regular(E2, E4)); // 1. e4
	if (board.white_pieces.pawns[FILE_E] != E4)
		test_error("After 1. e4, whites e pawn should be on e4");

	board_move(&board, move_regular(E7, E5)); // 1. ... e5
	if (board.black_pieces.pawns[FILE_E] != E5)
		test_error("After 1. ... e5, blacks e pawn should be on e5");

	board_move(&board, move_regular(D2, D4)); // 2. d4
	if (board.white_pieces.pawns[FILE_D] != D4)
		test_error("After 2. d4, whites d pawn should be on d4");

	board_move(&board, move_regular(E5, D4)); // 2. ... exd4
	if (board.black_pieces.pawns[FILE_E] != D4)
		test_error("After 2. ... exd4, blacks former e pawn should be on d4");
	if (board.white_pieces.pawns[FILE_D] != SQUARE_NONE)
		test_error("After 2. ... exd4, whites d pawn should not be present on d4");

	board_unmove(&board);
	if (board.black_pieces.pawns[FILE_E] != E5)
		test_error("After unmoving 2. ... exd4, blacks former e pawn should be back on e5");
	if (board.white_pieces.pawns[FILE_D] != D4)
		test_error("After unmoving 2. ... exd4, whites d pawn should be back on d4");
}

void check_knight_moves_from_initial_position()
{
	board_t board;
	board_init(&board);

	board_move(&board, move_regular(B1, C3)); // 1. Nc3
	if (board.white_pieces.queens_knight != C3)
		test_error("After 1. Nc3, whites queens knight should be on c3");

	board_unmove(&board);
	if (board.white_pieces.queens_knight != B1)
		test_error("After unmoving 1. Nc3, whites queens knight should be on b1");
}
