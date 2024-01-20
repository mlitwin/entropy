#include "vectormap.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_MAP_LEN (32)

struct mapNode
{
    VectorValue value;
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
    struct mapNode *root;
};

VectorMap *
NewVectorMap()
{
    VectorMap *v = calloc(sizeof(VectorMap), 1);
    v->root = NewMapNode();
    return v;
}
void DestroyVectorMap(VectorMap *v)
{
    FreeMapNode(v->root);
    free(v);
}

static VectorValue *getValue(VectorMap *vm, int *vec, int n)
{
    struct mapNode *cur = vm->root;
    for (int i = 0; i < n; i++)
    {
        const int index = vec[i];
        ensureNext(cur, index);
        if (!cur->next[index])
        {
            cur->next[index] = NewMapNode();
        }
        cur = cur->next[index];
    }

    return &cur->value;
}

VectorValue *VectorMap_Get(VectorMap *vm, int *vec, int n)
{
    return getValue(vm, vec, n);
}

#ifdef TEST
#include "test.h"
void TEST_VectorMap()
{
    int vec[] = {1, 2, 3, 4};
    VectorMap *v = NewVectorMap();
    VectorValue *val = VectorMap_Get(v, vec, 4);
    val = VectorMap_Get(v, vec, 4);
    if (0 != val->value)
    {
        FAIL("No VectorMap 0 init got %d", val->value);
    }
    val->value = 7;
    val = VectorMap_Get(v, vec, 4);

    if (7 != val->value)
    {
        FAIL("VectorMap setting not working got %d", val->value);
    }
    DestroyVectorMap(v);
}
#endif
