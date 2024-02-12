#include "lib/genesis/world.h"
#include "lib/types/matrix.h"
#include "lib/types/vectormap.h"
#include "lib/types/vector.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    sranddev();
    TEST_Vector();

    TEST_matrix();
    // TEST_VectorMap();
    //  TEST_World();

    return 0;
}