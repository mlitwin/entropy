#pragma once

double shannon_entropy(int n, const int *states);

struct EntropyMeasures
{
    double mean_shannon;
    double mean_jitter;
    double mean_sensitivity;
};

double time_jitter(int n, const int *probabilities);
void max_mean_entropies(struct EntropyMeasures *oMeasures, int n, int levels, const int *num_states, int *const *states, int *const *probabilities);

#ifdef TEST
void TEST_Entropies();
#endif