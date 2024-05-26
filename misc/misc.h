#ifndef SERVER_MISC_H
#define SERVER_MISC_H


#include <stdbool.h>
#include "string.h"

#define ALLOWED_SYMBOLS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?!,.-_\r\n "

bool sanitize_buffer(char *buffer, size_t size);

#endif //SERVER_MISC_H
