#include "test_util.h"
#include "board_fen.h"

#include <string.h>

void check_put();
void check_get();
void check_put_get();

int main(void)
{
	test_init();

	check_put();
	check_get();
	check_put_get();

	test_exit();
}

void check_put()
{
	board_t board;

	board_init(&board);
	
	board_set_fen(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	if (board_get(&board, A1) != (ROOK | WHITE))
		test_error("A1 does not contain a white rook");
	if (board_get(&board, B1) != (KNIGHT | WHITE))
		test_error("B1 does not contain a white knight");
	if (board_get(&board, C1) != (BISHOP | WHITE))
		test_error("C1 does not contain a white bishop");
	if (board_get(&board, D1) != (QUEEN | WHITE))
		test_error("D1 does not contain a white queen");
	if (board_get(&board, E1) != (KING | WHITE))
		test_error("E1 does not contain a white king");
	if (board_get(&board, F1) != (BISHOP | WHITE))
		test_error("F1 does not contain a white bishop");
	if (board_get(&board, G1) != (KNIGHT | WHITE))
		test_error("G1 does not contain a white knight");
	if (board_get(&board, H1) != (ROOK | WHITE))
		test_error("H1 does not contain a white rook");

	for (square_t sq = A2; sq <= H2; sq++)
		if (board_get(&board, sq) != (PAWN | WHITE))
			test_error("Rank 2 does not contain white pawns");
	
	for (square_t sq = A6; sq <= H3; sq++)
		if (board_get(&board, sq) != PIECE_NONE)
			test_error("Ranks 3-6 do not contain empty squares");
	
	for (square_t sq = A7; sq <= H7; sq++)
		if (board_get(&board, sq) != (PAWN | BLACK))
			test_error("Rank 7 does not contain black pawns");
		
	if (board_get(&board, A8) != (ROOK | BLACK))
		test_error("A8 does not contain a black rook");
	if (board_get(&board, B8) != (KNIGHT | BLACK))
		test_error("B8 does not contain a black knight");
	if (board_get(&board, C8) != (BISHOP | BLACK))
		test_error("C8 does not contain a black bishop");
	if (board_get(&board, D8) != (QUEEN | BLACK))
		test_error("D8 does not contain a black queen");
	if (board_get(&board, E8) != (KING | BLACK))
		test_error("E8 does not contain a black king");
	if (board_get(&board, F8) != (BISHOP | BLACK))
		test_error("F8 does not contain a black bishop");
	if (board_get(&board, G8) != (KNIGHT | BLACK))
		test_error("G8 does not contain a black knight");
	if (board_get(&board, H8) != (ROOK | BLACK))
		test_error("H8 does not contain a black rook");
}

void check_get()
{
	board_t board;
	char fen[FEN_BUF_LENGTH];
	
	board_init(&board);

	board_get_fen(&board, fen);
	if (strcmp(fen, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"))
		test_error("FEN of starting position is incorrect");
}

void check_put_get()
{
	board_t board;
	char fen[FEN_BUF_LENGTH];

	board_init(&board);

	const char *fen1 = "rnb1kbnr/ppp2ppp/8/q3p3/8/2NP1N2/PPP2PPP/R1BQKB1R b KQkq - 0 5";
	board_set_fen(&board, fen1);
	board_get_fen(&board, fen);
	if (strcmp(fen, fen1))
		test_error("After board_set_fen(..., fen1), board_get_fen did not return the same FEN");
}