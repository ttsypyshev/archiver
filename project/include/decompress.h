#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "common.h"

int *Decompress1(const char filename[], int *size_dec);

int **Decompress2(int *in, int size_in, int *index);

void Output(const char filename[], int **dec, int size_dec);

#endif // DECOMPRESS_H
