#include "hash.h"


u_int64_t hash(const u_int8_t *data, size_t size) {
    u_int64_t hash = 5381;
    for (size_t i = 0; i < size; ++i) {
        hash = hash * 33 + data[i];
    }
    return hash;
}
