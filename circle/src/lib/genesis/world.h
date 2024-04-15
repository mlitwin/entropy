#pragma once

#include "../types/matrix.h"

struct WorldSpec
{
    int n;
    int period;
    int v;
    int density;
    int precision;
    int sensitivity;
};

struct WorldView
{
    int **densities;
    int **probabilities;
    int **cohorts;
    int **states;
    int *num_states;
};

struct World
{
    struct WorldSpec s;
    struct WorldView v;

    int num_v;

    int t;

    int **cur;
};

struct World;

struct World *CreateNeWorld(int n, int v, int density, int precision);

void DestroyWorld(struct World *w);

void RunWorld(struct World *w);
void BeholdWorld(struct World *w);

void PrintWorld(const struct World *w);

#ifdef TEST
void TEST_World();
#endif