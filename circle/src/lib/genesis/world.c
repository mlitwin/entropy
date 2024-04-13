#include "world.h"
#include "../types/matrix.h"
#include "../algo/cycles.h"
#include "../stdio/util.h"

#include <stdio.h>
#include <stdlib.h>
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
            w->v.densities[0][i] += val;

            if (nextI < 0)
            {
                nextI += w->s.n;
            }
            w->permutation[j][i] = nextI;
        }
    }
}

struct World *CreateNeWorld(int n, int v, int density, int precision)
{
    struct World *w = calloc(sizeof(struct World), 1);

    w->s.n = n;
    w->s.period = n;
    w->s.precision = precision;
    w->num_v = 2 * v + 1;
    w->s.v = v;
    w->s.density = density;
    w->t = 0;

    w->a = (int **)NewMatrix(sizeof(int), w->num_v, n);
    w->b = (int **)NewMatrix(sizeof(int), w->num_v, n);
    w->permutation = (int **)NewMatrix(sizeof(int), w->num_v, n);

    w->v.densities = (int **)NewMatrix(sizeof(int), w->s.period, n);
    w->cur = w->a;
    w->next = w->b;

    ordainDarkMaterials(w);

    return w;
}

void DestroyWorld(struct World *w)
{
    free(w->v.num_states);
    DestroyMatrix((void **)w->v.states);
    DestroyMatrix((void **)w->v.probabilities);
    DestroyMatrix((void **)w->v.cohorts);
    DestroyMatrix((void **)w->v.densities);
    DestroyMatrix((void **)w->permutation);
    DestroyMatrix((void **)w->b);
    DestroyMatrix((void **)w->a);
    free(w);
}

static void AdvanceWorld(struct World *w)
{
    w->t++;

    for (int j = 0; j < w->num_v; j++)
    {
        int *cur_row = w->cur[j];
        int *next_row = w->next[j];

        for (int i = 0; i < w->s.n; i++)
        {
            const int curVal = cur_row[i];
            const int nextI = w->permutation[j][i];

            next_row[nextI] = curVal;

            w->v.densities[w->t][nextI / w->s.precision] += curVal;

            cur_row[i] = 0;
        }
    }

    int **tmp = w->cur;
    w->cur = w->next;
    w->next = tmp;
}

void RunWorld(struct World *w)
{
    while (w->t < w->s.n - 1)
    {
        AdvanceWorld(w);
        reportStatus("Advancing", w->t, w->s.n - 1);
    }
}

struct densityEntry
{
    int t;
    int cohort;
    int shift;
    int *v;
};

static int densityCyclicCmp(int *a0, int a_start, int *b0, int b_start, int len, int sensitivity)
{
    int *a = a0 + a_start;
    int *b = b0 + b_start;
    int n = len;

    while (n != 0)
    {
        const int diff = (*a) / sensitivity - (*b) / sensitivity;
        if (diff != 0)
        {
            return diff;
        }
        a++;
        if (a - a0 >= len)
        {
            a -= len;
        }
        b++;
        if (b - b0 >= len)
        {
            b -= len;
        }
        n--;
    }

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
    int *a = A->v;
    int *b = B->v;

    int n = ds->n;
    const int sensitivity = ds->sensitivity;

    return densityCyclicCmp(a, A->shift, b, B->shift, n, sensitivity);
}

static int densityEqual(int *a, int *b, int n, int sensitivity)
{
    while (n > 0)
    {
        if (*(a + n - 1) / sensitivity != *(b + n - 1) / sensitivity)
        {
            return 0;
        }
        n--;
    }
    return 1;
}

static int computeCohorts(struct World *w, struct densityEntry *densities, int *cohort_counts, int sensitivity)
{
    int cohort = 0;
    int cur = 1;

    cohort_counts[0] = 1;
    densities[0].cohort = cohort;

    while (cur < w->s.period)
    {
        if (!densityEqual(densities[cur].v, densities[cur - 1].v, w->s.n, sensitivity))
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

static int maxDensity(struct World *w)
{
    int d = 0;
    for (int t = 0; t < w->s.period; t++)
    {
        const int *v = w->v.densities[t];
        for (int i = 0; i < w->s.n; i++)
        {
            const int val = v[i];
            if (d < val)
            {
                d = val;
            }
        }
    }
    return d;
}

void BeholdWorld(struct World *w)
{
    struct densityEntry *densities = calloc(sizeof(struct densityEntry), w->s.period);
    int *cohort_counts = (int *)calloc(sizeof(int), w->s.n);
    struct densitySortThunk sortThunk;
    struct canonicalCycleShifter *shifterState = createCanonicalCycleShifter(w->s.n);

    w->s.sensitivity = maxDensity(w) + 1;

    w->v.cohorts = (int **)NewMatrix(sizeof(int), w->s.sensitivity, w->s.n);
    w->v.probabilities = (int **)NewMatrix(sizeof(int), w->s.sensitivity, w->s.n);
    w->v.states = (int **)NewMatrix(sizeof(int), w->s.sensitivity, w->s.n);
    w->v.num_states = (int *)calloc(sizeof(int), w->s.sensitivity);

    for (int t = 0; t < w->s.period; t++)
    {
        densities[t].t = t;
        densities[t].v = w->v.densities[t];
    }

    sortThunk.n = w->s.n;
    for (int s = 0; s < w->s.sensitivity; s++)
    {
        sortThunk.sensitivity = s + 1;

        for (int i = 0; i < w->s.period; i++)
        {
            densities[i].shift = canonicalCycleShift(shifterState, densities[i].v, s + 1);
        }
        qsort_r(densities, w->s.period, sizeof(struct densityEntry), &sortThunk, densityCmp);

        w->v.num_states[s] = computeCohorts(w, densities, cohort_counts, s + 1) + 1;
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
    free(densities);
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
    printMatrix(w->v.densities, w->s.period, w->s.n);
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
    const int n = 500;

    srand(1);
    w = CreateNeWorld(n, 3, 10, 1);
    RunWorld(w);
    BeholdWorld(w);
    DestroyWorld(w);
}
#endif