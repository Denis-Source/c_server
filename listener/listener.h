#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H

#include <pthread.h>
#include <arpa/inet.h>

#include "../hash_table/table.h"
#include "../connection/connection.h"
#include "../queue/queue.h"
#include "../connection_handler/connection_handler.h"


#define MAX_CONNECTIONS 256

typedef struct {
    Connection *server_connection;
    Queue *queue;
} ListenerArgs;

void *listen_connections(void *args);

#endif //SERVER_LISTENER_H
