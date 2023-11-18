#ifndef SIDE_H
#define SIDE_H

typedef enum side
{
    WHITE = 0b01000000,
    BLACK = 0b10000000,
} side_t;

#define SIDE_MASK (WHITE | BLACK)

#endif