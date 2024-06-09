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
    const int v = 45;
    const int n = 90;
    int **m = (int **)NewMatrix(sizeof(int), v, n);
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // m[i][j] += 1;
        }
    }
    m[36][0] = 22;

    if (m[36][0] != 22)
    {
        FAIL("Can't set matrix 22 != %d", m[36][0]);
    }

    DestroyMatrix((void **)m);
}
#endif