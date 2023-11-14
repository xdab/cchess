#ifndef RANDOM_H
#define RANDOM_H

/**
 * @brief Gets the next random number.
 */
unsigned long random_next();

/**
 * @brief Equivalent to 2^128 calls to next()
 */
void random_jump();

/**
 * @brief Equivalent to 2^192 calls to next()
 */
void random_long_jump();

#endif