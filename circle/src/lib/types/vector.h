#pragma once

typedef int *Vector;

Vector NewVector(int n);
void DestroyVector(Vector v);

void PrintVector(const Vector v, int n);

#ifdef TEST
void TEST_Vector();
#endif
