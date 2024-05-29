#ifndef HASH_TABLE_TABLE_H
#define HASH_TABLE_TABLE_H


#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"


/**
 * Structure representing a key-value pair.
 *
 * This structure represents a key-value pair used in a key-value table (KVTable).
 * It contains pointers to the key and value associated with the pair.
 *
 * The structure fields are defined as follows:
 *  - key: A pointer to the key in the key-value pair.
 *  - value: A pointer to the value in the key-value pair.
 *
 * Example usage:
 * @code
 * KVItem item;
 * item.key = "example_key";
 * item.value = "example_value";
 * @endcode
 */
typedef struct {
    void *key;
    void *value;
} KVItem;


/**
 * Structure representing a key-value table.
 *
 * This structure represents a key-value table used for storing key-value pairs.
 * It contains information about the size of the table and a pointer to the storage array
 * containing the key-value pairs.
 *
 * The structure fields are defined as follows:
 *  - size: The size of the table, indicating the maximum number of key-value pairs it can store.
 *  - storage: A pointer to the array of KVItem structures representing the storage buffer.
 *
 * Example usage:
 * @code
 * KVTable table;
 * table.size = 100;
 * table.storage = (KVItem *)malloc(table.size * sizeof(KVItem));
 * @endcode
 */
typedef struct {
    size_t size;
    KVItem *storage;
} KVTable;


/**
 * Initializes a key-value table with the specified size.
 *
 * This function initializes a key-value table with the specified size.
 * It allocates memory for the table structure and the underlying storage array.
 *
 * @param size The number of elements in the key-value table.
 *
 * @return A pointer to the initialized key-value table, or NULL if memory allocation fails.
 *
 * The function performs the following steps:
 * 1. Allocates memory for the KVTable structure.
 * 2. Allocates memory for the storage array of KVItem elements.
 * 3. Initializes the size of the table.
 * 4. Returns a pointer to the initialized table if successful, or NULL if memory allocation fails.
 *
 * Example usage:
 * @code
 * size_t table_size = 100;
 * KVTable *table = init_table(table_size);
 * if (table == NULL) {
 *     // Error: Failed to initialize the table.
 * } else {
 *     // Successfully initialized the table.
 * }
 * @endcode
 */
KVTable *init_table(size_t size);


/**
 * Inserts or updates a key-value pair in the specified key-value table.
 *
 * This function inserts or updates a key-value pair in the specified key-value table.
 * It computes the hash value of the key to determine the index in the table's storage array
 * where the key-value pair should be stored.
 *
 * @param table A pointer to the KVTable structure representing the key-value table.
 * @param key A pointer to the key data.
 * @param size The size of the key data in bytes.
 * @param value A pointer to the value data.
 *
 * @return true if the key-value pair is successfully inserted or updated, otherwise false.
 *
 * The function performs the following steps:
 * 1. Computes the hash value of the key and calculates the initial index in the table's storage array.
 * 2. Iterates through the storage array using linear probing to find an empty slot or a slot
 *    containing the same key as the one being inserted or updated.
 * 3. If an empty slot is found, the key-value pair is inserted at that slot.
 *    If a slot with the same key is found, the value associated with the key is updated.
 * 4. Returns true if the operation is successful, indicating the key-value pair is inserted or updated.
 *    Returns false if the table is full and no empty slots are found.
 *
 * Example usage:
 * @code
 * KVTable *table;
 * char *key = "example_key";
 * size_t key_size = strlen(key);
 * int value = 42;
 * if (set_table(table, key, key_size, &value)) {
 *     // Key-value pair successfully inserted or updated.
 * } else {
 *     // Error: Failed to insert or update key-value pair.
 * }
 * @endcode
 */
bool set_table(KVTable *table, void *key, size_t size, void *value);


/**
 * Clears the value associated with the specified key in the key-value table.
 *
 * This function clears the value associated with the specified key in the key-value table
 * by setting the value to NULL. It is equivalent to updating the key-value pair with a NULL value.
 *
 * @param table A pointer to the KVTable structure representing the key-value table.
 * @param key A pointer to the key data.
 * @param size The size of the key data in bytes.
 *
 * @return true if the value associated with the key is successfully cleared, otherwise false.
 *
 * The function internally calls the set_table function with the specified key and a NULL value,
 * effectively clearing the value associated with the key in the table.
 * Returns true if the set operation is successful, indicating the value associated with the key
 * is cleared or updated to NULL. Returns false if the set operation fails.
 *
 * Example usage:
 * @code
 * KVTable *table;
 * char *key = "example_key";
 * size_t key_size = strlen(key);
 * if (clear_table(table, key, key_size)) {
 *     // Value associated with the key successfully cleared.
 * } else {
 *     // Error: Failed to clear value associated with the key.
 * }
 * @endcode
 */
bool get_table(KVTable *table, void *key, size_t size, void **result);


/**
 * Retrieves the value associated with the specified key from the key-value table.
 *
 * This function retrieves the value associated with the specified key from the key-value table.
 * It computes the hash value of the key to determine the index in the table's storage array
 * where the key-value pair is expected to be stored.
 *
 * @param table A pointer to the KVTable structure representing the key-value table.
 * @param key A pointer to the key data.
 * @param size The size of the key data in bytes.
 * @param result A pointer to a pointer where the retrieved value will be stored.
 *
 * @return true if the value associated with the key is successfully retrieved, otherwise false.
 *
 * The function performs the following steps:
 * 1. Computes the hash value of the key and calculates the initial index in the table's storage array.
 * 2. Checks if the key-value pair at the computed index matches the specified key.
 *    If it matches, retrieves the value associated with the key and stores it in the result pointer.
 * 3. Returns true if the value associated with the key is found and retrieved successfully.
 *    Returns false if the key is not found or if the table slot is empty.
 *
 * Example usage:
 * @code
 * KVTable *table;
 * char *key = "example_key";
 * size_t key_size = strlen(key);
 * void *value;
 * if (get_table(table, key, key_size, &value)) {
 *     // Value associated with the key retrieved successfully.
 * } else {
 *     // Error: Failed to retrieve value associated with the key.
 * }
 * @endcode
 */
bool clear_table(KVTable *table, void *key, size_t size);


/**
 * Frees the memory allocated for the key-value table and its storage array.
 *
 * This function frees the memory allocated for the key-value table structure
 * and the underlying storage array of key-value pairs.
 *
 * @param table A pointer to the KVTable structure representing the key-value table.
 *
 * The function performs the following steps:
 * 1. Frees the memory allocated for the storage array.
 * 2. Frees the memory allocated for the KVTable structure.
 *
 * Example usage:
 * @code
 * KVTable *table;
 * // Populate table with data
 * free_table(table);
 * @endcode
 */
void free_table(KVTable *table);


#endif //HASH_TABLE_TABLE_H
