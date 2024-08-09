#include "common.h"
#include <stdio.h>

int Find(int **in, int last_position, int *str, int size) {
    for (int i = 0; i < last_position; ++i) {
        int flag = 1;
        for (int j = 0; j < size; ++j) {
            if (in[i][j] != str[j]) {
                flag = 0;
                break;
            }
        }
        if (flag) return i;
    }
    return -1;
}

int CreateMask(int a, int b) {
    int r = 0;
    for (int i = a; i <= b; ++i)
        r |= 1 << i;
    return r;
}
