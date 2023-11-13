#ifndef SQUARE_H
#define SQUARE_H

#include <stdbool.h>

typedef unsigned char square_t;

#define SQUARE_NULL (0xFF)

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

#endif