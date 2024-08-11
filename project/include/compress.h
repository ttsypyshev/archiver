#ifndef COMPRESS_H
#define COMPRESS_H

#include "common.h"

int *Compress1(const char filename[], int *index);

int *Analysis(int *size_enc, int enc[]);

void Compress2(const char filename[], int size_enc, int enc[]);

#endif // COMPRESS_H
