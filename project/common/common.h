#ifndef COMMON_H
#define COMMON_H

/// dictionary size values
#define size_dictionary 100000
#define size_out 1000000
#define size_word 100

int Find(int **in, int last_position, int *str, int size);
int CreateMask(int a, int b);

#endif // COMMON_H
