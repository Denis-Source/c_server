#include "context.h"


ServerContext *initialize_server_context() {
    if (!create_queues()) {
        printf("Cannot create mqueue");
        return NULL;
    }

    Queue *queue = init_queue(QUEUE_MODE_READWRITE);
    if (queue == NULL) {
        printf("Cannot open mqueue\n");
        return NULL;
    }

    KVTable *connections = init_table(SOCKET_MAX_CONNECTIONS);
    if (connections == NULL) {
        printf("Cannot allocate table connections\n");
        return NULL;
    }
    RecentMessages *recent_messages = init_recent_messages(RECENT_MESSAGES_SIZE);
    if (recent_messages == NULL) {
        printf("Cannot allocate message buffer\n");
        return NULL;
    }

    ServerContext *context = (ServerContext *) malloc(sizeof(ServerContext));
    if (context == NULL) {
        printf("Cannot allocate server context\n");
    }

    context->connections = connections;
    context->recent_messages = recent_messages;
    context->queue = queue;

    return context;
}


void free_server_context(ServerContext *context) {
    free_recent_messages(context->recent_messages);
    free_queue(context->queue);
    free_recent_messages(context->recent_messages);
    free(context);
}