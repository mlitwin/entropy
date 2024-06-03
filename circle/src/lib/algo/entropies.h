#pragma once

#include "../genesis/worlddefs.h"

double shannon_entropy(int n, const int *states);

struct EntropyMeasures
{
    double mean_shannon;
    double mean_jitter;
    double mean_sensitivity;
};

double time_jitter(int n, const int *probabilities);

void max_mean_entropies(struct EntropyMeasures *oMeasures, const struct WorldSpec *s, const struct WorldView *v);

#ifdef TEST
void TEST_Entropies();
#endif