#ifndef _FILE_H
#define _FILE_H

#include <stdint.h>

typedef int8_t file_t;

#define FILE_COUNT 8

#define FILE_A ((file_t)0)
#define FILE_B ((file_t)1)
#define FILE_C ((file_t)2)
#define FILE_D ((file_t)3)
#define FILE_E ((file_t)4)
#define FILE_F ((file_t)5)
#define FILE_G ((file_t)6)
#define FILE_H ((file_t)7)

#define FILE(c) (c - 'a')
#define FILE_SYMBOL(f) (f + 'a')

#endif