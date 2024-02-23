#include "vector.h"
#include <stdlib.h>

Vector NewVector(int n)
{
    Vector v = calloc(n, sizeof(int));
    return v;
}
void DestroyVector(Vector v)
{
    free(v);
}

int Vector_Cmp(struct Vector_CmpArgs *args, const Vector av, const Vector bv)
{
    Vector a = av;
    Vector b = bv;

    int n = args->aLen < args->bLen ? args->aLen : args->bLen;

    while (n != 0)
    {
        const int diff = (*a) / args->sensitivity - (*b) / args->sensitivity;
        if (diff != 0)
        {
            return diff;
        }
        a++;
        b++;
        n--;
    }

    return args->aLen - args->bLen;
}

#ifdef TEST
#include "test.h"
void TEST_Vector()
{
    int *v = NewVector(10);
    DestroyVector(v);
}
#endif
