#include "listener.h"

void *listen_connections(void *args) {
    ListenerArgs *t_args = (ListenerArgs*) args;
    Connection *server_connection = t_args->server_connection;
    Queue *queue = t_args->queue;
    Message message;

    populate_message(&message, MESSAGE_START_LISTENING, NULL, NULL);
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

    populate_message(&message, MESSAGE_STOP_LISTENING, NULL, NULL);
    send_queue(queue, &message);
    free(args);
    return NULL;
}
