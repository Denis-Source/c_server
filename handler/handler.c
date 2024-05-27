#include "handler.h"


void broadcast_message(KVTable *connection_table, char *payload, Connection *author) {
    if (author != NULL) format_message(payload, author, MESSAGE_SENT);
    for (int i = 0; i < connection_table->size; ++i) {
        Connection *client_connection = connection_table->storage[i].value;
        if (client_connection == NULL) continue;
        if (client_connection == author) continue;

        send_connection(client_connection, payload, strlen(payload));
    }
}

void *handle_connection(void *arg) {
    HandlerArgs *t_args = (HandlerArgs *) arg;

    Connection *client_connection = t_args->client_connection;
    Queue *queue = t_args->queue;
    char buffer[MESSAGE_BUFFER_SIZE + MESSAGE_FORMATTING_SIZE] = {0};
    QMessage message;

    populate_message(&message, Q_MESSAGE_OPEN_CONNECTION, client_connection, NULL);
    send_queue(queue, &message);
    while (read_connection(client_connection, buffer, MESSAGE_BUFFER_SIZE - 1)) {
        sanitize_buffer(buffer, MESSAGE_BUFFER_SIZE);
        memcpy(message.payload, buffer, sizeof(message.payload));
        populate_message(&message, Q_MESSAGE_RECEIVED, client_connection, buffer);
        send_queue(queue, &message);
        memset(buffer, '\0', MESSAGE_BUFFER_SIZE);
    }
    close_connection(client_connection);
    populate_message(&message, Q_MESSAGE_CLOSE_CONNECTION, client_connection, NULL);
    send_queue(queue, &message);
    free(arg);
    return NULL;
}
