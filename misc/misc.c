#include "misc.h"
#include "stdio.h"

bool is_allowed_char(char c) {
    return strchr(ALLOWED_SYMBOLS, c) != NULL;
}

bool sanitize_buffer(char *buffer, size_t size) {
    bool valid = true;
    size_t p = 0;

    for (size_t i = 0; i < size; ++i) {
        if (is_allowed_char(buffer[i])) buffer[p++] = buffer[i];
        else valid = false;
    }
    memset(buffer + p, '\0', size - p);

    return valid;
}

char *format_message(char *buffer, u_int64_t author) {
    char *new_buffer = malloc(strlen(buffer) + 16);
    sprintf(new_buffer, "%lx: %s", author, buffer);

    return new_buffer;
}