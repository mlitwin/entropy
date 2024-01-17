#pragma once

typedef int **Matrix;

Matrix NewMatrix(int n, int m);
void DestroyMatrix(Matrix M, int n);

#ifdef TEST
void TEST_matrix();
#endif