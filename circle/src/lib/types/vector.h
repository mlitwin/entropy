#pragma once

#include <stdint.h>

int64_t *NewVector(int64_t capacity);
void DestroyVector(int64_t *v);
int Vector_GetLen(int64_t *v);
void Vector_Push(int64_t **v, const int64_t a);

#ifdef TEST
void TEST_Vector();
#endif