#pragma once

typedef void **Matrix;

Matrix NewMatrix(int size, int n, int m);
void DestroyMatrix(Matrix m);

#ifdef TEST
void TEST_Matrix();
#endif