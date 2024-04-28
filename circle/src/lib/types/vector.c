#include "vector.h"
#include "../mem/mem.h"

#include <stdlib.h>

int64_t *NewVector(int64_t capacity)
{
    int64_t *vec = mem_malloc((capacity + 2) * sizeof(int64_t));
    vec[1] = capacity;

    return vec + 2;
}

void DestroyVector(int64_t *v)
{
    int64_t *vec = v - 2;
    free(vec);
}

int Vector_GetLen(int64_t *v)
{
    int64_t *vec = v - 2;

    return *vec;
}

void Vector_Push(int64_t **v, const int64_t a)
{
    int64_t *vec = *v - 2;
    const int len = *vec;
    int64_t capacity = *(vec + 1);

    if (len == capacity)
    {
        capacity *= 2;
        vec = mem_realloc(vec, (capacity + 2) * sizeof(int64_t));
        *(vec + 1) = capacity;
        *v = vec + 2;
    }
    (*v)[len] = a;
    (*vec)++;
}

int64_t Vector_Get(const int64_t *v, int index)
{
    const int64_t *vec = v - 2;
    const int len = *vec;
    return (index < len) ? *(v + index) : 0;
}

#ifdef TEST
#include "test.h"
void TEST_Vector()
{
    int64_t *v = NewVector(2);
    Vector_Push(&v, 2);
    if (*v != 2)
    {
        FAIL("Can't push 2 to vector got %lld", *v);
    }
    Vector_Push(&v, 3);
    Vector_Push(&v, 4);

    if (v[2] != 4)
    {
        FAIL("Can't grow vector got %lld", v[2]);
    }

    DestroyVector(v);
}
#endif