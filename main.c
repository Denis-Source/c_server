#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include "connection/connection.h"
#include "connection_handler/connection_handler.h"
#include "hash_table/table.h"
#include "queue/queue.h"
#include "listener/listener.h"
#include "error_codes.h"

#define PORT 6969
#define MAX_CONNECTIONS 256
#define QUEUE_NAME "/my_queue"

int main() {
    pthread_t thread_id;
    Message message = {0};
    Connection *server_connection = malloc(sizeof(Connection));

    Queue *queue = create_queue(QUEUE_NAME);
    if (queue == NULL) return ERROR_QUEUE_NOT_CREATED;

    KVTable *connection_table = table_init(MAX_CONNECTIONS);
    if (connection_table == NULL) return ERROR_ALLOCATION;

    if (bind_connection(PORT, server_connection) != true) return ERROR_PORT_BINDING;

    ListenerArgs *listener_args = malloc(sizeof(ListenerArgs));
    if (listener_args == NULL) return ERROR_ALLOCATION;
    listener_args->server_connection = server_connection;
    listener_args->queue = queue;

    pthread_create(&thread_id, NULL, listen_connections, (void *) listener_args);

    while (read_queue(queue, &message)) {
        switch (message.type) {
            case MESSAGE_NOT_SPECIFIED:
                break;
            case MESSAGE_START_LISTENING:
                printf("Started Listening\n");
                break;
            case MESSAGE_OPEN_CONNECTION:
                printf("Opened Connection\n");
                table_set(connection_table, &message.connection->fd, sizeof(message.connection->fd),
                          message.connection);
                break;
            case MESSAGE_CLOSE_CONNECTION:
                printf("Closed Connection\n");
                table_clear(connection_table, &message.connection->fd, sizeof(message.connection->fd));
                free(message.connection);
                break;
            case MESSAGE_RECEIVED:
                printf("Message received: %lu from %u\n", strlen(message.payload), message.connection->fd);
                broadcast_message(connection_table, message.payload, message.connection);
                break;
            case MESSAGE_STRIKE:
            case MESSAGE_BAN:
            case MESSAGE_STOP_LISTENING:
                break;
        }
    }

    return 0;
}
