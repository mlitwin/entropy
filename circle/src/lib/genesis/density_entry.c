#include "density_entry.h"

#include <stdlib.h>
#include <string.h>

#include "../types/vector.h"

int densityEntryCmp(const struct densityEntry *a, const struct densityEntry *b, int sensitivity)
{
    int64_t A = Vector_Get(a->states, sensitivity - 1) / sensitivity;
    int64_t B = Vector_Get(b->states, sensitivity - 1) / sensitivity;
    if (A < B)
        return -1;
    if (A > B)
        return 1;
    return 0;
}

struct densitySortThunk
{
    int n;
    int sensitivity;
};

static int
densityCmp(void *thunk, const void *iA, const void *iB)
{
    struct densitySortThunk *ds = thunk;
    const struct densityEntry *A = iA;
    const struct densityEntry *B = iB;
    return densityEntryCmp(A, B, ds->sensitivity);
}

static int computeCohorts(int *cohort_counts, struct densityEntry *densities, int period, int sensitivity)
{
    int cohort = 0;
    int cur = 1;

    cohort_counts[0] = 1;
    densities[0].cohort = cohort;

    while (cur < period)
    {

        if (0 != densityEntryCmp(densities + cur, densities + cur - 1, sensitivity))
        {
            cohort++;
            cohort_counts[cohort] = 0;
        }
        densities[cur].cohort = cohort;
        cohort_counts[cohort]++;
        cur++;
    }

    return cohort;
}

int ComputeCohorts(int *cohort_counts, struct densityEntry *densities, int n, int period, int sensitivity)
{
    struct densitySortThunk sortThunk = {n, sensitivity};

    qsort_r(densities, period, sizeof(struct densityEntry), &sortThunk, densityCmp);

    return computeCohorts(cohort_counts, densities, period, sensitivity) + 1;
}

static int cmp_density(const void *a, const void *b)
{
    return *(int *)b - *(int *)a;
}

void AddToMeshes(int ***meshes, int *densities, int n, int i0, int grain)
{
    int *d = densities;
    for (int j0 = 0; j0 < grain; j0++)
    {
        int cur = -1;
        int curCount = 0;
        if (j0 == grain - 1)
        {
            int len = (densities + n) - d;
            int remaining = grain - len;
            memcpy(densities + remaining, d, len);
            d = densities;
        }
        qsort(d, grain, sizeof(int), cmp_density);

        for (int k = 0; k < grain; k++)
        {
            const int val = d[k];
            int emit = (k == grain - 1);
            if (val == cur)
            {
                curCount++;
            }
            else
            {
                cur = val;
                curCount = 1;
                emit = 1;
            }

            if (emit)
            {
                for (int s = 1; s <= val; s++)
                {
                    const int v = val / s;
                    meshes[s - 1][i0][j0] += curCount * v;
                }
            }
        }

        n -= grain;
        d += grain;
    }
}
