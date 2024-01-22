#pragma once

typedef struct VectorMap VectorMap;

typedef union
{
    int value;
    void *ptr;
} VectorValue;

VectorMap *NewVectorMap(int sensitivity);
void DestroyVectorMap(VectorMap *v);

VectorValue *VectorMap_Get(VectorMap *vm, int *vec, int n);

#ifdef TEST
void TEST_VectorMap();
#endif
