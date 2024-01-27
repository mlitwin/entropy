#include "lib/genesis/world.h"
#include "lib/types/vectormap.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    World w;
    int *m;
    // VectorMap *vm;

    sranddev();
    // vm = NewVectorMap(w->sensitivity);
    CreateNeWorld(&w, 13, 2, 1, 1);
    m = calloc(sizeof(int), w.n);

    for (int t = 0; t < w.n; t++)
    {

        for (int i = 0; i < w.n; i++)
        {
            VectorValue *v = VectorMap_Get(w.vm[i], w.densities, w.n);
            (v->value)++;
            if (t == w.n - 1)
            {
                m[i] = v->value;
            }
        }

        // printf(".\n");
        PrintWorld(&w);
        AdvanceWorld(&w);
    }
    for (int t = 0; t < w.n; t++)
    {
        for (int i = 0; i < w.n; i++)
        {

            VectorValue *v = VectorMap_Get(w.vm[i], w.densities, w.n);
            printf("%d: (%d) x %d\n", t, i, v->value);
        }
        // PrintWorld(&w);
    }
    AdvanceWorld(&w);

    DestroyWorld(&w);

    return 0;
}