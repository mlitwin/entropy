#include "vector.h"
#include <stdlib.h>

Vector NewVector(int n)
{
    Vector v = calloc(n + 1, sizeof(int));
    *v = n;
    return v + 1;
}
void DestroyVector(Vector v)
{
    free(v - 1);
}

#if 0
static int vec_cmp(const void *a, const void *b, void *udata)
{
    int *v1 = a;
    int n1 = *(v1 - 1);
    int *v2 = b;
    int n2 = *(v2 - 1);

    int i = 0;

    while (i < n1 && i < n2)
    {
        int cmp = v2[i] - v1[i];
        if (0 != cmp)
        {
            return cmp;
        }
        i++;
    }

    return n2 - n1;
}

static uint64_t hash_vec(const void *item, uint64_t seed0, uint64_t seed1)
{
    int *v = item;
    return hashmap_sip(v, *(v - 1) * sizeof(int), seed0, seed1);
}
#endif

#ifdef TEST
#include "test.h"
void TEST_Vector()
{
    int *v = NewVector(10);
    DestroyVector(v);
}
#endif
