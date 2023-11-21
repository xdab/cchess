#include "test_util.h"
#include "board.h"
#include "board_move.h"
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

	board_move(&first, move_regular(E2, E4));

	if (first.hash == second.hash)
		test_error("Two different boards had the same hash");
}

void check_move_unmaking()
{
	board_t board;

	board_init(&board);

	zobrist_t initial_hash = board.hash;

	move_t move = move_regular(E2, E4);
	board_move(&board, move);
	board_unmove(&board);

	zobrist_t final_hash = board.hash;

	if (initial_hash != final_hash)
		test_error("Unmaking a move did not restore the initial hash");
}

void check_knights_returning_to_starting_squares()
{
	board_t board;

	board_init(&board);

	zobrist_t initial_hash = board.hash;

	board_move(&board, move_regular(G1, F3)); // 1. Nf3
	board_move(&board, move_regular(B8, C6)); // 1. ... Nc6
	board_move(&board, move_regular(F3, G1)); // 2. Ng1
	board_move(&board, move_regular(C6, B8)); // 2. ... Nb8

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