#include "world.h"
#include "../types/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

struct World
{
    int n;
    int max_v;
    int v;
    int precision;
    int t;

    int **cur;
    int **densities;
    int **probabilities;
    int **cohorts;
    int **permutation;

    int **next;
    int **a;
    int **b;
};

static void ordainDarkMaterials(struct World *w)
{
    for (int j = 0; j < w->v; j++)
    {
        const int velocity = j - w->v / 2;

        for (int i = 0; i < w->n; i++)
        {
            int nextI = (i + velocity) % w->n;
            const int val = rand() % w->v;

            w->cur[j][i] = val;
            w->densities[0][i] += val;

            if (nextI < 0)
            {
                nextI += w->n;
            }
            w->permutation[j][i] = nextI;
        }
    }
}

struct World *CreateNeWorld(int n, int v, int precision)
{
    struct World *w = calloc(sizeof(struct World), 1);

    w->n = n;
    w->max_v = v;
    v = 2 * v;
    w->v = v;
    w->t = 0;
    w->precision = precision;

    w->a = (int **)NewMatrix(sizeof(int), v, n);
    w->b = (int **)NewMatrix(sizeof(int), v, n);
    w->permutation = (int **)NewMatrix(sizeof(int), v, n);

    w->densities = (int **)NewMatrix(sizeof(int), n, n);
    w->cohorts = (int **)NewMatrix(sizeof(int), n, n);
    w->probabilities = (int **)NewMatrix(sizeof(int), n, n);

    w->cur = w->a;
    w->next = w->b;

    ordainDarkMaterials(w);

    return w;
}

void DestroyWorld(struct World *w)
{
    DestroyMatrix((void **)w->probabilities);
    DestroyMatrix((void **)w->cohorts);
    DestroyMatrix((void **)w->densities);
    DestroyMatrix((void **)w->permutation);
    DestroyMatrix((void **)w->b);
    DestroyMatrix((void **)w->a);
    free(w);
}

static void AdvanceWorld(struct World *w)
{
    w->t++;

    for (int j = 0; j < w->v; j++)
    {
        int *cur_row = w->cur[j];
        int *next_row = w->next[j];

        for (int i = 0; i < w->n; i++)
        {
            const int curVal = cur_row[i];
            const int nextI = w->permutation[j][i];

            next_row[nextI] = curVal;

            w->densities[w->t][nextI / w->precision] += curVal;

            cur_row[i] = 0;
        }
    }

    int **tmp = w->cur;
    w->cur = w->next;
    w->next = tmp;
}

void RunWorld(struct World *w)
{
    while (w->t < w->n - 1)
    {
        AdvanceWorld(w);
    }
}

struct densityEntry
{
    int t;
    int cohort;
    int *v;
};

struct densitySortThunk
{
    int n;
    int sensitivity;
};

static int
densityCmp(void *thunk, const void *iA, const void *iB)
{
    struct densitySortThunk *w = thunk;
    const struct densityEntry *A = iA;
    const struct densityEntry *B = iB;
    int *a = A->v;
    int *b = B->v;

    int n = w->n;
    const int sensitivity = w->sensitivity;

    while (n != 0)
    {
        const int diff = (*a) / sensitivity - (*b) / sensitivity;
        if (diff != 0)
        {
            return diff;
        }
        a++;
        b++;
        n--;
    }

    return 0;
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

    while (cur < w->n)
    {
        if (!densityEqual(densities[cur].v, densities[cur - 1].v, w->n, sensitivity))
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

void BeholdWorld(struct World *w)
{
    struct densityEntry *densities = calloc(sizeof(struct densityEntry), w->n);
    int *cohort_counts = (int *)calloc(sizeof(int), w->n);
    struct densitySortThunk sortThunk;

    for (int t = 0; t < w->n; t++)
    {
        densities[t].t = t;
        densities[t].v = w->densities[t];
        for (int s = 0; s < w->n; s++)
        {
            w->probabilities[s][t] = w->n; /* default */
        }
    }

    sortThunk.n = w->n;
    for (int s = 1; s < w->n; s++)
    {
        sortThunk.sensitivity = s;
        qsort_r(densities, w->n, sizeof(struct densityEntry), &sortThunk, densityCmp);

        computeCohorts(w, densities, cohort_counts, s);
        for (int t = 0; t < w->n; t++)
        {
            w->probabilities[s - 1][t] = cohort_counts[densities[t].cohort];
            w->cohorts[s - 1][t] = densities[t].cohort;
        }
    }

    free(cohort_counts);
    free(densities);
}

static void printMatrix(int **mat, int m, int n)
{
    for (int t = 0; t < m; t++)
    {
        for (int s = 0; s < n; s++)
        {
            const char *sep = s == 0 ? "" : " ";
            printf("%s%d", sep, mat[s][t]);
        }
        printf("\n");
    }
}

void PrintWorld(const struct World *w)
{
    printf("%d %d %d\n", w->n, w->v, w->precision);
    printMatrix(w->densities, w->n, w->n);
    printMatrix(w->cohorts, w->n, w->n);
    printMatrix(w->probabilities, w->n, w->n);
}

#ifdef TEST
#include "test.h"
void TEST_World()
{
    struct World *w;
    const int n = 500;

    srand(1);
    w = CreateNeWorld(n, 3, 1);
    RunWorld(w);
    BeholdWorld(w);
    DestroyWorld(w);
}
#endif