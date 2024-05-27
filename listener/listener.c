#include "listener.h"


void *listen_connections(void *args) {
    ListenerArgs *t_args = (ListenerArgs*) args;
    Connection *server_connection = malloc(sizeof(Connection));
    Queue *queue = t_args->queue;
    QMessage message;


    if (bind_connection(PORT, server_connection) != true) NULL;
    populate_message(&message, Q_MESSAGE_START_LISTENING, NULL, NULL);
    send_queue(queue, &message);

    while (listen_on_connection(server_connection)) {
        pthread_t thread_id;
        Connection *client_connection = malloc(sizeof(Connection));
        if (!accept_connection(server_connection, client_connection)) break;

        HandlerArgs *handler_args = malloc(sizeof(HandlerArgs));
        if(handler_args == NULL) break;
        handler_args->client_connection = client_connection;
        handler_args->queue = queue;

        pthread_create(&thread_id, NULL, handle_connection, (void *) handler_args);
    }

    populate_message(&message, Q_MESSAGE_STOP_LISTENING, NULL, NULL);
    send_queue(queue, &message);
    free(args);
    free(server_connection);
    return NULL;
}
