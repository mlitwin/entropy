#include "lib/genesis/world.h"
#include "lib/types/vectormap.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    World w;
    VectorMap *vm;

    sranddev();
    vm = NewVectorMap(1, 100);
    CreateNeWorld(&w, 40, 10);

    for (int t = 0; t < w.n; t++)
    {
        VectorValue *v = VectorMap_Get(vm, w.observation, w.n);
        (v->value)++;
        // PrintWorld(&w);
        AdvanceWorld(&w);
    }
    for (int t = 0; t < w.n; t++)
    {
        VectorValue *v = VectorMap_Get(vm, w.observation, w.n);
        // PrintWorld(&w);

        PrintWorld(&w);
        printf("%d: x %d\n", t, v->value);
    }
    AdvanceWorld(&w);

    DestroyWorld(&w);

    return 0;
}