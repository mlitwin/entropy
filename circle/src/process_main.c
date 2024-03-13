#include "lib/types/matrix.h"
#include "lib/stdio/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

static void usage()
{
    fprintf(stderr, "Usage: processes -p <path> <directory_name>\n");
    exit(EXIT_FAILURE);
}

static void GetLine(const char **oLine)
{
    static char *line = NULL;
    static size_t linecap = 0;
    static ssize_t linelen;
    static int index = 0;

    if (oLine == NULL)
    {
        free(line);
        return;
    }

    linelen = getline(&line, &linecap, stdin);
    if (linelen == 0)
    {
        fprintf(stderr, "Expected content at line %d\n", index);
        exit(-1);
    }
    index++;
    *oLine = line;
}

static int **ReadMatrix(int size)
{
    int **m = (int **)NewMatrix(sizeof(int), size, size);
    for (int t = 0; t < size; t++)
    {
        const char *l;
        GetLine(&l);
        for (int i = 0; i < size; i++)
        {
            int n;
            sscanf(l, "%d%n", &m[t][i], &n);
            l += (n);
        }
    }

    return m;
}

struct WorldSpec
{
    int n;
    int v;
    int p;
    int **density;
    int **prob;
};

static int meshDensity(struct WorldSpec *w, int sensitivity, int i0, int j0, int grain)
{
    int sum = 0;
    for (int i = 0; i < grain; i++)
    {
        for (int j = 0; j < grain; j++)
        {
            const int i1 = (i0 + i) % w->n;
            const int j1 = (j0 + j) % w->n;
            sum += w->density[i1][j1];
        }
    }

    return sum;
}

static void outputMesh(struct WorldSpec *w, int sensitivity, int size)
{
    const int grain = w->n / size;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            const int d = meshDensity(w, sensitivity, i, j, grain);
            printf("%d\n", d);
        }
    }
}

static int parse_args(int argc, char *argv[])
{
    int opt;
    char *path = NULL;
    char *dirname = NULL;

    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            path = optarg;
            break;
        default:
            usage();
        }
    }

    // Check if path is provided
    if (path != NULL)
    {
        if (chdir(path) != 0)
        {
            fprintf(stderr, "Could not chdir to %s\n", path);
            exit(EXIT_FAILURE);
        };
    }

    // Get the directory name
    if (optind >= argc)
    {
        fprintf(stderr, "Directory name must be provided.\n");
        usage();
    }

    return optind;
}

int main(int argc, char *argv[])
{

    struct WorldSpec w;

    const int size = 1024;
    const char *l;
    json_stream *stream = Create_JSON_Stream(stdout);
    int optind = parse_args(argc, argv);

    GetLine(&l);
    sscanf(l, "%d %d %d", &w.n, &w.v, &w.p);

    w.density = ReadMatrix(w.n);
    w.prob = ReadMatrix(w.n);
    jfprintf(stream, JSON_OBJECT_START);
    kv_jfprintf(stream, "n", JSON_INT, w.n);
    kv_jfprintf(stream, "v", JSON_INT, w.v);
    kv_jfprintf(stream, "p", JSON_INT, w.p);
    kv_jfprintf(stream, "level", JSON_ARRAY_START);
    jfprintf(stream, JSON_ARRAY_END);
    jfprintf(stream, JSON_OBJECT_END);

#if 0
    printf("%d %d %d\n", w.n, w.v, w.p);
    printf("%d\n", w.density[7][7]);
    printf("%d\n", w.prob[7][7]);


    outputMesh(&w, 1, size);
#endif

    DestroyMatrix((void **)w.density);
    DestroyMatrix((void **)w.prob);

    GetLine(NULL);

    Destroy_JSON_Stream(stream);
    return 0;
}