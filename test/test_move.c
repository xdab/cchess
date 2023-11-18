#include "test_util.h"
#include "move.h"
#include "square.h"
#include "piece.h"

int main(int argc, char const *argv[])
{
	test_init();

	move_t move = move_promotion(E7, E8, QUEEN);
	if (move_get_from(move) != E7)
		test_error("move_get_from(move_promotion(E7, E8, QUEEN)) != E7");
	if (move_get_to(move) != E8)
		test_error("move_get_to(move_promotion(E7, E8, QUEEN)) != E8");
	if (move_get_promoted_piece(move) != QUEEN)
		test_error("move_get_promoted_piece(move_promotion(E7, E8, QUEEN)) != QUEEN");

	test_exit();
}
