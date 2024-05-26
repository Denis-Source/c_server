#include "server.h"


#define PORT 6969
#define MAX_CONNECTIONS 256
#define QUEUE_NAME "/my_queue"

void server_handle_queue(QMessage *q_message, KVTable *connections) {
    switch (q_message->type) {
        case Q_MESSAGE_NOT_SPECIFIED:
            break;
        case Q_MESSAGE_START_LISTENING:
            printf("Started Listening\n");
            break;
        case Q_MESSAGE_OPEN_CONNECTION:
            printf("Opened Connection\n");
            table_set(connections, &q_message->connection->fd, sizeof(q_message->connection->fd),
                      q_message->connection);
            format_message(q_message->payload, q_message->connection, MESSAGE_CONNECTED);
            broadcast_message(connections, q_message->payload, NULL);
            break;
        case Q_MESSAGE_CLOSE_CONNECTION:
            printf("Closed Connection\n");
            table_clear(connections, &q_message->connection->fd, sizeof(q_message->connection->fd));
            format_message(q_message->payload, q_message->connection, MESSAGE_DISCONNECTED);
            broadcast_message(connections, q_message->payload, NULL);
            empty_connection(q_message->connection);
            free(q_message->connection);
            break;
        case Q_MESSAGE_RECEIVED:
            printf("QMessage received: %lu from %lu\n", strlen(q_message->payload), q_message->connection->name);
            broadcast_message(connections, q_message->payload, q_message->connection);
            break;
        case Q_MESSAGE_STRIKE:
        case Q_MESSAGE_BAN:
        case Q_MESSAGE_STOP_LISTENING:
            printf("Stopped Listening\n");
            break;
    }
}

void server_serve() {
    pthread_t thread_id;
    QMessage q_message = {0};
    Connection *server_connection = malloc(sizeof(Connection));

    Queue *queue = create_queue(QUEUE_NAME);
    if (queue == NULL) {
        printf("Cannot allocate mqueue");
        return;
    }

    KVTable *connections = table_init(MAX_CONNECTIONS);
    if (connections == NULL) {
        printf("Cannot allocate table connections");
        return;
    }

    if (bind_connection(PORT, server_connection) != true) return;

    ListenerArgs *listener_args = malloc(sizeof(ListenerArgs));
    if (listener_args == NULL) {
        printf("Cannot allocate listener");
        return;
    }
    listener_args->server_connection = server_connection;
    listener_args->queue = queue;

    pthread_create(&thread_id, NULL, listen_connections, (void *) listener_args);

    while (read_queue(queue, &q_message)) {
        server_handle_queue(&q_message, connections);
    }

    table_free(connections);
}