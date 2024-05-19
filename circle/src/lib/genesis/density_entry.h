#pragma once

#include <stdint.h>

struct densityEntry
{
    int t;
    int cohort;
    int shift;
    int64_t *states;
};

int densityEntryCmp(const struct densityEntry *a, const struct densityEntry *b, int sensitivity);

int ComputeCohorts(int *cohort_counts, struct densityEntry *densities, int n, int period, int sensitivity);

void AddToMeshes(int ***meshes, int *densities, int n, int i0, int grain);