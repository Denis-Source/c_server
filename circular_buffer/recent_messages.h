#ifndef SERVER_RECENT_MESSAGES_H
#define SERVER_RECENT_MESSAGES_H


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../definitions.h"


#define INVALID_ADDRESS -1

typedef struct {
    u_int16_t size;
    int32_t head;
    int32_t tail;
    char **storage;
} RecentMessages;

RecentMessages *init_recent_messages(size_t size);

bool add_recent_messages(RecentMessages *recent_messages, char *data);

bool get_head_recent_messages(RecentMessages *recent_messages, char *result, size_t index);

bool get_tail_recent_messages(RecentMessages *recent_messages, char *result, size_t index);

void free_recent_messages(RecentMessages *recent_messages);


#endif //SERVER_RECENT_MESSAGES_H
