#include "secrets.h"


u_int64_t static_generate_random() {
    static u_int64_t value = 0;
    if (value == 0) {
        srand((u_int32_t)(time(NULL) ^ getpid()));
        do {
            value = ((u_int64_t)rand() << 32) | rand();
        } while (value == 0);
    }
    return value;
}