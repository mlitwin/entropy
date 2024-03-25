#pragma once

#include "../types/matrix.h"

struct World;

struct World *CreateNeWorld(int n, int v, int density, int precision);

void DestroyWorld(struct World *w);

void RunWorld(struct World *w);
void BeholdWorld(struct World *w);

void PrintWorld(const struct World *w);

#ifdef TEST
void TEST_World();
#endif