#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int n;
    int v;
    Matrix cur;
    Matrix next;

    Matrix a;
    Matrix b;
} World;

void CreateNeWorld(World *w, int n, int v)
{
    w->n = n;
    w->v = v;
    w->a = NewMatrix(v, n);
    w->b = NewMatrix(v, n);
    w->cur = w->a;
    w->next = w->b;
}

void DestroyWorld(World *w)
{
    DestroyMatrix(w->a, w->v);
    DestroyMatrix(w->b, w->v);
}

void AdvanceWorld(World *w)
{
    for (int j = 0; j < w->v; j++)
    {
        int *cur_row = w->cur[j];
        int *next_row = w->next[j];
        for (int i = 0; i < w->n; i++)
        {
            int nextI = (i + j) % w->n;
            next_row[nextI] = cur_row[i];
            cur_row[i] = 0;
        }
    }
    Matrix tmp = w->cur;
    w->cur = w->next;
    w->next = tmp;
}

int main(int argc, char *argv[])
{
    World w;
    CreateNeWorld(&w, 10000, 5);
    printf("Hello World\n");

    for (int t = 0; t < w.n; t++)
    {
        AdvanceWorld(&w);
    }
    DestroyWorld(&w);

    return 0;
}