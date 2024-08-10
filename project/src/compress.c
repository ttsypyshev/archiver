#include <stdio.h>
#include <stdlib.h>

#include "compress.h"

/// determining the number of insignificant zeros
int CountZero(int tmp) {
    int flag = 1, count = 0;
    // we go through the bits until the bit is 0
    for (int position = sizeof(int) * 8 - 1; position >= 0; --position) {
        if (((tmp >> position) & 0x1) == 0 && flag) count++;
        else break;
    }
    return 32 - count;
}

/// delimiting a <enc1> for 2 compression
// adding delimiters ('0') to <enc> when increasing the number of significant bits
int *Analysis(int *size_enc, int enc[]) {
    int *tmp = malloc((*size_enc + *size_enc) * sizeof(int));
    if (tmp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int tmp_size = 0, type = 0;
    for (int index = 0; index < *size_enc; ++index) {
        if (type < CountZero(enc[index])) {
            type = CountZero(enc[index]);
            tmp[tmp_size++] = 0;
        }
        tmp[tmp_size++] = enc[index];
    }

    *size_enc = tmp_size;
    tmp = realloc(tmp, tmp_size * sizeof(int));
    if (tmp == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    return tmp;
}

/// the function compresses the text
int *Compress1(const char filename[], int *index) {
    FILE *in = fopen(filename, "rb");
    if (in == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int **dictionary = malloc(size_dictionary * sizeof(int *));
    if (dictionary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size_dictionary; i++) {
        dictionary[i] = calloc(size_word, sizeof(int));
        if (dictionary[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    int last_position = 0;
    for (int i = 1; i < 256; i++) dictionary[last_position++][0] = i;

    int *out = malloc(size_out * sizeof(int));
    if (out == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int byte = 0, num = 0;
    int current_string[size_word] = {0};
    int size = 0;
    int percent = 0;

    printf("compression [ ");
    while ((byte = fgetc(in)) != EOF) {
        if (last_position > size_dictionary * 0.1 * percent) {
            percent++;
            printf("#");
        }

        num++;
        current_string[size++] = byte;
        if (Find(dictionary, last_position, current_string, size) == -1) {
            for (int i = 0; i < size_word; ++i) dictionary[last_position][i] = current_string[i];
            last_position++;
            out[(*index)++] = Find(dictionary, last_position, current_string, size - 1);

            for (int i = 0; i < size_word; ++i) current_string[i] = 0;
            current_string[0] = byte;
            size = 1;
        }
    }
    printf("%.*s ]\n", 10 - percent, "##########");
    out[(*index)++] = Find(dictionary, last_position, current_string, size);

    for (int i = 0; i < size_dictionary; i++) free(dictionary[i]);
    free(dictionary);
    fclose(in);
    return out;
}

/// the function compresses the text
void Compress2(const char filename[], int size_enc, int enc[]) {
    FILE *encode_write = fopen(filename, "wb");
    if (encode_write == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int byte = 0, count = 0, tmp = 0, tmp_len = 0;
    int mask = 0, result = 0;
    int type = 0, percent = 0;

    printf("saving [ ");
    for (int i = 0; i < size_enc; ++i) {
        if (i > size_enc * 0.1 * percent) {
            percent++;
            printf("#");
        }

        if (enc[i] == 0) {
            type = CountZero(enc[i + 1]);
        }
        tmp = enc[i];
        tmp_len = type;

        while (tmp_len != 0) {
            mask = CreateMask(0, tmp_len - 1);
            result = mask & tmp;
            if (count + tmp_len > 8) {
                result >>= tmp_len - (8 - count);
                tmp_len -= (8 - count);
                byte <<= 8 - count;
                count = 8;
                mask = CreateMask(0, tmp_len - 1);
                tmp = mask & tmp;
            } else {
                byte <<= tmp_len;
                count += tmp_len;
                tmp_len = 0;
                tmp = 0;
            }
            byte += result;

            if (count == 8) {
                fputc(byte, encode_write);
                count = 0;
                byte = 0;
            }
        }
    }
    printf("%.*s ]\n", 10 - percent, "##########");

    byte <<= 8 - count;
    fputc(byte, encode_write);

    fclose(encode_write);
}
