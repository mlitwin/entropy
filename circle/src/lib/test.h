#pragma once

#ifdef TEST
#include <stdio.h>
#define FAIL(...)        \
    printf(__VA_ARGS__); \
    printf("\n");        \
    exit(-1);
#endif