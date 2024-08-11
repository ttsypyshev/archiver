#include "compress.h"

/**
 * Counts the number of leading zeros in a 32-bit integer.
 *
 * @param tmp The integer to analyze.
 * @return The number of leading zeros.
 */
int CountZero(int tmp) {
    int count = 0;
    while (((tmp >> (31 - count)) & 0x1) == 0 && count < 32) {
        count++;
    }
    return 32 - count;
}

/**
 * Analyzes the compressed data to identify delimiters for more efficient compression.
 *
 * @param size_enc Pointer to the size of the compressed data.
 * @param enc Compressed data.
 * @return Modified compressed data with delimiter information.
 */
int *Analysis(int *size_enc, int enc[]) {
    int *tmp = SafeMalloc((*size_enc) * 2 * sizeof(int), "Initialize tmp (func Analysis)");
    int tmp_size = 0, type = 0;

    for (int index = 0; index < *size_enc; ++index) {
        int current_type = CountZero(enc[index]);
        if (type < current_type) {
            type = current_type;
            tmp[tmp_size++] = 0;
        }
        tmp[tmp_size++] = enc[index];
    }

    *size_enc = tmp_size;
    tmp = SafeRealloc(tmp, tmp_size * sizeof(int), "Reinitialize tmp (func Analysis)");
    return tmp;
}

/**
 * Compresses a file using a dictionary-based algorithm and bit-level operations.
 *
 * @param filename The name of the file to compress.
 * @param index Pointer to an integer that will be updated with the size of the compressed data.
 * @return Pointer to the compressed data.
 */
int *Compress1(const char filename[], int *index) {
    printf("compression_1 [ ");

    FILE *in = fopen(filename, "rb");
    if (!in) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    Dictionary dict;
    InitializeDictionary(&dict, SIZE_DICTIONARY);

    for (int i = 1; i < 256; i++) {
        dict.data[dict.size++][0] = i;
    }

    int *out = SafeMalloc(SIZE_OUT * sizeof(int), "Initialize out buffer (func Compress1)");
    int current_string[SIZE_WORD] = {0};
    int size = 0;
    int byte;

    int percent = 0;
    while ((byte = fgetc(in)) != EOF) {
        if (dict.size > SIZE_DICTIONARY * 0.1 * percent) {
            percent++;
            printf("#");
        }

        current_string[size++] = byte;
        if (Find(&dict, current_string, size) == -1) {
            memcpy(dict.data[dict.size], current_string, size * sizeof(int));
            dict.size++;
            out[(*index)++] = Find(&dict, current_string, size - 1);
            current_string[0] = byte;
            size = 1;
        }
    }
    out[(*index)++] = Find(&dict, current_string, size);

    FreeDictionary(&dict);
    fclose(in);

    printf("%.*s ]\n", 10 - percent, "##########");
    return out;
}

/**
 * Writes the compressed data to a file with buffering to optimize I/O operations.
 *
 * @param filename The name of the output file.
 * @param size_enc The size of the compressed data.
 * @param enc Compressed data.
 */
void Compress2(const char filename[], int size_enc, int enc[]) {
    printf("compression_2 [ ");

    FILE *encode_write = fopen(filename, "wb");
    if (!encode_write) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int byte = 0, count = 0, tmp = 0, tmp_len = 0;
    int mask = 0, result = 0;
    int type = 0, percent = 0;

    char buffer[BUFFER_SIZE];
    size_t buffer_pos = 0;

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
            } else {
                byte <<= tmp_len;
                count += tmp_len;
                tmp_len = 0;
            }
            byte += result;

            if (count == 8) {
                buffer[buffer_pos++] = byte;
                if (buffer_pos == BUFFER_SIZE) {
                    fwrite(buffer, 1, BUFFER_SIZE, encode_write);
                    buffer_pos = 0;
                }
                count = 0;
                byte = 0;
            }
        }
    }

    if (count > 0) {
        byte <<= 8 - count;
        buffer[buffer_pos++] = byte;
    }

    if (buffer_pos > 0) {
        fwrite(buffer, 1, buffer_pos, encode_write);
    }

    fclose(encode_write);
    printf("%.*s ]\n", 10 - percent, "##########");
}
