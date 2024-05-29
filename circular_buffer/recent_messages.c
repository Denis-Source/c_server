#include "recent_messages.h"


RecentMessages *init_recent_messages(size_t size) {
    RecentMessages *buffer = malloc(sizeof(RecentMessages));
    if (buffer == NULL) return NULL;

    buffer->storage = malloc(size * sizeof(char *));
    if (buffer->storage == NULL) {
        free(buffer);
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        buffer->storage[i] = malloc(MESSAGE_SIZE * sizeof(char));
        if (buffer->storage[i] == NULL) {
            // Free previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(buffer->storage[j]);
            }
            free(buffer->storage);
            free(buffer);
            return NULL;
        }
    }

    buffer->head = INVALID_ADDRESS;
    buffer->tail = INVALID_ADDRESS;
    buffer->size = size;
    return buffer;
}

bool add_recent_messages(RecentMessages *recent_messages, char *data) {
    bool replaced = false;

    if (recent_messages->head == INVALID_ADDRESS) {
        recent_messages->head = 0;
        recent_messages->tail = 0;
    } else {
        recent_messages->head = (recent_messages->head + 1) % recent_messages->size;
        if (recent_messages->head == recent_messages->tail) {
            recent_messages->tail = (recent_messages->tail + 1) % recent_messages->size;
            replaced = true;
        }
    }
    memcpy(recent_messages->storage[recent_messages->head], data, MESSAGE_SIZE);
    return replaced;
}

bool get_head_recent_messages(RecentMessages *recent_messages, char *result, size_t index) {
    if (recent_messages->head == INVALID_ADDRESS) return false;
    if (index >= recent_messages->size) return false;

    size_t real_index = (recent_messages->head + recent_messages->size - index) % recent_messages->size;
    memcpy(result, recent_messages->storage[real_index], MESSAGE_SIZE);
    return true;
}

bool get_tail_recent_messages(RecentMessages *recent_messages, char *result, size_t index) {
    if (recent_messages->tail == INVALID_ADDRESS) return false;
    if (index >= recent_messages->size) return false;

    size_t real_index = (recent_messages->tail + index) % recent_messages->size;
    memcpy(result, recent_messages->storage[real_index], MESSAGE_SIZE);
    return true;
}

void free_recent_messages(RecentMessages *recent_messages) {
    for (size_t i = 0; i < recent_messages->size; i++) {
        free(recent_messages->storage[i]);
    }
    free(recent_messages->storage);
    free(recent_messages);
}
