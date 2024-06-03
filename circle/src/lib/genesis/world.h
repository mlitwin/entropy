#pragma once

#include "worlddefs.h"

struct World
{
    struct WorldSpec s;
    struct WorldView v;

    int num_v;

    int t;

    int **cur;
};

struct World;

void InitWorldSpec(struct WorldSpec *ws, int n, int v, int density, int precision);

struct World *CreateNeWorld(const struct WorldSpec *ws, int trace);

void DestroyWorld(struct World *w);

void RunWorld(struct World *w);
void BeholdWorld(struct World *w);

void PrintWorld(struct World *w);

#ifdef TEST
void TEST_World();
#endif