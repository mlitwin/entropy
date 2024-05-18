#pragma once

#include <stdint.h>

#include "density_entry.h"
#include "../types/matrix.h"
#include "../types/vector.h"

struct WorldSpec
{
    int n;
    int period;
    int v;
    int density;
    int precision;
    int sensitivity;
    int cyclic_equivalence;
    int mesh_size;
};

struct WorldView
{
    int *densities;
    int max_density;
    int **probabilities;
    int64_t ***meshes;
    int **cohorts;
    int **states;
    int *num_states;
    struct densityEntry *density_entries;
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

void InitWorldSpec(struct WorldSpec *ws, int n, int v, int density, int precision);

struct World *CreateNeWorld(const struct WorldSpec *ws, int trace);

void DestroyWorld(struct World *w);

void RunWorld(struct World *w);
void BeholdWorld(struct World *w);

void PrintWorld(const struct World *w);

#ifdef TEST
void TEST_World();
#endif