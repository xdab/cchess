#ifndef SQUARE_H
#define SQUARE_H

#include <stdbool.h>

typedef unsigned char square_t;

/**
 * Returns the square of the given file and rank.
 *
 * @param file The file of the square.
 * @param rank The rank of the square.
 *
 * @return The square of the given file and rank.
 */
square_t square_of(int file, int rank);

/**
 * Returns the file of the given square.
 *
 * @param square The square.
 *
 * @return The file of the given square.
 */
int square_file(square_t square);

/**
 * Returns the rank of the given square.
 *
 * @param square The square.
 *
 * @return The rank of the given square.
 */
int square_rank(square_t square);

/**
 * Returns whether the given square is valid.
 *
 * @param square The square.
 *
 * @return Whether the given square is valid.
 */
bool square_valid(square_t square);

// Thanks Copilot ;-)
#define SQUARE_NULL (0xFF)
#define SQUARE_A1 (square_of(FILE_A, RANK_1))
#define SQUARE_A2 (square_of(FILE_A, RANK_2))
#define SQUARE_A3 (square_of(FILE_A, RANK_3))
#define SQUARE_A4 (square_of(FILE_A, RANK_4))
#define SQUARE_A5 (square_of(FILE_A, RANK_5))
#define SQUARE_A6 (square_of(FILE_A, RANK_6))
#define SQUARE_A7 (square_of(FILE_A, RANK_7))
#define SQUARE_A8 (square_of(FILE_A, RANK_8))
#define SQUARE_B1 (square_of(FILE_B, RANK_1))
#define SQUARE_B2 (square_of(FILE_B, RANK_2))
#define SQUARE_B3 (square_of(FILE_B, RANK_3))
#define SQUARE_B4 (square_of(FILE_B, RANK_4))
#define SQUARE_B5 (square_of(FILE_B, RANK_5))
#define SQUARE_B6 (square_of(FILE_B, RANK_6))
#define SQUARE_B7 (square_of(FILE_B, RANK_7))
#define SQUARE_B8 (square_of(FILE_B, RANK_8))
#define SQUARE_C1 (square_of(FILE_C, RANK_1))
#define SQUARE_C2 (square_of(FILE_C, RANK_2))
#define SQUARE_C3 (square_of(FILE_C, RANK_3))
#define SQUARE_C4 (square_of(FILE_C, RANK_4))
#define SQUARE_C5 (square_of(FILE_C, RANK_5))
#define SQUARE_C6 (square_of(FILE_C, RANK_6))
#define SQUARE_C7 (square_of(FILE_C, RANK_7))
#define SQUARE_C8 (square_of(FILE_C, RANK_8))
#define SQUARE_D1 (square_of(FILE_D, RANK_1))
#define SQUARE_D2 (square_of(FILE_D, RANK_2))
#define SQUARE_D3 (square_of(FILE_D, RANK_3))
#define SQUARE_D4 (square_of(FILE_D, RANK_4))
#define SQUARE_D5 (square_of(FILE_D, RANK_5))
#define SQUARE_D6 (square_of(FILE_D, RANK_6))
#define SQUARE_D7 (square_of(FILE_D, RANK_7))
#define SQUARE_D8 (square_of(FILE_D, RANK_8))
#define SQUARE_E1 (square_of(FILE_E, RANK_1))
#define SQUARE_E2 (square_of(FILE_E, RANK_2))
#define SQUARE_E3 (square_of(FILE_E, RANK_3))
#define SQUARE_E4 (square_of(FILE_E, RANK_4))
#define SQUARE_E5 (square_of(FILE_E, RANK_5))
#define SQUARE_E6 (square_of(FILE_E, RANK_6))
#define SQUARE_E7 (square_of(FILE_E, RANK_7))
#define SQUARE_E8 (square_of(FILE_E, RANK_8))
#define SQUARE_F1 (square_of(FILE_F, RANK_1))
#define SQUARE_F2 (square_of(FILE_F, RANK_2))
#define SQUARE_F3 (square_of(FILE_F, RANK_3))
#define SQUARE_F4 (square_of(FILE_F, RANK_4))
#define SQUARE_F5 (square_of(FILE_F, RANK_5))
#define SQUARE_F6 (square_of(FILE_F, RANK_6))
#define SQUARE_F7 (square_of(FILE_F, RANK_7))
#define SQUARE_F8 (square_of(FILE_F, RANK_8))
#define SQUARE_G1 (square_of(FILE_G, RANK_1))
#define SQUARE_G2 (square_of(FILE_G, RANK_2))
#define SQUARE_G3 (square_of(FILE_G, RANK_3))
#define SQUARE_G4 (square_of(FILE_G, RANK_4))
#define SQUARE_G5 (square_of(FILE_G, RANK_5))
#define SQUARE_G6 (square_of(FILE_G, RANK_6))
#define SQUARE_G7 (square_of(FILE_G, RANK_7))
#define SQUARE_G8 (square_of(FILE_G, RANK_8))
#define SQUARE_H1 (square_of(FILE_H, RANK_1))
#define SQUARE_H2 (square_of(FILE_H, RANK_2))
#define SQUARE_H3 (square_of(FILE_H, RANK_3))
#define SQUARE_H4 (square_of(FILE_H, RANK_4))
#define SQUARE_H5 (square_of(FILE_H, RANK_5))
#define SQUARE_H6 (square_of(FILE_H, RANK_6))
#define SQUARE_H7 (square_of(FILE_H, RANK_7))
#define SQUARE_H8 (square_of(FILE_H, RANK_8))

#endif