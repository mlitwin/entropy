#pragma once

double shannon_entropy(int n, int *states);

double time_jitter(int n, int *probabilities);

#ifdef TEST
void TEST_Entropies();
#endif