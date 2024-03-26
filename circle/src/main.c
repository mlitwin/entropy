#include "lib/genesis/world.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

static void usage()
{
    printf("Usage: circle -n space -v velocity [-d density] [-p precision]\n");
    exit(-1);
}

int main(int argc, char *argv[])
{
    struct World *w;
    int n = -1, v = -1, d = -1, p = 1, ch;

    static struct option longopts[] = {
        {"space", required_argument, NULL, 'n'},
        {"max_velocity", required_argument, NULL, 'v'},
        {"density_range", required_argument, NULL, 'd'},
        {"precision", required_argument, NULL, 'p'},
        {NULL, 0, NULL, 0}};

    while ((ch = getopt_long(argc, argv, "n:v:d:p:", longopts, NULL)) != -1)
    {
        switch (ch)
        {
        case 'n':
            n = atoi(optarg);
            break;
        case 'v':
            v = atoi(optarg);
            break;
        case 'd':
            d = atoi(optarg);
            break;
        case 'p':
            p = atoi(optarg);
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
    if (d == -1)
    {
        d = v;
    }

    sranddev();

    w = CreateNeWorld(n, v, d, p);

    RunWorld(w);
    BeholdWorld(w);
    PrintWorld(w);
    DestroyWorld(w);

    return 0;
}