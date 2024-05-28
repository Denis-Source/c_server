#include "context.h"


ServerContext *initialize_server_context() {
    Queue *queue = create_queue(QUEUE_NAME);
    if (queue == NULL) {
        printf("Cannot allocate mqueue\n");
        return NULL;
    }

    KVTable *connections = init_table(SOCKET_MAX_CONNECTIONS);
    if (connections == NULL) {
        printf("Cannot allocate table connections\n");
        return NULL;
    }
    MessageBuffer *recent_messages = init_message_buffer(100);
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
    free_messages_buffer(context->recent_messages);
    free_queue(context->queue);
    free_messages_buffer(context->recent_messages);
    free(context);
}