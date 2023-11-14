#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdio.h>
#include <stdlib.h>

static int _error;

void test_init()
{
	_error = 0;
}

void test_error(const char *message)
{
	fprintf(stderr, "%s\n", message);
	_error = 1;
}

void test_exit()
{
	exit(_error ? EXIT_FAILURE : EXIT_SUCCESS);
}

#endif