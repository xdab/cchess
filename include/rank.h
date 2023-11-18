#ifndef RANK_H
#define RANK_H

#include <stdint.h>

typedef int8_t rank_t;

#define RANK_COUNT 8

#define RANK_1 ((rank_t)0)
#define RANK_2 ((rank_t)1)
#define RANK_3 ((rank_t)2)
#define RANK_4 ((rank_t)3)
#define RANK_5 ((rank_t)4)
#define RANK_6 ((rank_t)5)
#define RANK_7 ((rank_t)6)
#define RANK_8 ((rank_t)7)

#define RANK(c) (c - '1')
#define RANK_SYMBOL(r) (r + '1')

#endif