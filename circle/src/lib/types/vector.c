#include "vector.h"
#include "../mem/mem.h"

#include <stdlib.h>
#include <string.h>

int64_t *NewVector(int64_t capacity)
{
    int64_t *vec = mem_calloc((capacity + 2), sizeof(int64_t));
    vec[1] = capacity;

    return vec + 2;
}

void DestroyVector(int64_t *v)
{
    int64_t *vec = v - 2;
    free(vec);
}

static int64_t *growVector(int64_t **v, int index)
{
    int64_t *vec = *v - 2;
    const int64_t capacity = *(vec + 1);
    int64_t new_capacity = capacity;
    if (new_capacity <= index)
    {
        while (new_capacity <= index)
        {
            new_capacity *= 2;
        }
        vec = mem_realloc(vec, (new_capacity + 2) * sizeof(int64_t));
        *(vec + 1) = new_capacity;
        *v = vec + 2;
        bzero((*v) + capacity, new_capacity - capacity);
    }

    return vec;
}

int Vector_GetLen(int64_t *v)
{
    int64_t *vec = v - 2;

    return *vec;
}

void Vector_Push(int64_t **v, const int64_t a)
{
    const int len = *((*v) - 2);
    int64_t *vec = growVector(v, len);

    (*v)[len] = a;
    (*vec)++;
}

int64_t Vector_Get(const int64_t *v, int index)
{
    const int64_t *vec = v - 2;
    const int len = *vec;
    return (index < len) ? *(v + index) : 0;
}

int64_t Vector_Increment(int64_t **v, int index, const int64_t a)
{
    int64_t *vec = growVector(v, index);
    int64_t cur = (*v)[index];
    (*v)[index] += a;
    if (*vec <= index)
    {
        *vec = index + 1;
    }
    return cur;
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

    Vector_Increment(&v, 77, 10);
    if (v[77] != 10)
    {
        FAIL("Can't increment vector got %lld", v[77]);
    }

    if (Vector_Get(v, 77) != 10)
    {
        FAIL("Can't vector get got %lld", v[77]);
    }

    DestroyVector(v);
}
#endif