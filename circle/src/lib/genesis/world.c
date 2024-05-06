#include "world.h"
#include "../types/matrix.h"
#include "../algo/cycles.h"
#include "../stdio/util.h"
#include "../mem/mem.h"

#include <stdio.h>
#include <strings.h>

static void ordainDarkMaterials(struct World *w)
{
    for (int j = 0; j < w->num_v; j++)
    {
        const int velocity = j - (w->num_v - 1) / 2;

        for (int i = 0; i < w->s.n; i++)
        {
            int nextI = (i + velocity) % w->s.n;
            const int val = rand() % w->s.density;

            w->cur[j][i] = val;
            w->v.densities[i] += val;

            if (nextI < 0)
            {
                nextI += w->s.n;
            }
        }
    }
}

struct World *CreateNeWorld(int n, int v, int density, int precision)
{
    struct World *w = mem_calloc(sizeof(struct World), 1);
    const int period = n;

    w->s.n = n;
    w->s.period = period;
    w->s.precision = precision;
    w->num_v = 2 * v + 1;
    w->s.v = v;
    w->s.density = density;
    w->s.mesh_size = 1024;
    w->t = 0;

    w->cur = (int **)NewMatrix(sizeof(int), w->num_v, n);
    w->v.densities = mem_calloc(n, sizeof(int));
    w->v.density_entries = mem_calloc(period, sizeof(struct densityEntry));
    for (int i = 0; i < period; i++)
    {
        w->v.density_entries[i].states = NewVector(density);
    }

    w->v.meshes = (int64_t ***)NewMatrix(sizeof(int64_t *), w->s.mesh_size, w->s.mesh_size);
    for (int i = 0; i < w->s.mesh_size; i++)
    {
        for (int j = 0; j < w->s.mesh_size; j++)
        {
            w->v.meshes[i][j] = NewVector(256);
        }
    }

    ordainDarkMaterials(w);

    return w;
}

void DestroyWorld(struct World *w)
{
    free(w->v.num_states);
    DestroyMatrix((void **)w->v.states);
    DestroyMatrix((void **)w->v.probabilities);
    DestroyMatrix((void **)w->v.cohorts);
    free(w->v.densities);
    for (int i = 0; i < w->s.period; i++)
    {
        DestroyVector(w->v.density_entries[i].states);
    }
    free(w->v.density_entries);
    for (int i = 0; i < w->s.mesh_size; i++)
    {
        for (int j = 0; j < w->s.mesh_size; j++)
        {
            DestroyVector(w->v.meshes[i][j]);
        }
    }
    DestroyMatrix((void **)w->v.meshes);

    free(w);
}

static void AdvanceWorld(struct World *w)
{
    w->t++;
    bzero(w->v.densities, sizeof(int) * w->s.n);

    for (int j = 0; j < w->num_v; j++)
    {
        const int *cur_row = w->cur[j];
        const int velocity = j + 1;
        const int shift = (w->t * velocity) % w->s.n;
        const int first_segment_len = w->s.n - shift;

        for (int i = 0; i < first_segment_len; i++)
        {
            const int nextI = i + shift;
            w->v.densities[nextI / w->s.precision] += cur_row[i];
        }

        for (int i = first_segment_len; i < w->s.n; i++)
        {
            const int nextI = i - first_segment_len;
            w->v.densities[nextI / w->s.precision] += cur_row[i];
        }
    }
}

static void recordMesh(struct World *w)
{
    const int *densities = w->v.densities;
    const int size = w->s.mesh_size;
    const int grain_i = (w->s.n + size - 1) / size;
    const int grain_j = (w->s.period + size - 1) / size;
    const int i0 = w->t / grain_i;
    for (int j0 = 0; j0 < size; j0++)
    {
        for (int k = 0; k < grain_j; k++)
        {
            int j = (j0 * size + k) % w->s.n;
            const int d = densities[j];
            int s = 0;
            while (1)
            {
                const int v = d / (s + 1);
                if (v == 0)
                {
                    break;
                }
                Vector_Increment(&w->v.meshes[i0][j0], s, v);

                s++;
            }
        }
    }
}

