#include "test_util.h"
#include "board.h"
#include "board_fen.h"
#include "board_move.h"
#include "movegen.h"

void check_first_few_moves();
void check_sample_position_1();
void check_sample_position_2();
void check_sample_position_3();

int main(int argc, char const *argv[])
{
	test_init();

	check_first_few_moves();
	check_sample_position_1();

	test_exit();
}

void check_first_few_moves()
{
	board_t board;
	move_t moves[MAX_MOVES];
	int move_count;

	board_init(&board);
	movegen_generate(&board, moves, &move_count);

	fprintf(stderr, "move_count = %d\n", move_count);
	for (int i = 0; i < move_count; ++i)
	{
		char move_str[6];
		move_to_uci(moves[i], move_str);
		fprintf(stderr, "%s ", move_str);
	}
	fprintf(stderr, "\n");

	if (move_count != 20)
		test_error("Initial position did not have 20 (pseudo)legal moves");

	board_move(&board, move_regular(E2, E4)); // 1. e4
	movegen_generate(&board, moves, &move_count);
	if (move_count != 20)
		test_error("After 1. e4, there were != 20 (pseudo)legal moves for black");

	board_move(&board, move_regular(E7, E5)); // 1. ... e5
	movegen_generate(&board, moves, &move_count);
	if (move_count != 29)
		test_error("After 1. e4 e5, there were != 29 (pseudo)legal moves for white");
}

void check_sample_position_1()
{
	board_t board;
	move_t moves[MAX_MOVES];
	int move_count;

	// Kasparov vs Karpov, 1984 World Championship, position after 26. Nc6
	board_set_fen(&board, "2r3k1/p4ppp/b1N5/1p1P4/8/b5P1/n4PBP/B3R1K1 b - - 1 27");
	movegen_generate(&board, moves, &move_count);
	if (move_count != 27)
		test_error("In sample position 1 there were != 27 (pseudo)legal moves for black");
}

void check_sample_position_2()
{
	board_t board;
	move_t moves[MAX_MOVES];
	int move_count;

	// Anderssen vs Kieseritzky, 1851, position after 15. ... Qxf6
	board_set_fen(&board, "rnb1kbnr/p2p1ppp/5q2/1p3N1P/4PBP1/3P1Q2/PPP5/RN3KR1 w kq - 1 16");
	movegen_generate(&board, moves, &move_count);
	if (move_count != 42)
		test_error("In sample position 2 there were != 42 (pseudo)legal moves for white");
}

void check_sample_position_3()
{
	board_t board;
	move_t moves[MAX_MOVES];
	int move_count;

	// Carlsen vs Nakaumra, 2015 Sinquefield Cup, position after 26. ... a5
	board_set_fen(&board, "3r1rk1/1B3ppp/3b4/pp6/b2P4/2P1B3/1R1K1PPP/R7 w - - 0 27");
	movegen_generate(&board, moves, &move_count);
	if (move_count != 40)
		test_error("In sample position 3 there were != 40 (pseudo)legal moves for white");
}