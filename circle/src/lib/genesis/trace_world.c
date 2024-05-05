#include "trace_world.h"

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../lib/types/matrix.h"
#include "../lib/stdio/json.h"
#include "../lib/stdio/util.h"
#include "../lib/mem/mem.h"

struct W
{
    struct WorldSpec *s;
    struct WorldView *v;
} W;

static int cmp_density(const void *a, const void *b)
{
    return *(int *)b - *(int *)a;
}

static void computeMeshPixel(struct W w, int i0, int j0, int grain_i, int grain_j, int size, int *buf, int ***meshes)
{
    int **densities = w.v->densities;
    const int n = w.s->n;
    const int sensitivity = w.s->sensitivity;
    const int period = w.s->period;
    const int buflen = grain_i * grain_j;

    int *b = buf;

    int leg1 = (j0 + grain_j <= n) ? grain_j : n - j0;
    int leg2 = grain_j - leg1;
    int cur, curCount;

    // printf("%d %d\n", i0, j0);

    for (int t = 0; t < grain_i; t++)
    {
        const int *row = densities[(t + i0 * grain_i) % period];
        memcpy(b, row + j0, leg1 * sizeof(int));
        b += leg1;
    }
    if (leg2 > 0)
    {
        for (int t = 0; t < grain_i; t++)
        {
            const int *row = densities[(t + i0 * grain_i) % period];
            memcpy(b, row, leg2 * sizeof(int));
            b += leg2;
        }
    }

    qsort(buf, buflen, sizeof(int), cmp_density);

    cur = -1;
    curCount = 0;
    for (int k = 0; k < buflen; k++)
    {
        const int val = buf[k];
        int emit = (k == buflen - 1);
        if (val == cur)
        {
            curCount++;
        }
        else
        {
            cur = val;
            curCount = 1;
            emit = 1;
        }

        if (emit)
        {
            int s;
            for (s = 0; s < sensitivity; s++)
            {
                const int v = val / (s + 1);
                if (v == 0)
                {
                    break;
                }
                meshes[s][i0][j0] += curCount * v;
            }
        }
    }
}

static void computeMeshes(struct W w, int size, int ***meshes)
{
    const int grain_i = (w.s->n + size - 1) / size;
    const int grain_j = (w.s->period + size - 1) / size;

    int *buf = mem_calloc(grain_i * grain_j, sizeof(int));
    for (int i = 0; i < size; i++)
    {
        reportStatus("Computing Meshes: Row ", i, size);

        for (int j = 0; j < size; j++)
        {
            computeMeshPixel(w, i, j, grain_i, grain_j, size, buf, meshes);
        }
    }
    free(buf);
}

static void outputMesh(int64_t ***meshes, int *buf, int level, int size, const char *outputFileName)
{
    FILE *outfile = openFile(outputFileName, "w");

    json_stream *stream = Create_JSON_Stream(outfile);

    jfprintf(stream, JSON_OBJECT_START);
    kv_jfprintf(stream, "d", JSON_ARRAY_START);
    for (int i = 0; i < size; i++)
    {
        // jfprintf(stream, JSON_ARRAY_START);
        for (int j = 0; j < size; j++)
        {
            const int val = Vector_Get(meshes[i][j], level);
            buf[j] = val;
            // jfprintf(stream, JSON_INT, val);
        }

        vec_jfprinf(stream, JSON_INT, size, buf);

        // jfprintf(stream, JSON_ARRAY_END);
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
    int size = 1024;
    const int levels = ws->sensitivity;
    struct W w = {ws, wv};
    char filePath[PATH_MAX];
    int *buf = mem_malloc(sizeof(int) * size);

    if (size > ws->n)
    {
        size = ws->n;
    }

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
    // int ***meshes = mem_malloc(sizeof(int ***) * size);

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
        // meshes[level] = (int **)NewMatrix(sizeof(int), size, size);

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

    // computeMeshes(w, size, meshes);

    for (int level = 0; level < levels; level++)
    {
        char levelFile[PATH_MAX];
        sprintf(levelFile, "%s/level_%d.json", name, level);

        reportStatus("Writing", level, levels);

        outputMesh(wv->meshes, buf, level, size, levelFile);
        // DestroyMatrix((void **)meshes[level]);
    }
    // DestroyMatrix((void **)mesh);
    free(buf);
    // free(meshes);
}

#ifdef TEST
#include "test.h"
void TEST_TRACE_WORLD()
{
}
#endif