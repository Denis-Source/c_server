#ifndef SERVER_CONTEXT_H
#define SERVER_CONTEXT_H


#include "../definitions.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../circular_buffer/recent_messages.h"


typedef struct {
    KVTable *connections;
    RecentMessages *recent_messages;
    Queue *queue;
} ServerContext;

ServerContext *initialize_server_context();

void free_server_context(ServerContext *context);


#endif //SERVER_CONTEXT_H
