#ifndef SIDE_H
#define SIDE_H

typedef enum side
{
    SIDE_WHITE = 0b01000000,
    SIDE_BLACK = 0b10000000,
} side_t;

#define SIDE_MASK (SIDE_WHITE | SIDE_BLACK)

#endif