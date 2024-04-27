#include "lib/genesis/world.h"
#include "lib/types/matrix.h"
#include "lib/types/vector.h"
#include "lib/stdio/json.h"
#include "lib/algo/cycles.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    sranddev();
    TEST_Matrix();
    TEST_Vector();
    TEST_World();
    TEST_JSON();
    TEST_Cycles();
    return 0;
}