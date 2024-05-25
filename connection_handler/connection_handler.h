#ifndef SERVER_CONNECTION_HANDLER_H
#define SERVER_CONNECTION_HANDLER_H

#include "../connection/connection.h"
#include "../queue/queue.h"

typedef struct {
    Connection *conn;
    Queue *queue;
} ThreadArgs;

void *handle_connection(void *arg) ;

#endif //SERVER_CONNECTION_HANDLER_H
