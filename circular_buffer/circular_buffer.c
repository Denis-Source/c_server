#include "circular_buffer.h"

#define INVALID_ADDRESS -1

MessageBuffer *init_message_buffer(size_t size) {
    MessageBuffer *buffer = malloc(sizeof(MessageBuffer));
    if (buffer == NULL) return NULL;

    buffer->storage = malloc(size * sizeof(char*));
    if (buffer->storage == NULL) {
        free(buffer);
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        buffer->storage[i] = malloc(MESSAGE_SIZE * sizeof(char));
        if (buffer->storage[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(buffer->storage[j]);
            }
            free(buffer->storage);
            free(buffer);
            return NULL;
        }
    }

    buffer->head = INVALID_ADDRESS;
    buffer->size = size;
    return buffer;
}

bool add_messages_buffer(MessageBuffer *buffer, char *data) {
    bool replaced = false;

    buffer->head = (buffer->head + 1) % buffer->size;
    if (buffer->storage[buffer->head] != NULL) {
        buffer->tail = (buffer->tail + buffer->size - 1) % buffer->size;
        replaced = true;
    }
    memcpy(buffer->storage[buffer->head], data, MESSAGE_SIZE);
    return replaced;
}

bool get_messages_head_buffer(MessageBuffer *buffer, char *result, size_t index) {
    if (buffer->head == INVALID_ADDRESS) return false;
    if (index >= buffer->size) return false;

    size_t real_index = (buffer->head + buffer->size - index) % buffer->size;
    memcpy(result, buffer->storage[real_index], MESSAGE_SIZE);
    return true;
}

bool get_messages_tail_buffer(MessageBuffer *buffer, char *result, size_t index) {
    if (buffer->tail == INVALID_ADDRESS) return false;
    if (index >= buffer->size) return false;

    size_t real_index = (buffer->tail + buffer->size - index) % buffer->size;
    memcpy(result, buffer->storage[real_index], MESSAGE_SIZE);
    return true;
}

void free_messages_buffer(MessageBuffer *circularBuffer) {
    free(circularBuffer->storage);
    free(circularBuffer);
}
