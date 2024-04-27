#include "vector.h"
#include "../mem/mem.h"

#include <stdlib.h>

int *NewVector(int capacity)
{
    int *vec = mem_malloc((capacity + 2) * sizeof(int));
    vec[1] = capacity;

    return vec + 2;
}

void DestroyVector(int *v)
{
    int *vec = v - 2;
    free(vec);
}

int Vector_GetLen(int *v)
{
    int *vec = v - 2;

    return *vec;
}

void Vector_Push(int **v, const int a)
{
    int *vec = *v - 2;
    const int len = *vec;
    int capacity = *(vec + 1);

    if (len == capacity)
    {
        capacity *= 2;
        vec = mem_realloc(vec, (capacity + 2) * sizeof(int));
        *(vec + 1) = capacity;
        *v = vec + 2;
    }
    (*v)[len] = a;
    (*vec)++;
}

#ifdef TEST
#include "test.h"
void TEST_Vector()
{
    int *v = NewVector(2);
    Vector_Push(&v, 2);
    if (*v != 2)
    {
        FAIL("Can't push 2 to vector got %d", *v);
    }
    Vector_Push(&v, 3);
    Vector_Push(&v, 4);

    if (v[2] != 4)
    {
        FAIL("Can't grow vector got %d", v[2]);
    }

    DestroyVector(v);
}
#endif