#include "test_util.h"
#include "board.h"
#include "movegen.h"

void check_first_few_moves();

int main(int argc, char const *argv[])
{
	test_init();

	check_first_few_moves();

	test_exit();
}

void check_first_few_moves()
{
	board_t board;
	move_t moves[MAX_MOVES];
	int move_count;

	board_init(&board);
	movegen_generate(&board, moves, &move_count);
	if (move_count != 20)
		test_error("Initial position did not have 20 (pseudo)legal moves");

	board_make_move(&board, move_regular(SQUARE_E2, SQUARE_E4)); // 1. e4
	movegen_generate(&board, moves, &move_count);
	if (move_count != 20)
		test_error("After 1. e4, there were != 20 (pseudo)legal moves for black");	

	board_make_move(&board, move_regular(SQUARE_E7, SQUARE_E5)); // 1. ... e5
	movegen_generate(&board, moves, &move_count);
	if (move_count != 29)
		test_error("After 1. e4 e5, there were != 29 (pseudo)legal moves for white");
}