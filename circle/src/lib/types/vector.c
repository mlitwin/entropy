#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

Vector NewVector(int n)
{
    Vector v = calloc(n, sizeof(int));
    return v;
}
void DestroyVector(Vector v)
{
    free(v);
}

void PrintVector(const Vector v, int n)
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
