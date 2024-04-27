#include "mem.h"

#include <stdio.h>

void *mem_calloc(size_t count, size_t size)
{
    void *ret = calloc(count, size);
    if (ret == 0)
    {
        fprintf(stderr, "Can't calloc %zu of size %zu", count, size);
        exit(137);
    }

    return ret;
}

void *mem_malloc(size_t size)
{
    void *ret = malloc(size);
    if (ret == 0)
    {
        fprintf(stderr, "Can't malloc size %zu", size);
        exit(137);
    }

    return ret;
}

void *mem_realloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (ret == 0)
    {
        fprintf(stderr, "Can't realloc size %zu", size);
        exit(137);
    }

    return ret;
}