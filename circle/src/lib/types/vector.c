#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

int *NewVector(int n)
{
    int *v = calloc(n, sizeof(int));
    return v;
}
void DestroyVector(int *v)
{
    free(v);
}

void PrintVector(const int *v, int n)
{
    for (int i = 0; i < n; i++)
    {
        const char *sep = (i == 0) ? "(" : " ";
        printf("%s%d", sep, v[i]);
    }
    printf(")\n");
}

#ifdef TEST
#include "test.h"
void TEST_Vector()
{
    int *v = NewVector(10);
    DestroyVector(v);
}
#endif
