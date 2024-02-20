#pragma once

#include "../types/matrix.h"
#include "../types/vectormap.h"
#include "../types/vector.h"

typedef struct
{
    int n;
    int v;
    int precision;
    int **cur;
    int t;
    Vector *densities;
    VectorMap **vm;

    int **next;
    int **a;
    int **b;
} World;

void CreateNeWorld(World *w, int n, int v, int precision);

void DestroyWorld(World *w);

void AdvanceWorld(World *w);

void PrintWorld(const World *w);

#ifdef TEST
void TEST_World();
#endif