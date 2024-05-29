#ifndef HASH_TABLE_HASH_H
#define HASH_TABLE_HASH_H


#include <stdlib.h>


/**
 * Computes a hash value for the given data using the djb2 algorithm.
 *
 * This function computes a hash value for the given data using the djb2 algorithm,
 * which is a simple, non-cryptographic hash function. It iterates over each byte
 * of the data and updates the hash value based on the current byte and the previous hash value.
 *
 * @param data A pointer to an array of bytes representing the data to be hashed.
 * @param size The size of the data array in bytes.
 *
 * @return The computed hash value as a 64-bit unsigned integer.
 *
 * The function performs the following steps:
 * 1. Initializes the hash value to 5381.
 * 2. Iterates through each byte of the data array.
 * 3. Updates the hash value using the djb2 algorithm: hash = hash * 33 + data[i].
 * 4. Returns the computed hash value.
 *
 * Example usage:
 * @code
 * const char *data = "hello";
 * size_t size = strlen(data);
 * u_int64_t h = hash((const u_int8_t *)data, size);
 * printf("Hash value: %llu\n", h);
 * @endcode
 */
u_int64_t hash(const u_int8_t *data, size_t size);


#endif //HASH_TABLE_HASH_H
