#ifndef SERVER_CIRCULAR_BUFFER_H
#define SERVER_CIRCULAR_BUFFER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../definitions.h"

typedef struct {
    u_int16_t size;
    int32_t head;
    int32_t tail;
    char **storage;
} MessageBuffer;

MessageBuffer *init_message_buffer(size_t size);

bool add_messages_buffer(MessageBuffer *buffer, char *data);

bool get_messages_head_buffer(MessageBuffer *buffer, char *result, size_t index);

bool get_messages_tail_buffer(MessageBuffer *buffer, char *result, size_t index);

void free_messages_buffer(MessageBuffer *circularBuffer);

#endif //SERVER_CIRCULAR_BUFFER_H
