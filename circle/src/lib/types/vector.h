#pragma once

typedef int *Vector;

Vector NewVector(int n);
void DestroyVector(Vector v);

struct Vector_CmpArgs
{
    int aLen;
    int bLen;
    int sensitivity;
};

int Vector_Cmp(struct Vector_CmpArgs *args, const Vector a, const Vector);

#ifdef TEST
void TEST_Vector();
#endif
