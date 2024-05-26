#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H

#define MAX_CONNECTIONS 256

#include <arpa/inet.h>
#include <pthread.h>
#include "../hash_table/table.h"
#include "../connection/connection.h"
#include "../queue/queue.h"
#include "../connection_handler/connection_handler.h"

typedef struct {
    Connection *server_connection;
    Queue *queue;
} ListenerArgs;

void *listen_connections(void *args);

#endif //SERVER_LISTENER_H
