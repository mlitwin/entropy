#pragma once

void **NewMatrix(int size, int n, int m);
void DestroyMatrix(void **m);

#ifdef TEST
void TEST_Matrix();
#endif