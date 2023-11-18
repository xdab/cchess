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

	move_t move = move_regular(E2, E4); // e4
	board_make_move(board, move);
	if (board_get(board, E4) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After move e4, e4 did not contain a white pawn");
	if (board_get(board, E2) != PIECE_NONE)
		test_error("After move e4, e2 was not empty");
	if (board->side_to_move != SIDE_BLACK)
		test_error("After move 1. e4, side to move was not black");

	board_unmake_move(board);
	if (board_get(board, E2) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After unmaking e4, white pawn from e4 did not return to e2");
	if (board_get(board, E4) != PIECE_NONE)
		test_error("After unmaking e4, e4 was not empty");
	if (board->side_to_move != SIDE_WHITE)
		test_error("After unmaking 1. e4, side to move was not white");
}

void check_capture(board_t *board)
{
	board_init(board);
	board_make_move(board, move_regular(E2, E4)); // e4
	board_make_move(board, move_regular(D7, D5)); // d5

	board_make_move(board, move_regular(E4, D5)); // exd5
	if (board_get(board, D5) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After (1. e4 d5 2. exd5), d5 did not contain a white pawn");
	if (board_get(board, E4) != PIECE_NONE)
		test_error("After (1. e4 d5 2. exd5), e4 was not empty");
	if (board->side_to_move != SIDE_BLACK)
		test_error("After (1. e4 d5 2. exd5), side to move was not black");

	board_unmake_move(board);
	if (board_get(board, E4) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After unmaking (2. exd5), white pawn from d5 did not return to e4");
	if (board_get(board, D5) != (PIECE_PAWN | SIDE_BLACK))
		test_error("After unmaking (2. exd5), a black pawn was not returned to d5");
	if (board->side_to_move != SIDE_WHITE)
		test_error("After unmaking (2. exd5), side to move was not white");
}

void check_promotion(board_t *board)
{
	board_init(board);
	board_make_move(board, move_regular(C2, C4));				   // 1. c4
	board_make_move(board, move_regular(C7, C5));				   // 1. ... c5
	board_make_move(board, move_regular(D2, D4));				   // 2. d4
	board_make_move(board, move_regular(C5, D4));				   // 2. ... cxd4
	board_make_move(board, move_regular(C4, C5));				   // 3. c5
	board_make_move(board, move_regular(D7, D6));				   // 3. ... d6
	board_make_move(board, move_regular(C5, C6));				   // 4. c6
	board_make_move(board, move_regular(C8, D7));				   // 4. ... Bd7
	board_make_move(board, move_regular(C6, C7));				   // 5. c7
	board_make_move(board, move_regular(D7, E6));				   // 5. ... Be6
	board_make_move(board, move_promotion(C7, C8, PIECE_QUEEN)); // 6. c8=Q
	if (board_get(board, C8) != (PIECE_QUEEN | SIDE_WHITE))
		test_error("After (1. c4 c5 2. d4 cxd4 3. c5 d6 4. c6 Bd7 5. c7 Be6 6. c8=Q), c8 did not contain a white queen");
	if (board_get(board, C7) != PIECE_NONE)
		test_error("After (1. c4 c5 2. d4 cxd4 3. c5 d6 4. c6 Bd7 5. c7 Be6 6. c8=Q), c7 was not empty");
	if (board->side_to_move != SIDE_BLACK)
		test_error("After (1. c4 c5 2. d4 cxd4 3. c5 d6 4. c6 Bd7 5. c7 Be6 6. c8=Q), side to move was not black");
	
	board_unmake_move(board);
	if (board_get(board, C7) != (PIECE_PAWN | SIDE_WHITE))
		test_error("After unmaking (6. c8=Q), white queen from c8 did not return to c7 as a pawn");
	if (board_get(board, C8) != PIECE_NONE)
		test_error("After unmaking (6. c8=Q), c8 was not empty");
	if (board->side_to_move != SIDE_WHITE)
		test_error("After unmaking (6. c8=Q), side to move was not white");
}
