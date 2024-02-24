#include "world.h"
#include "../types/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

struct densityEntry
{
    int t;
    int *v;
};

struct World
{
    int n;
    int v;
    int precision;
    int **cur;
    int t;
    int **densities;
    int **permutation;

    int **next;
    int **a;
    int **b;
};

static void declareDarkMaterials(struct World *w)
{
    for (int j = 0; j < w->v; j++)
    {
        const int velocity = j - w->v / 2;

        for (int i = 0; i < w->n; i++)
        {
            int nextI = (i + velocity) % w->n;
            const int val = rand() % w->v;

            w->cur[j][i] = val;
            w->densities[0][i] += val;

            if (nextI < 0)
            {
                nextI += w->n;
            }
            w->permutation[j][i] = nextI;
        }
    }
}

struct World *CreateNeWorld(int n, int v, int precision)
{
    struct World *w = calloc(sizeof(struct World), 1);

    v *= 2;

    w->n = n;
    w->v = v;
    w->t = 0;
    w->precision = precision;

    w->a = (int **)NewMatrix(sizeof(int), v, n);
    w->b = (int **)NewMatrix(sizeof(int), v, n);
    w->permutation = (int **)NewMatrix(sizeof(int), v, n);

    w->densities = (int **)NewMatrix(sizeof(int), n, n);

    w->cur = w->a;
    w->next = w->b;

    declareDarkMaterials(w);

    return w;
}

void DestroyWorld(struct World *w)
{
    DestroyMatrix((void **)w->a);
    DestroyMatrix((void **)w->b);
    DestroyMatrix((void **)w->permutation);
    DestroyMatrix((void **)w->densities);
    free(w);
}

static void AdvanceWorld(struct World *w)
{
    w->t++;

    for (int j = 0; j < w->v; j++)
    {
        int *cur_row = w->cur[j];
        int *next_row = w->next[j];

        for (int i = 0; i < w->n; i++)
        {
            const int curVal = cur_row[i];
            const int nextI = w->permutation[j][i];

            next_row[nextI] = curVal;

            w->densities[w->t][nextI / w->precision] += curVal;

            cur_row[i] = 0;
        }
    }

    int **tmp = w->cur;
    w->cur = w->next;
    w->next = tmp;
}

void RunWorld(struct World *w)
{
    PrintWorld(w);
    while (w->t < w->n - 1)
    {
        AdvanceWorld(w);
        PrintWorld(w);
    }
}

static int densityCmp(void *thunk, const void *iA, const void *iB)
{
    struct World *w = thunk;
    const struct densityEntry *A = iA;
    const struct densityEntry *B = iB;
    int *a = A->v;
    int *b = B->v;

    int n = w->n;

    while (n != 0)
    {
        const int diff = (*a) - (*b);
        if (diff != 0)
        {
            return diff;
        }
        a++;
        b++;
        n--;
    }

    return 0;
}

static void PrintVector(const int *v, int n)
{
    for (int i = 0; i < n; i++)
    {
        const char *sep = (i == 0) ? "(" : " ";
        printf("%s%d", sep, v[i]);
    }
    printf(")\n");
}

void BeholdWorld(struct World *w)
{
    struct densityEntry *densities = calloc(sizeof(struct densityEntry *), w->n);

    for (int t = 0; t < w->n; t++)
    {
        densities[t].t = t;
        densities[t].v = w->densities[t];
    }

    printf("\nBehold\n");

    qsort_r(densities, w->n, sizeof(struct densityEntry), w, densityCmp);

    for (int t = 0; t < w->n; t++)
    {
        PrintVector(densities[t].v, w->n);
    }

    free(densities);
}

void PrintWorld(const struct World *w)
{
    for (int i = 0; i < w->n; i++)
    {
        const char *sep = (i == 0) ? "(" : " ";
        printf("%s%d", sep, w->densities[w->t][i]);
    }
    printf(")\n");
}

#ifdef TEST
#include "test.h"
void TEST_World()
{
    struct World *w;
    const int n = 100;

    srand(1);
    w = CreateNeWorld(n, 3, 1);
    RunWorld(w);
    BeholdWorld(w);
    DestroyWorld(w);
}
#endif