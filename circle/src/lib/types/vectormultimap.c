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
        int numValues;
        VectorValue *values = VectorMap_GetValues(vmm->maps[i], &numValues);
        for (int j = 1; j < numValues; j++)
        {
            free(values[j].ptr);
        }
        DestroyVectorMap(vmm->maps[i]);
    }
    free(vmm->maps);
    free(vmm->values);
    free(vmm);
}

static VectorValueRef *GetValueRefs(VectorMultiMap *vmm, Vector vec)
{
    VectorValue *v = VectorMap_Get(vmm->maps[0], vec);
    VectorValueRef *refs = v->ptr;

    if (refs == 0)
    {
        refs = calloc(sizeof(VectorValueRef), vmm->sensitivity);
        v->ptr = refs;
        for (int i = 1; i < vmm->sensitivity; i++)
        {
            refs[i] = VectorMap_GetRef(vmm->maps[i], vec);
        }
    }

    return refs;
}

void VectorMultiMap_Inc(VectorMultiMap *vmm, Vector vec, int inc)
{
    VectorValueRef *refs = GetValueRefs(vmm, vec);

    refs[0] += inc;
    for (int i = 1; i < vmm->sensitivity; i++)
    {
        VectorValue *v = VectorMap_GetFromRef(vmm->maps[i], refs[i]);
        v->value += inc;
    }
}

int *VectorMultiMap_Get(VectorMultiMap *vmm, Vector vec)
{
    VectorValueRef *refs = GetValueRefs(vmm, vec);

    vmm->values[0] = refs[0];
    for (int i = 1; i < vmm->sensitivity; i++)
    {
        VectorValue *v = VectorMap_GetFromRef(vmm->maps[i], refs[i]);
        vmm->values[i] = v->value;
    }

    return vmm->values;
}

#ifdef TEST
#include "test.h"

void TEST_VectorMultiMap()
{
    VectorMultiMap *vmm = NewVectorMultiMap(10, 10, 10);
    int vec[] = {1, 3, 4, 5, 6, 7, 8, 9, 10};
    int *vals;

    VectorMultiMap_Inc(vmm, vec, 7);

    vals = VectorMultiMap_Get(vmm, vec);

    if (7 != vals[0])
    {
        FAIL("VectorMap inc[0] not working got %d", vals[0]);
    }

    if (7 != vals[9])
    {
        FAIL("VectorMap inc[9] not working got %d", vals[0]);
    }

    DestroyVectorMultiMap(vmm);
}
#endif
