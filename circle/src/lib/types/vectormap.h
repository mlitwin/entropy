#pragma once
#include "vector.h"

typedef struct VectorMap VectorMap;

typedef union
{
    int value;
    void *ptr;
} VectorValue;

typedef int VectorValueRef;

VectorMap *NewVectorMap(int n, int capacity, int sensitivity);
void DestroyVectorMap(VectorMap *v);

VectorValue *VectorMap_Get(VectorMap *vm, Vector vec);

VectorValueRef VectorMap_GetRef(VectorMap *vm, Vector vec);
VectorValue *VectorMap_GetFromRef(VectorMap *vm, VectorValueRef ref);

VectorValue *VectorMap_GetValues(VectorMap *vm, int *oNumValues);

#ifdef TEST
void TEST_VectorMap();
#endif
