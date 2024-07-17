#pragma once

#include <stdint.h>

#include "density_entry.h"

struct WorldSpec
{
    int n;
    int period;
    int v;
    int num_particles;
    int precision;
    int min_sensitivity;
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
