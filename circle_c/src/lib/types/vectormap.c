#include "vectormap.h"
#include <stdlib.h>
#include <string.h>

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

static VectorValue *getValueFromNode(VectorMap *vm, struct mapNode *node)
{
    int valueIndex = node->valueIndex;
    if (valueIndex == 0)
    {
        valueIndex = vm->nextValue + 1;
        node->valueIndex = valueIndex;
        vm->nextValue++;
    }
    return &vm->values[valueIndex - 1];
}

static VectorValue *getValue(VectorMap *vm, int *vec, int n)
{
    struct mapNode *cur = vm->root;
    for (int i = 0; i < n; i++)
    {
        const int index = vec[i] / vm->sensitivity;
        ensureNext(cur, index);
        if (!cur->next[index])
        {
            cur->next[index] = NewMapNode();
        }
        cur = cur->next[index];
    }
    return getValueFromNode(vm, cur);
}

VectorValue *VectorMap_Get(VectorMap *vm, Vector vec)
{
    return getValue(vm, vec, *(vec - 1));
}

#ifdef TEST
#include "test.h"
void TEST_VectorMap()
{
    int vec[] = {1, 2, 3, 4};
    VectorMap *v = NewVectorMap(1, 1);
    VectorValue *val = VectorMap_Get(v, vec);
    val = VectorMap_Get(v, vec);
    if (0 != val->value)
    {
        FAIL("No VectorMap 0 init got %d", val->value);
    }
    val->value = 7;
    val = VectorMap_Get(v, vec);

    if (7 != val->value)
    {
        FAIL("VectorMap setting not working got %d", val->value);
    }
    DestroyVectorMap(v);
}
#endif
