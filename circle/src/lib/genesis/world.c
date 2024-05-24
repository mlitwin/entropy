#include "world.h"
#include "../types/matrix.h"
#include "../algo/cycles.h"
#include "../algo/entropies.h"
#include "../stdio/util.h"
#include "../mem/mem.h"

#include <stdio.h>
#include <strings.h>

static void ordainDarkMaterials(struct World *w)
{
    const int particles = w->s.num_particles;
    const int n = w->s.n;
    const int v = w->num_v;
    for (int p = 0; p < particles; p++)
    {
        const int j = rand() % v;
        const int i = rand() % n;
        w->cur[j][i] += 1;
        w->v.densities[i] += 1;
    }
}

void InitWorldSpec(struct WorldSpec *ws, int n, int v, int density, int precision)
{
    const int period = n;

    ws->n = n;
    ws->period = period;
    ws->precision = precision;
    ws->v = v;
    ws->num_particles = density;
    ws->mesh_size = 1024;
}

struct World *CreateNeWorld(const struct WorldSpec *ws, int trace)
{
    const int min_density = 32;
    struct World *w = mem_calloc(sizeof(struct World), 1);
    const int n = ws->n;
    const int period = n;
    int density = ws->num_particles / ws->n;

    if (density < min_density)
    {
        density = min_density;
    }

    w->s = *ws;

    w->num_v = 2 * w->s.v + 1;
    w->t = 0;
    w->cur = (int **)NewMatrix(sizeof(int), w->num_v, n);
    w->v.densities = mem_calloc(n, sizeof(int));
    w->v.max_density = 0;
    w->v.density_entries = mem_calloc(period, sizeof(struct densityEntry));
    for (int i = 0; i < period; i++)
    {
        w->v.density_entries[i].states = NewVector(density);
    }

    if (trace)
    {
        w->v.meshes = (int64_t ***)NewMatrix(sizeof(int64_t *), w->s.mesh_size, w->s.mesh_size);
        for (int i = 0; i < w->s.mesh_size; i++)
        {
            for (int j = 0; j < w->s.mesh_size; j++)
            {
                w->v.meshes[i][j] = NewVector(256);
            }
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

    if (w->v.meshes)
    {
        for (int i = 0; i < w->s.mesh_size; i++)
        {
            for (int j = 0; j < w->s.mesh_size; j++)
            {
                DestroyVector(w->v.meshes[i][j]);
            }
        }

        DestroyMatrix((void **)w->v.meshes);
    }

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

static void recordMeshRow(int64_t **mesh, const int *densities, int n, int i0, int size, int grain_j)
{
    for (int j0 = 0; j0 < size; j0++)
    {
        for (int k = 0; k < grain_j; k++)
        {
            int j = (j0 * size + k) % n;
            const int d = densities[j];
            int s = 0;
            while (1)
            {
                const int v = d / (s + 1);
                if (v == 0)
                {
                    break;
                }
                Vector_Increment(&mesh[j0], s, v);

                s++;
            }
        }
    }
}

static void recordMesh(struct World *w)
{
    const int *densities = w->v.densities;
    const int size = w->s.mesh_size;
    const int grain_i = (w->s.n + size - 1) / size;
    const int grain_j = (w->s.period + size - 1) / size;
    const int i0 = w->t / grain_i; // TODO i0 and iLast can overlap a bit
    recordMeshRow(w->v.meshes[i0], densities, w->s.n, i0, size, grain_j);
    if (i0 == 0)
    {
        const int iLast = w->s.n / grain_i;
        const int remainder = (iLast + size) - w->s.n;
        if (w->t < remainder)
        {
            recordMeshRow(w->v.meshes[i0], densities, w->s.n, iLast, size, grain_j);
        }
    }
}

static void recordWorld(struct World *w)
{
    const int n = w->s.n;
    const int *densities = w->v.densities;
    int maxDensity = densities[0];
    struct canonicalCycleShifter *shifterState = createCanonicalCycleShifter(w->s.n);

    for (int i = 1; i < n; i++)
    {
        if (densities[i] > maxDensity)
        {
            maxDensity = densities[i];
        }
    }
    for (int i = 1; i <= maxDensity; i++)
    {
        int start = 0;
        if (w->s.cyclic_equivalence)
        {
            start = canonicalCycleShift(shifterState, densities, i);
        }
        int64_t hash = cycleHash(densities, i, start, n);
        if (hash == 0)
        {
            break;
        }
        Vector_Push(&w->v.density_entries[w->t].states, hash);
    }

    if (w->v.max_density < maxDensity)
    {
        w->v.max_density = maxDensity;
    }

    if (w->v.meshes)
    {
        recordMesh(w);
    }
    destroyCanonicalCycleShifter(shifterState);
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

void BeholdWorld(struct World *w)
{
    struct densityEntry *densities = w->v.density_entries;
    int *cohort_counts = (int *)mem_calloc(sizeof(int), w->s.n);

    w->s.sensitivity = w->v.max_density + 1;

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
    free(cohort_counts);
}

void PrintWorld(struct World *w)
{
    struct EntropyMeasures entropies;

    double base_entropy = shannon_entropy(w->v.num_states[0], w->v.states[0]);

    max_mean_entropies(&entropies, w->s.n, w->s.sensitivity, w->v.num_states, w->v.states, w->v.probabilities);

    reportStatus(NULL, 0, 0);

    printf("%d %d %d %d %d %f %f %f %f\n",
           w->s.n,
           w->s.v,
           w->s.num_particles,
           w->s.precision,
           w->s.sensitivity,
           base_entropy,
           entropies.mean_jitter,
           entropies.mean_shannon,
           entropies.mean_sensitivity);
}

#ifdef TEST
#include "test.h"
void TEST_World()
{
    struct World *w;
    struct WorldSpec ws;
    const int n = 25;

    srand(1);
    InitWorldSpec(&ws, n, 2, 2, 1);
    w = CreateNeWorld(&ws, 1);
    RunWorld(w);
    /* PrintWorld(w); */
    BeholdWorld(w);
    DestroyWorld(w);
}
#endif