#include "connection_handler.h"
#include "stdio.h"

void broadcast_message(KVTable *connection_table, char* payload, Connection *author) {
    char *formatted_message = format_message(payload, author->name);
    for (int i = 0; i < connection_table->size; ++i) {
        Connection *client_connection = connection_table->_storage[i].value;
        if (client_connection == NULL) continue;
        if (client_connection == author) continue;

        send_connection(client_connection, formatted_message, strlen(formatted_message));
    }
    free(formatted_message);
}

void *handle_connection(void *arg) {
    HandlerArgs *t_args = (HandlerArgs *) arg;

    Connection *client_connection = t_args->client_connection;
    Queue *queue = t_args->queue;
    bool valid;
    char buffer[BUFFER_SIZE] = {0};
    Message message;

    populate_message(&message, MESSAGE_OPEN_CONNECTION, client_connection, NULL);
    send_queue(queue, &message);
    while (read_connection(client_connection, buffer, BUFFER_SIZE)) {
        valid = sanitize_buffer(buffer, BUFFER_SIZE);
        if (!valid) {
            populate_message(&message, MESSAGE_STRIKE, client_connection, NULL);
            send_queue(queue, &message);
        }
        memcpy(message.payload, buffer, sizeof(message.payload));
        populate_message(&message, MESSAGE_RECEIVED, client_connection, buffer);
        send_queue(queue, &message);
        memset(buffer, '\0', BUFFER_SIZE);
    }
    close_connection(client_connection);
    populate_message(&message, MESSAGE_CLOSE_CONNECTION, client_connection, NULL);
    send_queue(queue, &message);
    free(arg);
    return NULL;
}
