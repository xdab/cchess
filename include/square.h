#ifndef SQUARE_H
#define SQUARE_H

#include "file.h"
#include "rank.h"

#include <stdint.h>

typedef int8_t square_t;

#define SQUARE_NONE ((square_t)(-1))

#define A8 ((square_t)0)
#define B8 ((square_t)1)
#define C8 ((square_t)2)
#define D8 ((square_t)3)
#define E8 ((square_t)4)
#define F8 ((square_t)5)
#define G8 ((square_t)6)
#define H8 ((square_t)7)

#define A7 ((square_t)8)
#define B7 ((square_t)9)
#define C7 ((square_t)10)
#define D7 ((square_t)11)
#define E7 ((square_t)12)
#define F7 ((square_t)13)
#define G7 ((square_t)14)
#define H7 ((square_t)15)

#define A6 ((square_t)16)
#define B6 ((square_t)17)
#define C6 ((square_t)18)
#define D6 ((square_t)19)
#define E6 ((square_t)20)
#define F6 ((square_t)21)
#define G6 ((square_t)22)
#define H6 ((square_t)23)

#define A5 ((square_t)24)
#define B5 ((square_t)25)
#define C5 ((square_t)26)
#define D5 ((square_t)27)
#define E5 ((square_t)28)
#define F5 ((square_t)29)
#define G5 ((square_t)30)
#define H5 ((square_t)31)

#define A4 ((square_t)32)
#define B4 ((square_t)33)
#define C4 ((square_t)34)
#define D4 ((square_t)35)
#define E4 ((square_t)36)
#define F4 ((square_t)37)
#define G4 ((square_t)38)
#define H4 ((square_t)39)

#define A3 ((square_t)40)
#define B3 ((square_t)41)
#define C3 ((square_t)42)
#define D3 ((square_t)43)
#define E3 ((square_t)44)
#define F3 ((square_t)45)
#define G3 ((square_t)46)
#define H3 ((square_t)47)

#define A2 ((square_t)48)
#define B2 ((square_t)49)
#define C2 ((square_t)50)
#define D2 ((square_t)51)
#define E2 ((square_t)52)
#define F2 ((square_t)53)
#define G2 ((square_t)54)
#define H2 ((square_t)55)

#define A1 ((square_t)56)
#define B1 ((square_t)57)
#define C1 ((square_t)58)
#define D1 ((square_t)59)
#define E1 ((square_t)60)
#define F1 ((square_t)61)
#define G1 ((square_t)62)
#define H1 ((square_t)63)

#define SQUARE_MIN A8
#define SQUARE_MAX H1
#define SQUARE_COUNT 64

#define SQUARE_FILE(sq) ((sq) % 8)
#define SQUARE_RANK(sq) (7 - ((sq) / 8))

#define SQUARE_DARK(sq) (((sq) % 2) == (((sq) / 8) % 2))
#define SQUARE_LIGHT(sq) !SQUARE_DARK(sq)

static inline square_t SQUARE_OF(file_t file, rank_t rank)
{
	if (file < FILE_A || file > FILE_H)
		return SQUARE_NONE;
	if (rank < RANK_1 || rank > RANK_8)
		return SQUARE_NONE;
	return (square_t)((7 - (rank)) * 8 + (file));
}

#define SQUARE_VALID(sq) (((sq) >= SQUARE_MIN) && ((sq) <= SQUARE_MAX))

#endif