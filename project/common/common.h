#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Maximum number of entries in the dictionary. Adjust this size based on memory constraints.
#define SIZE_DICTIONARY 100000

// Maximum size of the output buffer. This defines how much space is allocated for compressed or processed data.
#define SIZE_OUT 1000000

// Maximum length of a single word or string that the algorithm can handle. Used in the dictionary operations.
#define SIZE_WORD 100

// Size of the buffer used for I/O operations, such as reading from or writing to files. Adjust this for optimal performance.
#define BUFFER_SIZE 8192

// Structure representing a dictionary used in compression/decompression algorithms.
// 'data' is a two-dimensional array storing the dictionary entries (each entry is an array of integers).
// 'size' keeps track of the current number of entries in the dictionary.
typedef struct {
    int **data;  // Pointer to the dictionary data (array of integer arrays)
    int size;    // Current number of entries in the dictionary
} Dictionary;

int Find(Dictionary *dict, int *str, int size);

int CreateMask(int a, int b);

void InitializeDictionary(Dictionary *dict, int size);

void FreeDictionary(Dictionary *dict);

void *SafeMalloc(size_t size, const char *text_error);

void *SafeCalloc(size_t nmemb, size_t size, const char *text_error);

void *SafeRealloc(void *ptr, size_t size, const char *text_error);

#endif // COMMON_H
