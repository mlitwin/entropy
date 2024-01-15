#include "lib/genesis/world.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    World w;

    sranddev();
    CreateNeWorld(&w, 4, 2);

    for (int t = 0; t < w.n; t++)
    {
        PrintWorld(&w);
        AdvanceWorld(&w);
    }
    DestroyWorld(&w);

    return 0;
}