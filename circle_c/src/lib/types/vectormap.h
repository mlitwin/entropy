#pragma once
#include "vector.h"

typedef struct VectorMap VectorMap;

typedef union
{
    int value;
    void *ptr;
} VectorValue;

VectorMap *NewVectorMap(int capacity, int sensitivity);
void DestroyVectorMap(VectorMap *v);
void VectorMap_Set(VectorMap *vm, Vector vec, VectorValue val);
int VectorMap_Inc(VectorMap *vm, Vector vec, int inc);

VectorValue VectorMap_Get(VectorMap *vm, Vector vec);

VectorMap ***NewMatrixOfVectorMap(int capacity, int n, int m);

#ifdef TEST
void TEST_VectorMap();
#endif
