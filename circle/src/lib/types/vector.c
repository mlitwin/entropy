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

#ifdef TEST
#include "test.h"
void TEST_Vector()
{
    int *v = NewVector(10);
    DestroyVector(v);
}
#endif
