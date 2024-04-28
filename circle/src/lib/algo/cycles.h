#pragma once

#include <stdint.h>

struct canonicalCycleShifter;

struct canonicalCycleShifter *createCanonicalCycleShifter(int n);
void destroyCanonicalCycleShifter(struct canonicalCycleShifter *state);
int canonicalCycleShift(struct canonicalCycleShifter *state, int *v, int sensitivity);
int64_t cycleHash(int *v, int sensitivity, int start, int len);

#ifdef TEST
void TEST_Cycles();
#endif