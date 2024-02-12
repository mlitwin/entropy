#pragma once
#include "vector.h"

typedef struct VectorMap VectorMap;

typedef union
{
    int value;
    void *ptr;
} VectorValue;

VectorMap *NewVectorMap(int sensitivity);
void DestroyVectorMap(VectorMap *v);

VectorValue *VectorMap_Get(VectorMap *vm, Vector vec);

VectorMap ***NewMatrixOfVectorMap(int n, int m);

#ifdef TEST
void TEST_VectorMap();
#endif
