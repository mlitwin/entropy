#pragma once

#include "../types/matrix.h"
#include "../types/vectormap.h"

typedef struct
{
    int n;
    int v;
    int precision;
    int sensitivity;
    Matrix cur;
    int *densities;
    VectorMap **vm;

    Matrix next;
    Matrix a;
    Matrix b;
} World;

void CreateNeWorld(World *w, int n, int v, int precision, int sensitivity);

void DestroyWorld(World *w);

void AdvanceWorld(World *w);

void PrintWorld(const World *w);

#ifdef TEST
void TEST_World();
#endif