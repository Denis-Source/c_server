#include "connection_handler.h"


void broadcast_message(KVTable *connection_table, char* payload) {
//    for (int i = 0; i < connection_table->size; ++i) {
//        Connection *client_connection = connection_table[i]._storage->value;
//        if (client_connection == NULL) continue;
//        send_connection(client_connection, payload, strlen(payload));
//    }
}

void *handle_connection(void *arg) {
    HandlerArgs *t_args = (HandlerArgs *) arg;

    Connection *client_connection = t_args->client_connection;
    Queue *queue = t_args->queue;
    char buffer[BUFFER_SIZE] = {0};
    Message message;

    populate_message(&message, MESSAGE_OPEN_CONNECTION, client_connection, NULL);
    send_queue(queue, &message);
    while (read_connection(client_connection, buffer, BUFFER_SIZE)) {
        memcpy(message.payload, buffer, sizeof(message.payload));
        populate_message(&message, MESSAGE_RECEIVED, client_connection, buffer);
        send_queue(queue, &message);
    }
    close_connection(client_connection);
    populate_message(&message, MESSAGE_CLOSE_CONNECTION, NULL, NULL);
    send_queue(queue, &message);
    free(client_connection);
    return NULL;
}
