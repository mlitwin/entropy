#pragma once

int *NewVector(int n);
void DestroyVector(int *v);

void PrintVector(const int *v, int n);

#ifdef TEST
void TEST_Vector();
#endif
