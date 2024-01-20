#include "lib/genesis/world.h"
#include "lib/types/matrix.h"
#include "lib/types/vectormap.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    sranddev();

    TEST_matrix();
    TEST_VectorMap();

    return 0;
}