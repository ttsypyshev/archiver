#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decompress.h"
#include "common.h"

/// dictionary size values
#define size_dictionary 100000
#define size_out 1000000
#define size_word 100

///// search for a word in the dictionary
//int Find(int **in, int last_position, int *str, int size) {
//    for (int i = 0; i < last_position; ++i) {
//        int flag = 1;
//        for (int j = 0; j < size; ++j) {
//            if (in[i][j] != str[j]) {
//                flag = 0;
//                break;
//            }
//        }
//        // if found, then output the index
//        if (flag) return i;
//    }
//    // if not found, then output -1
//    return -1;
//}
//
///// creating a bit mask
//int CreateMask(int a, int b) {
//    int r = 0;
//    for (int i = a; i <= b; ++i)
//        r |= 1 << i;
//    return r;
//}

/// saving the decoded text to a file
void Output(const char filename[], int **dec, int size_dec) {
    FILE *new_write = fopen(filename, "wb");
    if (new_write == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size_dec; ++i) {
        for (int j = 0; j < size_word; ++j) {
            if (dec[i][j] != 0) {
                fputc(dec[i][j], new_write);
            }
        }
    }

    fclose(new_write);
}

/// the function decodes the phrase
int **Decompress1(int *in, int size_in, int *index) {
    printf("decompression_2 [ ");

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

    int **out = malloc(size_out * sizeof(int *));
    if (out == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size_out; ++i) {
        out[i] = calloc(size_word, sizeof(int));
        if (out[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    (*index)++;

    int percent = 0;

    for (int num = 0; num < size_in; ++num) {
        if (num > size_in * 0.1 * percent) {
            percent++;
            printf("#");
        }

        int tmp[size_word] = {0};
        int size = 0;

        for (size = 0; out[(*index) - 1][size] != 0; ++size)
            tmp[size] = out[(*index) - 1][size];

        if (in[num] < last_position)
            tmp[size++] = dictionary[in[num]][0];
        else
            tmp[size++] = out[(*index) - 1][0];

        if (Find(dictionary, last_position, tmp, size) == -1) {
            for (int i = 0; i < size; ++i) dictionary[last_position][i] = tmp[i];
            last_position++;
        }

        for (int i = 0; i < size_word; ++i) {
            out[*index][i] = dictionary[in[num]][i];
        }
        (*index)++;
    }
    printf("%.*s ]\n", 10 - percent, "##########");

    for (int i = 0; i < size_dictionary; i++) free(dictionary[i]);
    free(dictionary);

    return out;
}

/// the function decodes the phrase
int *Decompress2(const char filename[], int *size_dec) {
    int type = 1, num = 0;
    int byte = 0, tmp = 0, count = 0;
    int result = 0, mask = 0;

    FILE *encode_read = fopen(filename, "rb");
    if (encode_read == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    printf("decompression_1 [ ");

    fseek(encode_read, 0, SEEK_END);
    int enc_size = ftell(encode_read);
    fseek(encode_read, 0, SEEK_SET);

    int *dec = calloc(enc_size, sizeof(int));
    if (dec == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int percent = 0;

    while (num++ <= enc_size) {
        if (num - 1 > enc_size * 0.1 * percent) {
            percent++;
            printf("#");
        }

        byte = fgetc(encode_read);
        if (byte == EOF) break;

        tmp <<= 8;
        mask = CreateMask(0, 7);
        tmp += mask & byte;
        count += 8;

        while (count > type) {
            mask = CreateMask(count - type, count - 1);
            result = mask & tmp;
            result >>= count - type;

            if (result == 0) {
                while (result == 0 && type != count + 1) {
                    type++;
                    mask = CreateMask(count - type, count - 1);
                    result = mask & tmp;
                    result >>= count - type;
                }
                type--;
                result >>= 1;
            } else {
                dec[(*size_dec)++] = result;
            }
            count -= type;
        }
    }
    printf("%.*s ]\n", 10 - percent, "##########");

    fclose(encode_read);
    return dec;
}

//int main() {
//    printf("Enter the path to the encoding file (press \"Enter\" to complete the input):\n");
//    char filename[100] = {0};
//    scanf("%s", filename); // e.g., ../files/encode.txt
//
//    int size_dec = 0;
//    int *dec = Decompress2(filename, &size_dec); // decompress 2 method
//
//    int size_dec1 = 0;
//    int **dec1 = Decompress1(dec, size_dec, &size_dec1); // decompress 1 method
//    free(dec);
//
//    printf("\nEnter the path to the new file (press \"Enter\" to complete the input):\n");
//    scanf("%s", filename); // e.g., ../files/new_file.txt
//
//    Output(filename, dec1, size_dec1); // output the decoded text
//    free(dec1);
//
//    return 0;
//}