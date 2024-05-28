#ifndef SERVER_MISC_H
#define SERVER_MISC_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../connection/connection.h"
#include "../definitions.h"


typedef enum {
    MESSAGE_CONNECTED,
    MESSAGE_DISCONNECTED,
    MESSAGE_SENT,
} MessageType;

bool sanitize_buffer(char *buffer, size_t size);

void format_message(char *message, char *result, Connection *connection, MessageType type);


#endif //SERVER_MISC_H
