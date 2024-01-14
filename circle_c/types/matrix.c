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