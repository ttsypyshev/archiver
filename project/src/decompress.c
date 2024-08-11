#include "decompress.h"

/**
 * Writes decompressed data to a file with buffering to optimize I/O operations.
 *
 * @param filename The name of the output file.
 * @param dec Decompressed data in a two-dimensional array.
 * @param size_dec The size of the decompressed data.
 */
void Output(const char *filename, int **dec, int size_dec) {
    FILE *new_write = fopen(filename, "wb");
    if (!new_write) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    uint8_t *buffer = SafeMalloc(SIZE_OUT, "Initialize buffer output (func Output)");

    int buffer_index = 0;
    for (int i = 0; i < size_dec; ++i) {
        for (int j = 0; j < SIZE_WORD; ++j) {
            if (dec[i][j] != 0) {
                buffer[buffer_index++] = (uint8_t) dec[i][j];
                if (buffer_index == SIZE_OUT) {
                    fwrite(buffer, 1, buffer_index, new_write);
                    buffer_index = 0;
                }
            }
        }
    }

    if (buffer_index > 0) {
        fwrite(buffer, 1, buffer_index, new_write);
    }

    free(buffer);
    fclose(new_write);
}

/**
 * Decompresses a file using bit-level operations and efficient memory management.
 *
 * @param filename The name of the input file.
 * @param size_dec Pointer to an integer that will hold the size of the decompressed data.
 * @return Pointer to the decompressed data.
 */
int *Decompress1(const char *filename, int *size_dec) {
    printf("decompression_1 [ ");

    FILE *encode_read = fopen(filename, "rb");
    if (!encode_read) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fseek(encode_read, 0, SEEK_END);
    int enc_size = ftell(encode_read);
    fseek(encode_read, 0, SEEK_SET);

    int *dec = SafeMalloc(enc_size * sizeof(int), "Initialize dec (func Decompress1)");
    int type = 1, num = 0;
    int byte = 0, tmp = 0, count = 0;
    int result = 0, mask = 0, percent = 0;

    while ((byte = fgetc(encode_read)) != EOF) {
        if (num++ > enc_size * 0.1 * percent) {
            percent++;
            printf("#");
        }

        tmp = (tmp << 8) | (byte & 0xFF);
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

    fclose(encode_read);
    printf("%.*s ]\n", 10 - percent, "##########");

    return dec;
}

/**
 * Decompresses data using a dictionary-based approach with dynamic memory management.
 *
 * @param in Compressed input data.
 * @param size_in The size of the compressed data.
 * @param index Pointer to an integer that will be updated with the size of the decompressed data.
 * @return Pointer to a two-dimensional array containing the decompressed data.
 */
int **Decompress2(int *in, int size_in, int *index) {
    printf("decompression_2 [ ");

    Dictionary dict;
    InitializeDictionary(&dict, SIZE_DICTIONARY);
    for (int i = 1; i < 256; i++) dict.data[dict.size++][0] = i;

    int **out = SafeMalloc(SIZE_OUT * sizeof(int *), "Initialize buffer output (func Decompress2)");
    for (int i = 0; i < SIZE_OUT; ++i) {
        out[i] = calloc(SIZE_WORD, sizeof(int));
    }

    (*index)++;
    int percent = 0;
    for (int num = 0; num < size_in; ++num) {
        if (num > size_in * 0.1 * percent) {
            percent++;
            printf("#");
        }

        int tmp[SIZE_WORD] = {0};
        int size = 0;

        while (out[(*index) - 1][size] != 0) {
            ++size;
        }

        memcpy(tmp, out[(*index) - 1], size * sizeof(int));
        tmp[size++] = (in[num] < dict.size) ? dict.data[in[num]][0] : out[(*index) - 1][0];

        if (Find(&dict, tmp, size) == -1) {
            memcpy(dict.data[dict.size], tmp, size * sizeof(int));
            dict.size++;
        }

        memcpy(out[*index], dict.data[in[num]], SIZE_WORD * sizeof(int));
        (*index)++;
    }

    FreeDictionary(&dict);

    printf("%.*s ]\n", 10 - percent, "##########");
    return out;
}
