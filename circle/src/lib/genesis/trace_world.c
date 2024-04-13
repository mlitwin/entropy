#include "trace_world.h"

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../lib/types/matrix.h"
#include "../lib/stdio/json.h"
#include "../lib/stdio/util.h"

struct W
{
    struct WorldSpec *s;
    struct WorldView *v;
} W;

static void computeMeshes(struct W w, int size, int ***meshes)
{
    const int grain_n = (w.s->n + size - 1) / size;
    const int n = grain_n * size;
    const int grain_m = (w.s->period + size - 1) / size;
    const int m = grain_m * size;

    for (int i = 0; i < n; i++)
    {
        const int i0 = i % w.s->n;
        const int i_mesh = i / grain_n;
        reportStatus("Computing Meshes", i, n);

        for (int j = 0; j < m; j++)
        {
            const int j0 = j % w.s->period;

            const int d = w.v->densities[i0][j0];
            const int j_mesh = j / grain_m;
            for (int s = 0; s < w.s->sensitivity; s++)
            {
                const int v = d / (s + 1);
                if (v == 0)
                {
                    break;
                }
                meshes[s][i_mesh][j_mesh] += v;
            }
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

/*

    Sum_n -p[i] ln(p[i])
    Sum_n -c[i]/n ln(c[i]/n)
    -1/n Sum_n c[i](ln(c[i]) - ln(n))
    ln(n) - 1/n c[i]ln(c[])
*/
static double shannon_entropy(int n, int *states)
{
    double c = 0;
    double nd = 0;
    for (int i = 0; i < n; i++)
    {
        const double s = (double)states[i];
        nd += s;
        c += s * log(s);
    }

    return log(nd) - c / nd;
}

static double time_jitter(int n, int *probabilities)
{
    int jitter = 0;
    for (int i = 0; i < n; i++)
    {
        const int prev = (i == 0) ? n - 1 : i - 1;
        if (probabilities[i] > probabilities[prev])
        {
            jitter++;
        }
        else if (probabilities[i] < probabilities[prev])
        {
            jitter--;
        }
    }

    return (double)jitter / (double)n;
}

static void jfprintStates(json_stream *restrict stream, int n, int *states)
{
    kv_jfprintf(stream, "states", JSON_ARRAY_START);
    while (n--)
    {
        jfprintf(stream, JSON_INT, *(states++));
    }
    jfprintf(stream, JSON_ARRAY_END);
}

void Trace_World(struct WorldSpec *ws, struct WorldView *wv, const char *name, const char *dir)
{
    const int size = 1024;
    const int levels = ws->sensitivity;
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
    for (int level = 0; level < levels; level++)
    {
        char levelFile[PATH_MAX];
        sprintf(levelFile, "%s/level_%d.json", name, level);
        meshes[level] = (int **)NewMatrix(sizeof(int), size, size);

        jfprintf(stream, JSON_OBJECT_START);
        kv_jfprintf(stream, "level", JSON_INT, level);
        kv_jfprintf(stream, "file", JSON_STRING, levelFile);
        kv_jfprintf(stream, "num_states", JSON_INT, w.v->num_states[level]);
        kv_jfprintf(stream, "shannon_entropy", JSON_NUMBER, shannon_entropy(w.v->num_states[level], w.v->states[level]));
        kv_jfprintf(stream, "time_jitter", JSON_NUMBER, time_jitter(w.s->n, w.v->probabilities[level]));
        jfprintStates(stream, w.v->num_states[level], w.v->states[level]);
        jfprintf(stream, JSON_OBJECT_END);
    }
    jfprintf(stream, JSON_ARRAY_END);
    jfprintf(stream, JSON_OBJECT_END);
    fclose(indexFile);
    Destroy_JSON_Stream(stream);

    computeMeshes(w, size, meshes);

    for (int level = 0; level < levels; level++)
    {
        char levelFile[PATH_MAX];
        sprintf(levelFile, "%s/level_%d.json", name, level);

        reportStatus("Writing", level, levels);

        outputMesh(w, meshes[level], size, levelFile);
        DestroyMatrix((void **)meshes[level]);
    }
    free(meshes);
}

#ifdef TEST
#include "test.h"
void TEST_TRACE_WORLD()
{
}
#endif