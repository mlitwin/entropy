#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int reportStatus(const char *step, int i, int n)
{
#ifndef TEST
    printf("\33[2K\r%s: %d/%d", step, i, n);
    return fflush(stdout);
#else
    return 0;
#endif
}

FILE *
openFile(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        fprintf(stderr, "Could not open %s for %s error code %d\n", filename, mode, errno);
    }
    return file;
}
