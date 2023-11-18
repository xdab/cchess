#include "test_util.h"
#include "square.h"
#include "rank.h"
#include "file.h"

void check_SQUARE_OF();

int main(int argc, char const *argv[])
{
	test_init();

	check_SQUARE_OF();

	test_exit();
}

void check_SQUARE_OF()
{
	// Corners

	if (SQUARE_OF(FILE_A, RANK_1) != A1)
		test_error("SQUARE_OF(FILE_A, RANK_1) != A1");

	if (SQUARE_OF(FILE_A, RANK_8) != A8)
		test_error("SQUARE_OF(FILE_A, RANK_8) != A8");

	if (SQUARE_OF(FILE_H, RANK_1) != H1)
		test_error("SQUARE_OF(FILE_H, RANK_1) != H1");

	if (SQUARE_OF(FILE_H, RANK_8) != H8)
		test_error("SQUARE_OF(FILE_H, RANK_8) != H8");

	// Middle

	if (SQUARE_OF(FILE_C, RANK_3) != C3)
		test_error("SQUARE_OF(FILE_C, RANK_3) != C3");

	if (SQUARE_OF(FILE_D, RANK_4) != D4)
		test_error("SQUARE_OF(FILE_D, RANK_4) != D4");

	if (SQUARE_OF(FILE_E, RANK_5) != E5)
		test_error("SQUARE_OF(FILE_E, RANK_5) != E5");

	if (SQUARE_OF(FILE_F, RANK_6) != F6)
		test_error("SQUARE_OF(FILE_F, RANK_6) != F6");
}
