#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "string.h"

#include "../connection/connection.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../misc/misc.h"


#define MESSAGE_BUFFER_SIZE QUEUE_PAYLOAD_SIZE

typedef struct {
    Connection *client_connection;
    Queue *queue;
} HandlerArgs;

void broadcast_message(KVTable *connection_table, char* payload, Connection *author);

void *handle_connection(void *arg);

#endif //SERVER_HANDLER_H
