#include "test_util.h"
#include "file.h"
#include "board.h"
#include "board_move.h"
#include "board_fen.h"
#include "board_pieces.h"

#include <string.h>

void check_pawn_moves_from_initial_position();
void check_knight_moves_from_initial_position();
void check_sample_game_1();
void check_promotions();

int main()
{
	test_init();

	check_pawn_moves_from_initial_position();
	check_knight_moves_from_initial_position();
	check_sample_game_1();
	check_promotions();

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

void check_sample_game_1()
{
	board_t board;
	board_init(&board);

	board_move(&board, move_regular(E2, E4)); // 1. e4
	board_move(&board, move_regular(C7, C5)); // 1. ... c5
	board_move(&board, move_regular(G1, F3)); // 2. Nf3
	board_move(&board, move_regular(B8, C6)); // 2. ... Nc6
	board_move(&board, move_regular(C2, C3)); // 3. c3
	board_move(&board, move_regular(D7, D6)); // 3. ... d6
	board_move(&board, move_regular(D2, D4)); // 4. d4
	board_move(&board, move_regular(C5, D4)); // 4. ... cxd4
	board_move(&board, move_regular(C3, D4)); // 5. cxd4
	board_move(&board, move_regular(E7, E6)); // 5. ... e6
	board_move(&board, move_regular(B1, C3)); // 6. Nc3
	board_move(&board, move_regular(G8, E7)); // 6. ... Nge7
	board_move(&board, move_regular(F1, B5)); // 7. Bb5
	board_move(&board, move_regular(G7, G6)); // 7. ... g6
	board_move(&board, move_regular(E1, G1)); // 8. O-O
	board_move(&board, move_regular(F8, G7)); // 8. ... Bg7
	board_move(&board, move_regular(D4, D5)); // 9. d5
	board_move(&board, move_regular(E6, D5)); // 9. ... exd5

	if (board.white_pieces.king != G1)
		test_error("In sample game 1, whites king should be on g1");
	if (board.white_pieces.queen != D1)
		test_error("In sample game 1, whites queen should be on d1");
	if (board.white_pieces.queens_rook != A1)
		test_error("In sample game 1, whites queens rook should be on a1");
	if (board.white_pieces.kings_rook != F1)
		test_error("In sample game 1, whites kings rook should be on f1");
	if (board.white_pieces.queens_bishop != C1)
		test_error("In sample game 1, whites queens bishop should be on c1");
	if (board.white_pieces.kings_bishop != B5)
		test_error("In sample game 1, whites kings bishop should be on b5");
	if (board.white_pieces.queens_knight != C3)
		test_error("In sample game 1, whites queens knight should be on c3");
	if (board.white_pieces.kings_knight != F3)
		test_error("In sample game 1, whites kings knight should be on f3");
	if (board.white_pieces.pawns[FILE_A] != A2)
		test_error("In sample game 1, whites a pawn should be on a2");
	if (board.white_pieces.pawns[FILE_B] != B2)
		test_error("In sample game 1, whites b pawn should be on b2");
	if (board.white_pieces.pawns[FILE_C] != SQUARE_NONE)
		test_error("In sample game 1, white should not have a c pawn");
	if (board.white_pieces.pawns[FILE_D] != SQUARE_NONE)
		test_error("In sample game 1, white should not have a d pawn");
	if (board.white_pieces.pawns[FILE_E] != E4)
		test_error("In sample game 1, whites e pawn should be on e4");
	if (board.white_pieces.pawns[FILE_F] != F2)
		test_error("In sample game 1, whites f pawn should be on f2");
	if (board.white_pieces.pawns[FILE_G] != G2)
		test_error("In sample game 1, whites g pawn should be on g2");
	if (board.white_pieces.pawns[FILE_H] != H2)
		test_error("In sample game 1, whites h pawn should be on h2");
	if (board.white_pieces.has_promoted_pieces)
		test_error("In sample game 1, white should not have promoted pieces");

	if (board.black_pieces.king != E8)
		test_error("In sample game 1, blacks king should be on e8");
	if (board.black_pieces.queen != D8)
		test_error("In sample game 1, blacks queen should be on d8");
	if (board.black_pieces.queens_rook != A8)
		test_error("In sample game 1, blacks queens rook should be on a8");
	if (board.black_pieces.kings_rook != H8)
		test_error("In sample game 1, blacks kings rook should be on h8");
	if (board.black_pieces.queens_bishop != C8)
		test_error("In sample game 1, blacks queens bishop should be on c8");
	if (board.black_pieces.kings_bishop != G7)
		test_error("In sample game 1, blacks kings bishop should be on g7");
	if (board.black_pieces.queens_knight != C6)
		test_error("In sample game 1, blacks queens knight should be on c6");
	if (board.black_pieces.kings_knight != E7)
		test_error("In sample game 1, blacks kings knight should be on e7");
	if (board.black_pieces.pawns[FILE_A] != A7)
		test_error("In sample game 1, blacks a pawn should be on a7");
	if (board.black_pieces.pawns[FILE_B] != B7)
		test_error("In sample game 1, blacks b pawn should be on b7");
	if (board.black_pieces.pawns[FILE_C] != SQUARE_NONE)
		test_error("In sample game 1, black should not have a c pawn");
	if (board.black_pieces.pawns[FILE_D] != D6)
		test_error("In sample game 1, blacks d pawn should be on d6");
	if (board.black_pieces.pawns[FILE_E] != D5)
		test_error("In sample game 1, blacks e pawn should be on d5");
	if (board.black_pieces.pawns[FILE_F] != F7)
		test_error("In sample game 1, blacks f pawn should be on f7");
	if (board.black_pieces.pawns[FILE_G] != G6)
		test_error("In sample game 1, blacks g pawn should be on g6");
	if (board.black_pieces.pawns[FILE_H] != H7)
		test_error("In sample game 1, blacks h pawn should be on h7");
	if (board.black_pieces.has_promoted_pieces)
		test_error("In sample game 1, black should not have promoted pieces");

	for (int i = 0; i < 18; i++)
		board_unmove(&board);

	board_pieces_t init_pieces;

	board_pieces_init(&init_pieces, WHITE);
	if (memcmp(&init_pieces, &board.white_pieces, sizeof(board_pieces_t)))
		test_error("After unmaking all moves, white pieces do not equal white starting position");

	board_pieces_init(&init_pieces, BLACK);
	if (memcmp(&init_pieces, &board.black_pieces, sizeof(board_pieces_t)))
		test_error("After unmaking all moves, black pieces do not equal black starting position");
}

void check_promotions()
{
	board_t board;
	board_set_fen(&board, "1r6/P1P1P1P1/6K1/8/8/1k6/1p1p1p1p/8 w - - 0 1");

	board_move(&board, move_promotion(A7, B8, QUEEN));
	board_unmove(&board);

	if (board.white_pieces.has_promoted_pieces)
		test_error("check_promotions: after unmaking axb8=Q there are still promoted pieces");

	if (board.black_pieces.kings_rook != B8)
		test_error("check_promotions: rook did not return to b8");
}