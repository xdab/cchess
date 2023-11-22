#include "test_util.h"
#include "board.h"
#include "board_fen.h"
#include "search.h"

void check_free_queen_1();

int main(int argc, char const *argv[])
{
	test_init();

	check_free_queen_1();

	test_exit();
}

void check_free_queen_1()
{
	board_t board;
	board_set_fen(&board, "rnb1kbnr/ppp1p1pp/5p2/3q4/8/2N5/PPPP1PPP/R1BQKBNR w KQkq - 0 4");

	board_print(&board, stderr);

	const move_t best_move = move_regular(C3, D5);
	for (int depth = 2; depth <= 5; depth++)
	{
		move_t search_best_move;
		score_t score = search(&board, depth, &search_best_move);

		char search_best_move_uci[6];
		move_to_uci(search_best_move, search_best_move_uci);
		fprintf(stderr, "depth %d: %+d cp after move %s\n", depth, score, search_best_move_uci);

		if (search_best_move != best_move)
			test_error("Expected best move c3d5");
	}
}