#include "common.h"

/**
 * Searches for a string in the dictionary.
 *
 * @param dict Pointer to the dictionary.
 * @param str The string to search for.
 * @param size The size of the string.
 * @return The index of the string in the dictionary, or -1 if not found.
 */
int Find(Dictionary *dict, int *str, int size) {
    for (int i = 0; i < dict->size; ++i) {
        if (memcmp(dict->data[i], str, size * sizeof(int)) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Creates a bitmask for extracting specific bits from a number.
 *
 * @param a The starting bit position (inclusive).
 * @param b The ending bit position (inclusive).
 * @return The created bitmask.
 */
int CreateMask(int a, int b) {
    return ((1 << (b - a + 1)) - 1) << a;
}

/**
 * Initializes a dictionary with a specified size.
 *
 * @param dict Pointer to the dictionary to initialize.
 * @param size The size of the dictionary.
 */
void InitializeDictionary(Dictionary *dict, int size) {
    dict->data = SafeMalloc(size * sizeof(int *), "Initialize dictionary (layer 1)");
    for (int i = 0; i < size; ++i) {
        dict->data[i] = SafeCalloc(SIZE_WORD, sizeof(int), "Initialize dictionary (layer 2)");
    }
    dict->size = 0;
}

/**
 * Frees the memory allocated for a dictionary.
 *
 * @param dict Pointer to the dictionary to free.
 */
void FreeDictionary(Dictionary *dict) {
    if (dict && dict->data) {
        for (int i = 0; i < dict->size; ++i) {
            free(dict->data[i]);
        }
        free(dict->data);
        dict->data = NULL;
    }
}

/**
 * Allocates memory and checks for allocation failures.
 *
 * @param size The size of the memory to allocate.
 * @param text_error A message describing the error if allocation fails.
 * @return Pointer to the allocated memory.
 */
void *SafeMalloc(size_t size, const char *text_error) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed. Error: %s\n", text_error);
        perror("System error");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**
 * Allocates zero-initialized memory and checks for allocation failures.
 *
 * @param nmemb The number of elements to allocate.
 * @param size The size of each element.
 * @param text_error A message describing the error if allocation fails.
 * @return Pointer to the allocated memory.
 */
void *SafeCalloc(size_t nmemb, size_t size, const char *text_error) {
    void *ptr = calloc(nmemb, size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed. Error: %s\n", text_error);
        perror("System error");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/**
 * Reallocates memory and checks for reallocation failures.
 *
 * @param ptr Pointer to the memory to reallocate.
 * @param size The new size of the memory block.
 * @param text_error A message describing the error if reallocation fails.
 * @return Pointer to the reallocated memory.
 */
void *SafeRealloc(void *ptr, size_t size, const char *text_error) {
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        fprintf(stderr, "Memory reallocation failed. Error: %s\n", text_error);
        perror("System error");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}
