#include "trace_world.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../lib/types/matrix.h"
#include "../lib/stdio/json.h"

struct W
{
    struct WorldSpec *s;
    struct WorldView *v;
} W;

static void reportStatus(const char *step, int i, int n)
{
    printf("\33[2K\r%s: %d/%d", step, i, n);
    fflush(stdout);
}

static FILE *
openFile(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        fprintf(stderr, "Could not open %s for %s error code %d\n", filename, mode, errno);
    }
    return file;
}

static void meshDensities(struct W w, int size, int i0, int j0, int grain, int ***meshes)
{
    for (int i = 0; i < grain; i++)
    {
        for (int j = 0; j < grain; j++)
        {
            const int i1 = (i0 + i) % w.s->n;
            const int j1 = (j0 + j) % w.s->n;
            const int d = w.v->densities[i1][j1];
            for (int sensitivity = 1; sensitivity <= size; sensitivity++)
            {
                meshes[sensitivity - 1][i0][j0] += d / sensitivity;
            }
        }
    }
}

static void computeMeshes(struct W w, int size, int ***meshes)
{
    const int grain = (w.s->n + size - 1) / size;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            reportStatus("Computing", i * size + j, size * size);
            meshDensities(w, size, i, j, grain, meshes);
        }
    }
}

static void outputMesh(struct W w, int **mesh, int size, const char *outputFileName)
{
    FILE *outfile = openFile(outputFileName, "w");

    json_stream *stream = Create_JSON_Stream(outfile);

    jfprintf(stream, JSON_OBJECT_START);
    kv_jfprintf(stream, "d", JSON_ARRAY_START);
    for (int i = 0; i < size; i++)
    {
        jfprintf(stream, JSON_ARRAY_START);

        for (int j = 0; j < size; j++)
        {
            jfprintf(stream, JSON_INT, mesh[i][j]);
        }

        jfprintf(stream, JSON_ARRAY_END);
    }
    jfprintf(stream, JSON_ARRAY_END);

    jfprintf(stream, JSON_OBJECT_END);

    Destroy_JSON_Stream(stream);
    fclose(outfile);
}

void Trace_World(struct WorldSpec *ws, struct WorldView *wv, const char *name, const char *dir)
{
    const int size = 1024;
    struct W w = {ws, wv};
    char filePath[PATH_MAX];

    if (dir)
    {
        if (chdir(dir) != 0)
        {
            fprintf(stderr, "Could not chdir to %s\n", dir);
            exit(EXIT_FAILURE);
        };
    }

    sprintf(filePath, "%s.json", name);
    FILE *indexFile = openFile(filePath, "w");
    json_stream *stream = Create_JSON_Stream(indexFile);
    int ***meshes = malloc(sizeof(int ***) * size);

    jfprintf(stream, JSON_OBJECT_START);
    kv_jfprintf(stream, "n", JSON_INT, w.s->n);
    kv_jfprintf(stream, "v", JSON_INT, w.s->v);
    kv_jfprintf(stream, "width", JSON_INT, size);
    kv_jfprintf(stream, "height", JSON_INT, size);
    kv_jfprintf(stream, "levels", JSON_ARRAY_START);
    for (int level = 1; level <= size; level++)
    {
        char levelFile[PATH_MAX];
        sprintf(levelFile, "%s/level_%d.json", name, level);
        meshes[level - 1] = (int **)NewMatrix(sizeof(int), size, size);

        jfprintf(stream, JSON_OBJECT_START);
        kv_jfprintf(stream, "level", JSON_INT, level);
        kv_jfprintf(stream, "file", JSON_STRING, levelFile);
        jfprintf(stream, JSON_OBJECT_END);
    }
    jfprintf(stream, JSON_ARRAY_END);
    jfprintf(stream, JSON_OBJECT_END);
    fclose(indexFile);
    Destroy_JSON_Stream(stream);

    computeMeshes(w, size, meshes);

    for (int level = 1; level <= size; level++)
    {
        char levelFile[PATH_MAX];
        sprintf(levelFile, "%s/level_%d.json", name, level);

        reportStatus("Writing", level, size);

        outputMesh(w, meshes[level - 1], size, levelFile);
        DestroyMatrix((void **)meshes[level - 1]);
    }
    free(meshes);
}

#ifdef TEST
#include "test.h"
void TEST_TRACE_WORLD()
{
}
#endif