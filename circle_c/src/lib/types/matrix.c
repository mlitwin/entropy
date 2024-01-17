#include "matrix.h"
#include <stdlib.h>

int **NewMatrix(int n, int m)
{
    int **ret;

    ret = calloc(n, sizeof(int *));
    for (int j = 0; j < n; j++)
    {
        ret[j] = calloc(m, sizeof(int));
    }

    return ret;
}

void DestroyMatrix(Matrix M, int n)
{
    for (int j = 0; j < n; j++)
    {
        free(M[j]);
    }
}

#ifdef TEST
#include "test.h"
void TEST_matrix()
{
    int **m = NewMatrix(20, 30);
    m[4][7] = 22;

    if (m[4][7] != 22)
    {
        FAIL("Can't set matrix 22 != %d", m[4][7]);
    }

    DestroyMatrix(m, 20);
}
#endif