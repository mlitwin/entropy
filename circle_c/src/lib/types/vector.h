#pragma once

typedef int *Vector;

Vector NewVector(int n);
void DestroyVector(Vector v);

#ifdef TEST
void TEST_Vector();
#endif
