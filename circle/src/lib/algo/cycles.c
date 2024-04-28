#include "cycles.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../mem/mem.h"

struct rleDigit
{
    int index;
    int value;
    int count;
};

struct digitRun
{
    int start;
    int len;
};

static int rleCmp(const struct rleDigit *a, const struct rleDigit *b)
{
    int ret = a->value - b->value;
    if (ret != 0)
    {
        return ret;
    }

    return a->count - b->count;
}

struct canonicalCycleShifter
{
    int n;
    struct rleDigit *digits;
    struct digitRun *maxRuns;
};

struct canonicalCycleShifter *createCanonicalCycleShifter(int n)
{
    struct canonicalCycleShifter *state = mem_calloc(sizeof(struct canonicalCycleShifter), 1);
    state->n = n;
    state->digits = mem_calloc(sizeof(struct rleDigit), n);
    state->maxRuns = mem_calloc(sizeof(struct digitRun), n);

    return state;
}

void destroyCanonicalCycleShifter(struct canonicalCycleShifter *state)
{
    free(state->digits);
    free(state->maxRuns);
    free(state);
}

static int computeRLE(struct canonicalCycleShifter *state, int *v, int sensitivity)
{
    int count = 0;
    state->digits[count].index = 0;
    state->digits[count].value = *v / sensitivity;
    v++;
    state->digits[count].count = 1;
    for (int i = 1; i < state->n; i++, v++)
    {
        if (state->digits[count].value == (*v / sensitivity))
        {
            state->digits[count].count++;
        }
        else
        {
            count++;
            state->digits[count].index = i;
            state->digits[count].value = *v / sensitivity;
            state->digits[count].count = 1;
        }
    }

    if (count > 0 && state->digits[0].value == state->digits[count].value)
    {
        state->digits[0].index += state->digits[count].index;
        state->digits[0].count += state->digits[count].count;
        count--;
    }

    return count + 1;
}

static int computeInitialDigitRun(struct canonicalCycleShifter *state, int digitCount)
{
    struct digitRun *maxRuns = state->maxRuns;
    struct rleDigit *digits = state->digits;
    int maxCount = 1;
    maxRuns[0].start = 0;
    maxRuns[0].len = 1;
    for (int i = 1; i < digitCount; i++)
    {
        const int cmp = rleCmp(digits, digits + i);

        if (cmp < 0)
        {
            maxRuns[0].start = i;
            maxRuns[0].len = 1;
        }
        else if (cmp == 0)
        {
            maxRuns[maxCount].start = i;
            maxRuns[maxCount].len = 1;
            maxCount++;
        }
    }
    return maxCount;
}

static int maxDigitRunCmp(const struct digitRun *a, const struct digitRun *b, const struct rleDigit *digits, int digitCount)
{
    int cmp = a->len - b->len;
    if (cmp == 0)
    {
        const int aLastDigit = (a->start + a->len - 1) % digitCount;
        const int bLastDigit = (b->start + b->len - 1) % digitCount;
        cmp = rleCmp(digits + aLastDigit, digits + bLastDigit);
    }
    return cmp;
}

int canonicalCycleShift(struct canonicalCycleShifter *state, int *v, int sensitivity)
{
    const int digitCount = computeRLE(state, v, sensitivity);
    const struct rleDigit *digits = state->digits;
    struct digitRun *maxRuns = state->maxRuns;
    int maxRunCount = computeInitialDigitRun(state, digitCount);

    while (maxRunCount > 1)
    {
        int cur = 0;
        int curMaxCount = 0;
        while (cur < maxRunCount)
        {
            const int nextDigitIndex = (maxRuns[cur].start + maxRuns[cur].len) % digitCount;
            const int next = (cur + 1) % maxRunCount;
            int advance;
            if (nextDigitIndex == maxRuns[next].start)
            {
                maxRuns[cur].len += maxRuns[next].len;
                advance = 2;
            }
            else
            {
                maxRuns[cur].len += 1;
                advance = 1;
            }

            if (cur == 0)
            {
                curMaxCount++;
            }
            else
            {
                const int cmp = maxDigitRunCmp(maxRuns, maxRuns + cur, digits, digitCount);

                if (cmp < 0)
                {
                    maxRuns[0] = maxRuns[cur];
                    curMaxCount = 1;
                }
                else if (cmp == 0)
                {
                    maxRuns[curMaxCount] = maxRuns[cur];
                    curMaxCount++;
                }
            }
            cur += advance;
        }
        maxRunCount = curMaxCount;
    }

    return digits[maxRuns[0].start].index;
}

// https://cp-algorithms.com/string/string-hashing.html
const int64_t m = 1000000009; /* (10^9 + 9) a prime */
const int64_t p = 1000000007; /* 10^9 + 7 a prime */
int64_t cycleHash(int *v, int sensitivity, int start, int len)
{
    int64_t ret = 0;
    int64_t a = 1;
    for (int i = 0; i < len; i++)
    {
        int index = (i + start) % len;
        int val = v[index] / sensitivity;
        ret += (val * a) % m;

        a = (a * p) % m;
    }
    return ret;
}

#ifdef TEST
#include "test.h"

static void testSomeCycles(int n, int **tests, int *expected)
{
    struct canonicalCycleShifter *shifter = createCanonicalCycleShifter(n);
    while (*tests)
    {
        const int shift = canonicalCycleShift(shifter, *tests, 1);
        if (shift != *expected)
        {
            FAIL("canonicalCycleShift expected %d got %d", *expected, shift);
        }

        tests++;
        expected++;
    }
    destroyCanonicalCycleShifter(shifter);
}

void TEST_Cycles()
{
    int *tests[] = {(int[]){0, 1, 2, 3, 0},
                    (int[]){9, 0, 9, 1, 0},
                    (int[]){9, 1, 2, 9, 2},
                    (int[]){9, 9, 9, 9, 0},
                    (int[]){9, 8, 9, 0, 0},
                    (int[]){9, 8, 9, 8, 9},
                    0};
    static int expected[] = {3, 2, 3, 0, 0, 4};

    testSomeCycles(5, tests, expected);
}
#endif