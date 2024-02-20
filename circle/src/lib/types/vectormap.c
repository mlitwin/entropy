#include "vectormap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int Vector_Equal(int n, int sensitivity, Vector a, Vector b)
{
    while (n > 0)
    {
        if ((*a) / sensitivity != (*b) / sensitivity)
        {
            return 0;
        }
        a++;
        b++;
        n--;
    }

    return 1;
}

#define INITIAL_MAP_LEN (32)

/*
static int mod(int i, int n)
{
    return (i % n + n) % n;
}
*/

struct mapNode
{
    int valueIndex;
    Vector v;
    struct mapNode **next;
    int nextLen;
};

static struct mapNode *NewMapNode()
{
    struct mapNode *node = calloc(sizeof(struct mapNode), 1);
    node->next = calloc(sizeof(struct mapNode *), INITIAL_MAP_LEN);
    node->nextLen = INITIAL_MAP_LEN;
    return node;
}

static void FreeMapNode(struct mapNode *node)
{
    for (int i = 0; i < node->nextLen; i++)
    {
        if (node->next[i])
        {
            FreeMapNode(node->next[i]);
        }
    }
    free(node->next);
    free(node);
}

static void ensureNext(struct mapNode *node, int n)
{
    int len = node->nextLen;
    while (len <= n)
    {
        len *= 2;
    }
    if (len != node->nextLen)
    {
        char *ptr;
        node->next = realloc(node->next, sizeof(struct mapNode) * len);
        ptr = (char *)node->next;
        bzero(ptr + node->nextLen, len - node->nextLen);
        node->nextLen = len;
    }
}

struct VectorMap
{
    int capacity;
    int sensitivity;
    VectorValue *values;
    int nextValue;
    struct mapNode *root;
};

VectorMap *
NewVectorMap(int capacity, int sensitivity)
{
    VectorMap *v = calloc(sizeof(VectorMap), 1);

    v->capacity = capacity;
    v->sensitivity = sensitivity;
    v->values = calloc(sizeof(VectorValue), capacity);
    v->root = NewMapNode();
    return v;
}
void DestroyVectorMap(VectorMap *v)
{
    FreeMapNode(v->root);
    free(v);
}

VectorMap ***NewMatrixOfVectorMap(int capacity, int n, int m)
{
    VectorMap ***ret = calloc(n, sizeof(VectorMap **));
    **ret = calloc(n * m, sizeof(VectorMap *));

    for (int i = 0; i < n; i++)
    {
        ret[i] = *ret + i * m;
        for (int j = 0; j < m; j++)
        {
            ret[i][j] = NewVectorMap(capacity, j + 1);
        }
    }

    return ret;
}

void DestroyMatrixOfVectorMap(VectorMap **vm)
{
    free(*vm);
    free(vm);
}

static int getIndexFromNode(VectorMap *vm, struct mapNode *node)
{
    int valueIndex = node->valueIndex;
    if (valueIndex == 0)
    {
        valueIndex = vm->nextValue + 1;
        node->valueIndex = valueIndex;
        vm->nextValue++;
    }
    return valueIndex;
}

static int getIndex(VectorMap *vm, Vector vec, int n)
{
    struct mapNode *cur = vm->root;

    for (int i = 0; i < n; i++)
    {
        if (!cur->v)
        {
            cur->v = vec;
            break;
        }
        if (Vector_Equal(n - i, vm->sensitivity, vec + i, cur->v + i))
        {
            break;
        }

        const int index = vec[i] / vm->sensitivity;
        ensureNext(cur, index);
        if (!cur->next[index])
        {
            cur->next[index] = NewMapNode();
        }
        cur = cur->next[index];
    }

    return getIndexFromNode(vm, cur);
}

static VectorValue *getValue(VectorMap *vm, Vector vec, int n)
{
    const int valueIndex = getIndex(vm, vec, n);

    return &vm->values[valueIndex - 1];
}

VectorValueRef VectorMap_GetRef(VectorMap *vm, Vector vec)
{
    return getIndex(vm, vec, *(vec - 1));
}
VectorValue VectorMap_GetFromRef(VectorMap *vm, VectorValueRef ref)
{
    return vm->values[ref - 1];
}

VectorValue VectorMap_Get(VectorMap *vm, Vector vec)
{
    return *getValue(vm, vec, *(vec - 1));
}

void VectorMap_Set(VectorMap *vm, Vector vec, VectorValue val)
{
    VectorValue *v = getValue(vm, vec, *(vec - 1));
    *v = val;
}

int VectorMap_PreInc(VectorMap *vm, Vector vec, int inc)
{
    VectorValue *v = getValue(vm, vec, *(vec - 1));
    v->value += inc;

    return v->value;
}

#ifdef TEST
#include "test.h"

static void testGetSet(VectorMap *v, Vector vec)
{
    VectorValue val = VectorMap_Get(v, vec);
    if (0 != val.value)
    {
        FAIL("No VectorMap 0 init got %d", val.value);
    }
    VectorMap_PreInc(v, vec, 7);

    val = VectorMap_Get(v, vec);

    if (7 != val.value)
    {
        FAIL("VectorMap setting not working got %d", val.value);
    }
}

static void
testAMap(int capacity, int sensitivity)
{
    int testCases[] =
        {1, 3, 4, 5,
         1, 2, 4, 5};

    VectorMap *v = NewVectorMap(capacity, sensitivity);
    int numCases = 2;
    for (int i = 0; i < numCases; i++)
    {
        Vector vec = NewVector(4);
        for (int j = 0; j < 4; j++)
        {
            vec[j] = testCases[4 * i + j];
        }
        testGetSet(v, vec);
    }

    DestroyVectorMap(v);
}

void TEST_VectorMap()
{
    testAMap(10, 1);
    // testAMap(10, 2);
    // testAMap(10, 10);
}
#endif
