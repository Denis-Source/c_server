#include "formatting.h"


bool is_allowed_char(char c) {
    return strchr(MESSAGE_ALLOWED_SYMBOLS, c) != NULL;
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

void format_message(char *result, char *message, Connection *connection, MessageType type) {
    switch (type) {
        case MESSAGE_CONNECTED:
            sprintf(result, "%lx connected!\n", connection->name);
            break;
        case MESSAGE_DISCONNECTED:
            sprintf(result, "%lx disconnected!\n", connection->name);
            break;
        case MESSAGE_SENT:
            sprintf(result, "%lx: %s", connection->name, message);
            break;
        default:
            sprintf(result, "%s", message);
    }
    if (message[strlen(result) - 1 != '\n']) result[strlen(result) - 1] = '\n';
}
