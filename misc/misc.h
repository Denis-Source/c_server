#ifndef SERVER_MISC_H
#define SERVER_MISC_H


#include <stdbool.h>
#include "string.h"
#include "stdlib.h"


#define ALLOWED_SYMBOLS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?!,.-_\r\n "

bool sanitize_buffer(char *buffer, size_t size);

char *format_message(char* buffer, u_int64_t author);

#endif //SERVER_MISC_H
