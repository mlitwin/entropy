#include "vectormultimap.h"

#include <stdlib.h>

struct VectorMultiMap
{
    int n;
    int capacity;
    int sensitivity;
    int *values;
    VectorMap **maps;
};

VectorMultiMap *NewVectorMultiMap(int n, int capacity, int sensitivity)
{
    struct VectorMultiMap *vmm = calloc(sizeof(VectorMultiMap), 1);

    vmm->n = n;
    vmm->capacity = capacity;
    vmm->sensitivity = sensitivity;

    vmm->values = calloc(sizeof(int), sensitivity);
    vmm->maps = calloc(sizeof(VectorMap *), sensitivity);

    for (int i = 0; i < sensitivity; i++)
    {
        vmm->maps[i] = NewVectorMap(n, capacity, i + 1);
    }
    return vmm;
}
void DestroyVectorMultiMap(VectorMultiMap *vmm)
{
    for (int i = 0; i < vmm->sensitivity; i++)
    {
        DestroyVectorMap(vmm->maps[i]);
    }
    free(vmm->maps);
    free(vmm->values);
    free(vmm);
}

void VectorMultiMap_Inc(VectorMultiMap *vmm, Vector vec)
{
}
int *VectorMultiMap_Get(VectorMultiMap *vmm, Vector vec)
{
    return 0;
}

#ifdef TEST
#include "test.h"

void TEST_VectorMultiMap()
{
}
#endif
