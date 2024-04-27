#pragma once

int *NewVector(int capacity);
void DestroyVector(int *v);
int Vector_GetLen(int *v);
void Vector_Push(int **v, const int a);

#ifdef TEST
void TEST_Vector();
#endif