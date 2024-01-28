#include "matrix.h"
#include <stdlib.h>

Matrix NewMatrix(int size, int n, int m)
{
    void **ret = calloc(n, sizeof(void *));
    *ret = calloc(size, n * m);

    for (int j = 0; j < n; j++)
    {
        ret[j] = (char *)(*ret) + j * size * m;
    }

    return ret;
}

void DestroyMatrix(Matrix m)
{
    free(*(char **)m);
    free((void *)m);
}

#ifdef TEST
#include "test.h"
void TEST_matrix()
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