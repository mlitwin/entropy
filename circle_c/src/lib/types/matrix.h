#pragma once

typedef int **Matrix;

Matrix NewMatrix(int n, int m);
void DestroyMatrix(Matrix m);

#ifdef TEST
void TEST_matrix();
#endif