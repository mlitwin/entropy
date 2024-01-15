#pragma once

#include "../types/matrix.h"

typedef struct
{
    int n;
    int v;
    Matrix cur;
    int *observation;

    Matrix next;
    Matrix a;
    Matrix b;
} World;

void CreateNeWorld(World *w, int n, int v);

void DestroyWorld(World *w);

void AdvanceWorld(World *w);

void PrintWorld(const World *w);