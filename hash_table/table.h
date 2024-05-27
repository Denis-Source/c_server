#ifndef HASH_TABLE_TABLE_H
#define HASH_TABLE_TABLE_H

#include <stdbool.h>
#include <string.h>
#include "stdlib.h"

#include "hash.h"


typedef struct {
    void *key;
    void *value;
} KVItem;

typedef struct {
    size_t size;
    KVItem *storage;
} KVTable;

KVTable *table_init(size_t size);

bool table_set(KVTable *table, void *key, size_t size, void *value);

bool table_get(KVTable *table, void *key, size_t size, void **result);

bool table_clear(KVTable *table, void *key, size_t size);

void table_free(KVTable *table);

#endif //HASH_TABLE_TABLE_H
