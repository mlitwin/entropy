#pragma once

#include <stdio.h>

FILE *openFile(const char *filename, const char *mode);

int reportStatus(const char *step, int i, int n);