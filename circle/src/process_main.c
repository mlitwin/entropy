#include "lib/types/matrix.h"
#include "lib/stdio/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

static void usage()
{
    fprintf(stderr, "Usage: processes [-d <directory_name>] name\n");
    exit(EXIT_FAILURE);
}

static FILE *openFile(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        fprintf(stderr, "Could not open %s for %s error code %d\n", filename, mode, errno);
    }
    return file;
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
            sum += w->density[i1][j1] / sensitivity;
        }
    }

    return sum;
}

static void outputMesh(struct WorldSpec *w, int sensitivity, int size, const char *outputFileName)
{
    const int grain = w->n / size;
    FILE *outfile = openFile(outputFileName, "w");

    json_stream *stream = Create_JSON_Stream(outfile);

    jfprintf(stream, JSON_OBJECT_START);
    kv_jfprintf(stream, "d", JSON_ARRAY_START);
    for (int i = 0; i < size; i++)
    {
        jfprintf(stream, JSON_ARRAY_START);

        for (int j = 0; j < size; j++)
        {
            const int d = meshDensity(w, sensitivity, i, j, grain);
            jfprintf(stream, JSON_INT, d);
        }

        jfprintf(stream, JSON_ARRAY_END);
    }
    jfprintf(stream, JSON_ARRAY_END);

    jfprintf(stream, JSON_OBJECT_END);

    Destroy_JSON_Stream(stream);
    fclose(outfile);
}

static int parse_args(int argc, char *argv[])
{
    int opt;
    char *dirname = NULL;

    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch (opt)
        {
        case 'd':
            dirname = optarg;
            break;
        default:
            usage();
        }
    }

    if (dirname != NULL)
    {
        if (chdir(dirname) != 0)
        {
            fprintf(stderr, "Could not chdir to %s\n", dirname);
            exit(EXIT_FAILURE);
        };
    }

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
    // const int size = 2;
    const char *l;
    int optind = parse_args(argc, argv);
    const char *name = argv[optind];
    char indexFilePath[PATH_MAX];
    sprintf(indexFilePath, "%s.json", name);
    FILE *indexFile = openFile(indexFilePath, "w");
    json_stream *stream = Create_JSON_Stream(indexFile);

    GetLine(&l);
    sscanf(l, "%d %d %d", &w.n, &w.v, &w.p);

    w.density = ReadMatrix(w.n);
    w.prob = ReadMatrix(w.n);
    jfprintf(stream, JSON_OBJECT_START);
    kv_jfprintf(stream, "n", JSON_INT, w.n);
    kv_jfprintf(stream, "v", JSON_INT, w.v);
    kv_jfprintf(stream, "p", JSON_INT, w.p);
    kv_jfprintf(stream, "levels", JSON_ARRAY_START);
    for (int level = 1; level <= size; level++)
    {
        char levelFile[PATH_MAX];
        sprintf(levelFile, "%s/level_%d.json", name, level);

        jfprintf(stream, JSON_OBJECT_START);
        kv_jfprintf(stream, "level", JSON_INT, level);
        kv_jfprintf(stream, "file", JSON_STRING, levelFile);
        jfprintf(stream, JSON_OBJECT_END);
    }
    jfprintf(stream, JSON_ARRAY_END);
    jfprintf(stream, JSON_OBJECT_END);
    fclose(indexFile);
    Destroy_JSON_Stream(stream);

    for (int level = 1; level <= size; level++)
    {
        char levelFile[PATH_MAX];
        sprintf(levelFile, "%s/level_%d.json", name, level);

        printf("\33[2K\r");
        printf("%d/%d", level, size);
        fflush(stdout);

        outputMesh(&w, level, size, levelFile);
    }

    DestroyMatrix((void **)w.density);
    DestroyMatrix((void **)w.prob);

    GetLine(NULL);

    return 0;
}