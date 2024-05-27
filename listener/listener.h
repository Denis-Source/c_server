#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H

#include <pthread.h>
#include <arpa/inet.h>

#include "../hash_table/table.h"
#include "../connection/connection.h"
#include "../queue/queue.h"
#include "../handler/handler.h"


#define MAX_CONNECTIONS 256
#define PORT 6969

typedef struct {
    Queue *queue;
} ListenerArgs;

void *listen_connections(void *args);

#endif //SERVER_LISTENER_H
