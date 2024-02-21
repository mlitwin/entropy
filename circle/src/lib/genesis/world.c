#include "world.h"
#include "../types/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

static void incrementVectorMaps(World *w)
{
    return;
    for (int i = 0; i < w->n; i++)
    {
        VectorMap_Get(w->vm[i], w->densities[w->t])
            ->value++;
    }
}

static void declareDarkMaterials(World *w)
{
    for (int j = 0; j < w->v; j++)
    {
        for (int i = 0; i < w->n; i++)
        {
            const int val = rand() % w->v;
            w->cur[j][i] = val;
            w->densities[0][i] += val;
        }
    }
    incrementVectorMaps(w);
}

void CreateNeWorld(World *w, int n, int v, int precision)
{
    v *= 2;

    w->n = n;
    w->v = v;
    w->t = 0;
    w->precision = precision;

    w->a = (int **)NewMatrix(sizeof(int), v, n);
    w->b = (int **)NewMatrix(sizeof(int), v, n);
    w->densities = calloc(sizeof(Vector *), w->n);
    for (int i = 0; i < w->n; i++)
    {
        w->densities[i] = NewVector(w->n);
    }
    w->cur = w->a;
    w->next = w->b;
    w->vm = calloc(sizeof(VectorMap *), w->n);
    for (int i = 0; i < w->n; i++)
    {
        w->vm[i] = NewVectorMap(n, n, i + 1);
    }

    declareDarkMaterials(w);
}

void DestroyWorld(World *w)
{
    DestroyMatrix((void **)w->a);
    DestroyMatrix((void **)w->b);
    for (int i = 0; i < w->n; i++)
    {
        DestroyVector(w->densities[i]);
    }
    free(w->densities);
}

void AdvanceWorld(World *w)
{
    w->t++;

    for (int j = 0; j < w->v; j++)
    {
        int *cur_row = w->cur[j];
        int *next_row = w->next[j];
        const int velocity = j - w->v / 2;

        for (int i = 0; i < w->n; i++)
        {
            int curVal = cur_row[i];
            int nextI = (i + velocity) % w->n;
            if (nextI < 0)
            {
                nextI += w->n;
            }

            next_row[nextI] = curVal;

            w->densities[w->t][nextI / w->precision] += curVal;

            cur_row[i] = 0;
        }
    }

    incrementVectorMaps(w);

    int **tmp = w->cur;
    w->cur = w->next;
    w->next = tmp;
}

void PrintWorld(const World *w)
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
    World w;
    sranddev();
    CreateNeWorld(&w, 10, 10, 1);
    AdvanceWorld(&w);
    DestroyWorld(&w);
}
#endif