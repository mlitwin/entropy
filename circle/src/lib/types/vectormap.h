#pragma once
#include "vector.h"

typedef struct VectorMap VectorMap;

typedef union
{
    int value;
    void *ptr;
} VectorValue;

typedef int VectorValueRef;

VectorMap *NewVectorMap(int capacity, int sensitivity);
void DestroyVectorMap(VectorMap *v);
void VectorMap_Set(VectorMap *vm, Vector vec, VectorValue val);
int VectorMap_PreInc(VectorMap *vm, Vector vec, int inc);

VectorValue VectorMap_Get(VectorMap *vm, Vector vec);
VectorValueRef VectorMap_GetRef(VectorMap *vm, Vector vec);
VectorValue VectorMap_GetFromRef(VectorMap *vm, VectorValueRef ref);

VectorMap ***NewMatrixOfVectorMap(int capacity, int n, int m);

#ifdef TEST
void TEST_VectorMap();
#endif
