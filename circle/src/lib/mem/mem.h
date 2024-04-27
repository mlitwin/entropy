#pragma once

#include <stdlib.h>

void *mem_calloc(size_t count, size_t size);
void *mem_malloc(size_t size);
void *mem_realloc(void *ptr, size_t size);