#pragma once
#include "vectormap.h"

typedef struct VectorMultiMap VectorMultiMap;

VectorMultiMap *NewVectorMultiMap(int n, int capacity, int sensitivity);
void DestroyVectorMultiMap(VectorMultiMap *vmm);

void VectorMultiMap_Inc(VectorMultiMap *vmm, Vector vec);
int *VectorMultiMap_Get(VectorMultiMap *vmm, Vector vec);

#ifdef TEST
void TEST_VectorMultiMap();
#endif
