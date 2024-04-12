#pragma once

struct canonicalCycleShifter;

struct canonicalCycleShifter *createCanonicalCycleShifter(int n);
void destroyCanonicalCycleShifter(struct canonicalCycleShifter *state);
int canonicalCycleShift(struct canonicalCycleShifter *state, int *v);

#ifdef TEST
void TEST_Cycles();
#endif