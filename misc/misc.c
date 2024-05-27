#include "misc.h"


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

void format_message(char *message, Connection *connection, MessageType type) {
    char temp_buffer[strlen(message) + MESSAGE_FORMATTING_SIZE];
    strcpy(temp_buffer, message);
    if (message[strlen(message) - 1 != '\n']) temp_buffer[strlen(message) - 1] = '\n';
    switch (type) {
        case MESSAGE_CONNECTED:
            sprintf(message, "%lx connected!\n", connection->name);
            break;
        case MESSAGE_DISCONNECTED:
            sprintf(message, "%lx disconnected!\n", connection->name);
            break;
        case MESSAGE_SENT:
            sprintf(message, "%lx: %s", connection->name, temp_buffer);
            break;
        default:
            sprintf(message, "%s", temp_buffer);
    }
}
