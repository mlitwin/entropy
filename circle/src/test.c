#include "lib/genesis/world.h"
#include "lib/types/matrix.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    sranddev();
    TEST_Matrix();
    TEST_World();

    return 0;
}