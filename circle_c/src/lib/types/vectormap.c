#include "vectormap.h"
#include "hashmap.h"
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
    int sensitivity;
    struct hashmap *h;
    struct mapNode *root;
};

static int vec_cmp(const void *a, const void *b, void *udata)
{
    int *v1 = a;
    int n1 = *(v1 - 1);
    int *v2 = b;
    int n2 = *(v2 - 1);

    int i = 0;

    while (i < n1 && i < n2)
    {
        int cmp = v2[i] - v1[i];
        if (0 != cmp)
        {
            return cmp;
        }
        i++;
    }

    return n2 - n1;
}

static uint64_t hash_vec(const void *item, uint64_t seed0, uint64_t seed1)
{
    int *v = item;
    return hashmap_sip(v, *(v - 1) * sizeof(int), seed0, seed1);
}

VectorMap *
NewVectorMap(int sensitivity)
{
    VectorMap *v = calloc(sizeof(VectorMap), 1);
    v->sensitivity = sensitivity;
    v->root = NewMapNode();
    v->h = hashmap_new(sizeof(int *), 0, 0, 0, &hash_vec, vec_cmp, free, NULL);
    return v;
}
void DestroyVectorMap(VectorMap *v)
{
    FreeMapNode(v->root);
    free(v);
}

VectorMap ***NewMatrixOfVectorMap(int n, int m)
{
    VectorMap ***ret = calloc(n, sizeof(VectorMap **));
    **ret = calloc(n * m, sizeof(VectorMap *));

    for (int i = 0; i < n; i++)
    {
        ret[i] = *ret + i * m;
        for (int j = 0; j < m; j++)
        {
            ret[i][j] = NewVectorMap(j + 1);
        }
    }

    return ret;
}

void DestroyMatrixOfVectorMap(VectorMap **vm)
{
    free(*vm);
    free(vm);
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
    VectorMap *v = NewVectorMap(1000);
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
