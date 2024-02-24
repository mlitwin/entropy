#include "lib/genesis/world.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

static int signum(int a)
{
    return (0 < a) - (a < 0);
}

struct TimeStats
{
    int count;
    int wt;
    int center;
};

void timestats(struct TimeStats *ts, int *tv, int n)
{
    int center = 0;
    ts->count = 0;
    ts->wt = 0;
    for (int i = 0; i < n; i++)
    {
        center += i * abs(tv[i]);
        ts->wt += abs(tv[i]);
        if (tv[i] != 0)
        {
            ts->count++;
        }
    }
    ts->center = (center + ts->wt / 2) / ts->wt;
}

int sortVecCmp(void *thunk, const void *a, const void *b)
{
    return Vector_Cmp((struct Vector_CmpArgs *)thunk, (int *)a, (int *)b);
}

int main(int argc, char *argv[])
{
    World w;
    int *m;
    int *tv;
    int n, v, ch;
    struct TimeStats ts;

    while ((ch = getopt(argc, argv, "n:v:")) != -1)
    {
        switch (ch)
        {
        case 'n':
            n = atoi(optarg);
            break;
        case 'v':
            v = atoi(optarg);
            break;
        case '?':
        default:;
            // usage();
        }
    }
    argc -= optind;
    argv += optind;

    sranddev();
    CreateNeWorld(&w, n, v, 1);
    m = NewVector(w.n);
    tv = NewVector(w.n);

    for (int t = 0; t < w.n; t++)
    {

        printf("first %d\n", t);
#if 0
        for (int i = 0; i < w.n; i++)
        {
            VectorValue *v = VectorMap_Get(w.vm[i], w.densities[w.t]);
            v->value++;
            // printf("%d: (%d) d %d\n", t, i, v->value);

            if (t == w.n - 1)
            {
                m[i] = v->value;
            }
        }
#endif

        // PrintWorld(&w);
        if (t != w.n - 1)
        {
            AdvanceWorld(&w);
        }
    }

    struct Vector_CmpArgs args = {w.n, w.n, 1};
    for (int i = 0; i < w.n; i++)
    {
        printf("%d\n", i + 1);

        args.sensitivity = i + 1;
        qsort_r(w.densities, w.n, sizeof(int), &args, &sortVecCmp);
    }
#if 0
    for (int t = 0; t < w.n; t++)
    {
        printf("%d\n", t);

        for (int i = 0; i < w.n; i++)
        {

            VectorValue *v = VectorMap_Get(w.vm[i], w.densities[t]);
            int prev = m[i];
            int delta = v->value - prev;
            tv[i] += signum(delta);
            if (0 != delta)
            {
                // printf("%d: (%d) d %d %d %d\n", t, i, delta, prev, v->value);
            }
            m[i] = v->value;
        }
        // AdvanceWorld(&w);

        //  PrintWorld(&w);
    }

    timestats(&ts, tv, w.n);
    printf("(%d, %d): %d %d %d\n", w.precision, w.n, ts.count, ts.wt, ts.center);
#endif

    // DestroyWorld(&w);

    return 0;
}