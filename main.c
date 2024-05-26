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
    QMessage q_message = {0};
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

    while (read_queue(queue, &q_message)) {
        switch (q_message.type) {
            case Q_MESSAGE_NOT_SPECIFIED:
                break;
            case Q_MESSAGE_START_LISTENING:
                printf("Started Listening\n");
                break;
            case Q_MESSAGE_OPEN_CONNECTION:
                printf("Opened Connection\n");
                table_set(connection_table, &q_message.connection->fd, sizeof(q_message.connection->fd),
                          q_message.connection);
                format_message(q_message.payload, q_message.connection, MESSAGE_CONNECTED);
                broadcast_message(connection_table, q_message.payload, NULL);
                break;
            case Q_MESSAGE_CLOSE_CONNECTION:
                printf("Closed Connection\n");
                table_clear(connection_table, &q_message.connection->fd, sizeof(q_message.connection->fd));
                format_message(q_message.payload, q_message.connection, MESSAGE_DISCONNECTED);
                broadcast_message(connection_table, q_message.payload, NULL);
                empty_connection(q_message.connection);
                free(q_message.connection);
                break;
            case Q_MESSAGE_RECEIVED:
                printf("QMessage received: %lu from %lu\n", strlen(q_message.payload), q_message.connection->name);
                broadcast_message(connection_table, q_message.payload, q_message.connection);
                break;
            case Q_MESSAGE_STRIKE:
            case Q_MESSAGE_BAN:
            case Q_MESSAGE_STOP_LISTENING:
                printf("Stopped Listening\n");
                break;
        }
    }

    return 0;
}
