#include "lib/genesis/world.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

#if 0
static int signum(int a)
{
    return (0 < a) - (a < 0);
}

struct TimeStats
{
    int count;
    int wt;
    int center;
};

void timestats(struct TimeStats *ts, int *tv, int n)
{
    int center = 0;
    ts->count = 0;
    ts->wt = 0;
    for (int i = 0; i < n; i++)
    {
        center += i * abs(tv[i]);
        ts->wt += abs(tv[i]);
        if (tv[i] != 0)
        {
            ts->count++;
        }
    }
    ts->center = (center + ts->wt / 2) / ts->wt;
}
#endif

static void usage()
{
    printf("Usage: circle -n space -v velocity\n");
    exit(-1);
}

int main(int argc, char *argv[])
{
    struct World *w;
    int n = -1, v = -1, ch;

    while ((ch = getopt(argc, argv, "n:v:")) != -1)
    {
        switch (ch)
        {
        case 'n':
            n = atoi(optarg);
            break;
        case 'v':
            v = atoi(optarg);
            break;
        case '?':
        default:;
            usage();
        }
    }
    argc -= optind;
    argv += optind;

    if (n == -1 || v == -1)
    {
        usage();
    }

    sranddev();

    w = CreateNeWorld(n, v, 1);

    RunWorld(w);
    BeholdWorld(w);
    DestroyWorld(w);

    return 0;
}