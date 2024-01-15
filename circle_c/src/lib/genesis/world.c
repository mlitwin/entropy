#include "world.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

static void declareDarkMaterials(World *w)
{
    for (int j = 0; j < w->v; j++)
    {
        for (int i = 0; i < w->n; i++)
        {
            const int val = rand() % 3;
            w->cur[j][i] = val;
            w->observation[i] += val;
        }
    }
}

void CreateNeWorld(World *w, int n, int v)
{
    v *= 2;

    w->n = n;
    w->v = v;
    w->a = NewMatrix(v, n);
    w->b = NewMatrix(v, n);
    w->observation = calloc(n, sizeof(int));
    w->cur = w->a;
    w->next = w->b;

    declareDarkMaterials(w);
}

void DestroyWorld(World *w)
{
    DestroyMatrix(w->a, w->v);
    DestroyMatrix(w->b, w->v);
}

void AdvanceWorld(World *w)
{
    bzero(w->observation, w->n * sizeof(int));

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
            w->observation[nextI] += curVal;
            cur_row[i] = 0;
        }
    }
    Matrix tmp = w->cur;
    w->cur = w->next;
    w->next = tmp;
}

void PrintWorld(const World *w)
{
    for (int i = 0; i < w->n; i++)
    {
        const char *sep = (i == 0) ? "(" : " ";
        printf("%s%d", sep, w->observation[i]);
    }
    printf(")\n");
}