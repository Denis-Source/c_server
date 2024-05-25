#include "table.h"


KVTable *table_init(size_t size) {
    KVTable *table = malloc(sizeof(KVTable));
    if (table == NULL) return NULL;

    table->size = size;
    table->_storage = (KVItem *) calloc(size, sizeof(KVItem));

    if (table->_storage == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

bool table_set(KVTable *table, void *key, size_t size, void *value) {
    u_int64_t hash_value = hash(key, size) % table->size;
    u_int64_t index;

    for (size_t i = 0; i < table->size; ++i) {
        index = (hash_value + i) % table->size;
        if (table->_storage[index].key == NULL) {
            table->_storage[index].key = key;
            table->_storage[index].value = value;
            return true;
        } else if (memcmp(key, table->_storage[index].key, size) == 0) {
            table->_storage[index].value = value;
            return true;
        }
    }

    return false;
}


bool table_clear(KVTable *table, void *key, size_t size) {
    return table_set(table, key, size, NULL);
}

bool table_get(KVTable *table, void *key, size_t size, void **result) {
    u_int64_t hash_value = hash(key, size) % table->size;
    u_int64_t index;

    if (table->_storage[hash_value].key == NULL) {
        *result = NULL;
        return false;
    }

    for (size_t i = 0; i < table->size; ++i) {
        index = (hash_value + i) % table->size;
        if (table->_storage[index].key == NULL || memcmp(key, table->_storage[index].key, size) != 0) continue;
        *result = table->_storage[index].value;
        return true;
    }

    *result = NULL;
    return false;
}

void table_free(KVTable *table) {
    free(table->_storage);
    free(table);
}