static void recordWorld(struct World *w)
{
    const int n = w->s.n;
    const int *densities = w->v.densities;
    int maxDensity = densities[0];
    for (int i = 1; i < n; i++)
    {
        if (densities[i] > maxDensity)
        {
            maxDensity = densities[i];
        }
    }
    for (int i = 1; i <= maxDensity; i++)
    {
        int64_t hash = cycleHash(densities, i, 0, n);
        if (hash == 0)
        {
            break;
        }
        Vector_Push(&w->v.density_entries[w->t].states, hash);
    }

    recordMesh(w);
}

void RunWorld(struct World *w)
{
    recordWorld(w);
    while (w->t < w->s.n - 1)
    {
        AdvanceWorld(w);
        recordWorld(w);
        reportStatus("Advancing", w->t, w->s.n - 1);
    }
}

static int maxDensity(struct World *w)
{
    int d = 0;

    const int *v = w->v.densities;
    for (int i = 0; i < w->s.n; i++)
    {
        const int val = v[i];
        if (d < val)
        {
            d = val;
        }
    }

    return d;
}

void BeholdWorld(struct World *w)
{
    struct densityEntry *densities = w->v.density_entries;
    int *cohort_counts = (int *)mem_calloc(sizeof(int), w->s.n);
    struct canonicalCycleShifter *shifterState = createCanonicalCycleShifter(w->s.n);

    w->s.sensitivity = maxDensity(w) + 1;

    w->v.cohorts = (int **)NewMatrix(sizeof(int), w->s.sensitivity, w->s.n);
    w->v.probabilities = (int **)NewMatrix(sizeof(int), w->s.sensitivity, w->s.n);
    w->v.states = (int **)NewMatrix(sizeof(int), w->s.sensitivity, w->s.n);
    w->v.num_states = (int *)mem_calloc(sizeof(int), w->s.sensitivity);

    for (int t = 0; t < w->s.period; t++)
    {
        densities[t].t = t;
    }

    for (int s = 0; s < w->s.sensitivity; s++)
    {

        /*
                for (int i = 0; i < w->s.period; i++)
                {
                    densities[i].shift = canonicalCycleShift(shifterState, densities[i].v, s + 1);
                }
                */

        w->v.num_states[s] = ComputeCohorts(cohort_counts, densities, w->s.n, w->s.period, s + 1);
        for (int i = 0; i < w->v.num_states[s]; i++)
        {
            w->v.states[s][i] = cohort_counts[i];
        }
        for (int i = 0; i < w->s.period; i++)
        {
            const int t = densities[i].t;
            w->v.cohorts[s][t] = densities[i].cohort;
            w->v.probabilities[s][t] = cohort_counts[densities[i].cohort];
        }
        reportStatus("Computing states", s, w->s.sensitivity);
    }
    destroyCanonicalCycleShifter(shifterState);
    free(cohort_counts);
}

static void printMatrix(int **mat, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            const char *sep = j == 0 ? "" : " ";
            printf("%s%d", sep, mat[i][j]);
        }
        printf("\n");
    }
}

void PrintWorld(const struct World *w)
{
    printf("# n v density precision sensitivity\n");
    printf("%d %d %d %d %d\n", w->s.n, w->s.v, w->s.density, w->s.precision, w->s.sensitivity);
    printf("# densities %dx%d\n", w->s.period, w->s.n);
    printf("# cohorts %dx%d\n", w->s.sensitivity, w->s.n);
    printMatrix(w->v.cohorts, w->s.sensitivity, w->s.n);
    printf("# probabilities %dx%d\n", w->s.sensitivity, w->s.n);
    printMatrix(w->v.probabilities, w->s.sensitivity, w->s.n);
}

#ifdef TEST
#include "test.h"
void TEST_World()
{
    struct World *w;
    const int n = 25;

    srand(1);
    w = CreateNeWorld(n, 2, 2, 1);
    RunWorld(w);
    /* PrintWorld(w); */
    BeholdWorld(w);
    DestroyWorld(w);
}
#endif