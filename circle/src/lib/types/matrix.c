#include "matrix.h"
#include "../mem/mem.h"

#include <stdlib.h>

void **NewMatrix(int size, int m, int n)
{
    void **ret = mem_calloc(m, sizeof(void *));
    *ret = mem_calloc(size, m * n);

    for (int j = 0; j < m; j++)
    {
        ret[j] = (char *)(*ret) + j * size * n;
    }

    return ret;
}

void DestroyMatrix(void **m)
{
    free(*(char **)m);
    free((void *)m);
}

#ifdef TEST
#include "test.h"
void TEST_Matrix()
{
    int **m = (int **)NewMatrix(sizeof(int), 20, 30);
    m[4][7] = 22;

    if (m[4][7] != 22)
    {
        FAIL("Can't set matrix 22 != %d", m[4][7]);
    }

    DestroyMatrix((void **)m);
}
#endif