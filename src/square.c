#include "square.h"
#include "board.h"

square_t square_of(int file, int rank)
{
	if (file < FILE_A || file > FILE_H || rank < RANK_1 || rank > RANK_8)
		return SQUARE_NULL;
	return file + (rank << 4);
}

int square_file(square_t square)
{
	return square & 0x0F;
}

int square_rank(square_t square)
{
	return square >> 4;
}

bool square_valid(square_t square)
{
	return square != SQUARE_NULL;
}