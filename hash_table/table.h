#ifndef HASH_TABLE_TABLE_H
#define HASH_TABLE_TABLE_H


#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"


typedef struct {
    void *key;
    void *value;
} KVItem;

typedef struct {
    size_t size;
    KVItem *storage;
} KVTable;

KVTable *init_table(size_t size);

bool set_table(KVTable *table, void *key, size_t size, void *value);

bool get_table(KVTable *table, void *key, size_t size, void **result);

bool clear_table(KVTable *table, void *key, size_t size);

void free_table(KVTable *table);


#endif //HASH_TABLE_TABLE_H
