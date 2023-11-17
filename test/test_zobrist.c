#include "test_util.h"
#include "board.h"
#include "zobrist.h"

void check_identical_boards()
{
	board_t first;
	board_t second;

	board_init(&first);
	board_init(&second);

	if (first.hash != second.hash)
		test_error("Two initialized boards had different hashes");

	if (first.hash == 0)
		test_error("Initialized board hash was zero");
}

void check_different_boards()
{
	board_t first;
	board_t second;

	board_init(&first);
	board_init(&second);

	board_make_move(&first, move_regular(SQUARE_E2, SQUARE_E4));

	if (first.hash == second.hash)
		test_error("Two different boards had the same hash");
}

void check_move_unmaking()
{
	board_t board;

	board_init(&board);

	zobrist_t initial_hash = board.hash;

	move_t move = move_regular(SQUARE_E2, SQUARE_E4);
	board_make_move(&board, move);
	board_unmake_move(&board);

	zobrist_t final_hash = board.hash;

	if (initial_hash != final_hash)
		test_error("Unmaking a move did not restore the initial hash");
}

void check_knights_returning_to_starting_squares()
{
	board_t board;

	board_init(&board);

	zobrist_t initial_hash = board.hash;

	board_make_move(&board, move_regular(SQUARE_G1, SQUARE_F3)); // 1. Nf3
	board_make_move(&board, move_regular(SQUARE_B8, SQUARE_C6)); // 1. ... Nc6
	board_make_move(&board, move_regular(SQUARE_F3, SQUARE_G1)); // 2. Ng1
	board_make_move(&board, move_regular(SQUARE_C6, SQUARE_B8)); // 2. ... Nb8

	zobrist_t final_hash = board.hash;

	if (initial_hash != final_hash)
		test_error("Moving knights to their starting squares did not restore the initial hash");
}

int main(int argc, char const *argv[])
{
	test_init();
	zobrist_init();

	check_identical_boards();
	check_different_boards();
	check_move_unmaking();
	check_knights_returning_to_starting_squares();

	test_exit();
}