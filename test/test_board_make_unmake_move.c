#include "test_util.h"
#include "board.h"

void check_regular_move(board_t *board);
void check_capture(board_t *board);
void check_promotion(board_t *board);
void check_en_passant(board_t *board);
void check_castling(board_t *board);

int main(int argc, char const *argv[])
{
	test_init();

	board_t board;
	check_regular_move(&board);
	check_capture(&board);
	check_promotion(&board);

	test_exit();
}

void check_regular_move(board_t *board)
{
	board_init(board);

	move_t move = move_regular(SQUARE_E2, SQUARE_E4); // e4
	board_make_move(board, move);
	if (board_get(board, SQUARE_E4) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After move e4, e4 did not contain a white pawn");
	if (board_get(board, SQUARE_E2) != PIECE_NONE)
		test_error("After move e4, e2 was not empty");

	board_unmake_move(board);
	if (board_get(board, SQUARE_E2) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After unmaking e4, white pawn from e4 did not return to e2");
	if (board_get(board, SQUARE_E4) != PIECE_NONE)
		test_error("After unmaking e4, e4 was not empty");
}

void check_capture(board_t *board)
{
	board_init(board);
	board_make_move(board, move_regular(SQUARE_E2, SQUARE_E4)); // e4
	board_make_move(board, move_regular(SQUARE_D7, SQUARE_D5)); // d5

	board_make_move(board, move_regular(SQUARE_E4, SQUARE_D5)); // exd5
	if (board_get(board, SQUARE_D5) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After (1. e4 d5 2. exd5), d5 did not contain a white pawn");
	if (board_get(board, SQUARE_E4) != PIECE_NONE)
		test_error("After (1. e4 d5 2. exd5), e4 was not empty");

	board_unmake_move(board);
	if (board_get(board, SQUARE_E4) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After unmaking (2. exd5), white pawn from d5 did not return to e4");
	if (board_get(board, SQUARE_D5) != (PIECE_PAWN | SIDE_BLACK))
		test_error("After unmaking (2. exd5), a black pawn was not returned to d5");
}

void check_promotion(board_t *board)
{
	board_init(board);
	board_make_move(board, move_regular(SQUARE_C2, SQUARE_C4));				   // 1. c4
	board_make_move(board, move_regular(SQUARE_C7, SQUARE_C5));				   // 1. ... c5
	board_make_move(board, move_regular(SQUARE_D2, SQUARE_D4));				   // 2. d4
	board_make_move(board, move_regular(SQUARE_C5, SQUARE_D4));				   // 2. ... cxd4
	board_make_move(board, move_regular(SQUARE_C4, SQUARE_C5));				   // 3. c5
	board_make_move(board, move_regular(SQUARE_D7, SQUARE_D6));				   // 3. ... d6
	board_make_move(board, move_regular(SQUARE_C5, SQUARE_C6));				   // 4. c6
	board_make_move(board, move_regular(SQUARE_C8, SQUARE_D7));				   // 4. ... Bd7
	board_make_move(board, move_regular(SQUARE_C6, SQUARE_C7));				   // 5. c7
	board_make_move(board, move_regular(SQUARE_D7, SQUARE_E6));				   // 5. ... Be6
	board_make_move(board, move_promotion(SQUARE_C7, SQUARE_C8, PIECE_QUEEN)); // 6. c8=Q
	board_print(board, stderr);
	if (board_get(board, SQUARE_C8) != (PIECE_QUEEN | SIDE_WHITE))
		test_error("After (1. c4 c5 2. d4 cxd4 3. c5 d6 4. c6 Bd7 5. c7 Be6 6. c8=Q), c8 did not contain a white queen");
	if (board_get(board, SQUARE_C7) != PIECE_NONE)
		test_error("After (1. c4 c5 2. d4 cxd4 3. c5 d6 4. c6 Bd7 5. c7 Be6 6. c8=Q), c7 was not empty");
}
