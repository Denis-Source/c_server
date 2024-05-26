#ifndef SERVER_CONNECTION_HANDLER_H
#define SERVER_CONNECTION_HANDLER_H

#include "string.h"
#include "../connection/connection.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../misc/misc.h"

#define BUFFER_SIZE 1024

typedef struct {
    Connection *client_connection;
    Queue *queue;
} HandlerArgs;

void broadcast_message(KVTable *connection_table, char* payload, Connection *author);

void *handle_connection(void *arg);

#endif //SERVER_CONNECTION_HANDLER_H
