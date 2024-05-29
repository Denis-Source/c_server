#include "table.h"


KVTable *init_table(size_t size) {
    KVTable *table = malloc(sizeof(KVTable));
    if (table == NULL) return NULL;

    table->storage = (KVItem *) calloc(size, sizeof(KVItem));
    if (table->storage == NULL) {
        free(table);
        return NULL;
    }
    table->size = size;

    return table;
}

bool set_table(KVTable *table, void *key, size_t size, void *value) {
    u_int64_t hash_value = hash(key, size) % table->size;
    u_int64_t index;

    for (size_t i = 0; i < table->size; ++i) {
        index = (hash_value + i) % table->size;
        if (table->storage[index].key == NULL) {
            table->storage[index].key = key;
            table->storage[index].value = value;
            return true;
        } else if (memcmp(key, table->storage[index].key, size) == 0) {
            table->storage[index].value = value;
            return true;
        }
    }

    return false;
}

bool clear_table(KVTable *table, void *key, size_t size) {
    return set_table(table, key, size, NULL);
}

bool get_table(KVTable *table, void *key, size_t size, void **result) {
    u_int64_t hash_value = hash(key, size) % table->size;
    u_int64_t index;

    if (table->storage[hash_value].key == NULL) {
        *result = NULL;
        return false;
    }

    for (size_t i = 0; i < table->size; ++i) {
        index = (hash_value + i) % table->size;
        if (table->storage[index].key == NULL || memcmp(key, table->storage[index].key, size) != 0) continue;
        *result = table->storage[index].value;
        return true;
    }

    *result = NULL;
    return false;
}

void free_table(KVTable *table) {
    free(table->storage);
    free(table);
}
