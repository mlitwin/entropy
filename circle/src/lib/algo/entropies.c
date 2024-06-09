#include "entropies.h"

#include <math.h>

/*

    Sum_n -p[i] ln(p[i])
    Sum_n -c[i]/n ln(c[i]/n)
    -1/n Sum_n c[i](ln(c[i]) - ln(n))
    ln(n) - 1/n c[i]ln(c[])
*/
double shannon_entropy(int n, const int *states)
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

double time_jitter(int n, const int *probabilities)
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

static double energy(int n, int v, const int **d)
{
    double e = 0;
    const int num_v = 2 * v + 1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < num_v; j++)
        {
            const double velocity = j - v;
            e += velocity * velocity;
        }
    }
    return e / 2.0;
}

void max_mean_entropies(struct EntropyMeasures *oMeasures, const struct WorldSpec *s, const struct WorldView *v, int **d)
{
    double total_jitter = 0;

    double mean_entropy = 0;
    double mean_sensitivity = 0;

    for (int l = 0; l < s->sensitivity; l++)
    {
        double jitter = time_jitter(s->n, v->probabilities[l]);
        if (jitter < 0)
        {
            jitter = -jitter;
        }
        total_jitter += jitter;
    }

    for (int l = 0; l < s->sensitivity; l++)
    {
        double jitter = time_jitter(s->n, v->probabilities[l]);
        if (jitter < 0)
        {
            jitter = -jitter;
        }

        mean_entropy += shannon_entropy(v->num_states[l], v->states[l]) * jitter;
        mean_sensitivity += (l + 1) * jitter;
    }

    if (total_jitter > 0)
    {
        mean_entropy /= total_jitter;
        mean_sensitivity /= total_jitter;
    }

    oMeasures->s = *s;
    oMeasures->energy = energy(s->n, s->v, d);
    oMeasures->base_shannon = shannon_entropy(v->num_states[0], v->states[0]);
    oMeasures->mean_jitter = total_jitter / s->sensitivity;
    oMeasures->mean_shannon = mean_entropy;
    oMeasures->mean_sensitivity = mean_sensitivity;
}

#ifdef TEST
#include "test.h"

void TEST_Entropies()
{
}
#endif