#include "context.h"


ServerContext *initialize_server_context() {
    if (!create_queue()) {
        printf("Cannot create mqueue\n");
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

    return context;
}


void free_server_context(ServerContext *context) {
    free_recent_messages(context->recent_messages);
    free_table(context->connections);
    free(context);
}
