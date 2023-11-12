#ifndef SIDE_H
#define SIDE_H

typedef enum side
{
    SIDE_WHITE = 64,
    SIDE_BLACK = 128,
} side_t;

#define SIDE_MASK (SIDE_WHITE | SIDE_BLACK)

#endif