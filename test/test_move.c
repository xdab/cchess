#include "test_util.h"
#include "move.h"
#include "square.h"
#include "ranks.h"
#include "files.h"
#include "piece.h"

int main(int argc, char const *argv[])
{
	test_init();

	move_t move = move_promotion(SQUARE_E7, SQUARE_E8, PIECE_QUEEN);
	if (move_get_from(move) != SQUARE_E7)
		test_error("move_get_from(move_promotion(SQUARE_E7, SQUARE_E8, PIECE_QUEEN)) != SQUARE_E7");
	if (move_get_to(move) != SQUARE_E8)
		test_error("move_get_to(move_promotion(SQUARE_E7, SQUARE_E8, PIECE_QUEEN)) != SQUARE_E8");
	if (move_get_promoted_piece(move) != PIECE_QUEEN)
		test_error("move_get_promoted_piece(move_promotion(SQUARE_E7, SQUARE_E8, PIECE_QUEEN)) != PIECE_QUEEN");

	test_exit();
}
