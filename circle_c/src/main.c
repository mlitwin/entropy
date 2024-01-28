#include "lib/genesis/world.h"
#include "lib/types/vectormap.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    World w;
    int *m;
    int n, v, ch;
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
    m = calloc(sizeof(int), w.n);

    for (int t = 0; t < w.n; t++)
    {

        for (int i = 0; i < w.n; i++)
        {
            VectorValue *v = VectorMap_Get(w.vm[i], w.densities, w.n);
            (v->value)++;
            // printf("%d: (%d) d %d\n", t, i, v->value);

            if (t == w.n - 1)
            {
                m[i] = v->value;
            }
        }

        // PrintWorld(&w);
        AdvanceWorld(&w);
    }
    for (int t = 0; t < w.n; t++)
    {
        for (int i = 0; i < w.n; i++)
        {

            VectorValue *v = VectorMap_Get(w.vm[i], w.densities, w.n);
            int prev = m[i];
            int delta = v->value - prev;
            if (0 != delta)
            {
                printf("%d: (%d) d %d %d %d\n", t, i, delta, prev, v->value);
            }
            m[i] = v->value;
        }
        AdvanceWorld(&w);

        //  PrintWorld(&w);
    }

    DestroyWorld(&w);

    return 0;
}