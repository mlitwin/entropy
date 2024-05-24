#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define CLEAR_LINE "\33[2K\r"

int reportStatus(const char *step, int i, int n)
{
#ifndef TEST
    if (step)
    {
        fprintf(stderr, CLEAR_LINE "%s: %d/%d", step, i, n);
    }
    else
    {
        fprintf(stderr, CLEAR_LINE);
    }
    return fflush(stderr);
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
