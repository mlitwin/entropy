#include "entropies.h"

#include <math.h>

/*

    Sum_n -p[i] ln(p[i])
    Sum_n -c[i]/n ln(c[i]/n)
    -1/n Sum_n c[i](ln(c[i]) - ln(n))
    ln(n) - 1/n c[i]ln(c[])
*/
double shannon_entropy(int n, int *states)
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

double time_jitter(int n, int *probabilities)
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

#ifdef TEST
#include "test.h"

void TEST_Entropies()
{
}
#endif