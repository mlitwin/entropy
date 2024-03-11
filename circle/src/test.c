#include "lib/genesis/world.h"
#include "lib/types/matrix.h"
#include "lib/stdio/json.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    sranddev();
    TEST_Matrix();
    TEST_World();
    TEST_JSON();

    return 0;
}