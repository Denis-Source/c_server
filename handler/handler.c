#include "handler.h"


void send_recent_messages(Connection *connection, RecentMessages *recent_messages) {
    char message[MESSAGE_SIZE] = {0};
    u_int32_t index = 0;
    while (get_tail_recent_messages(recent_messages, message, index)) {
        send_connection(connection, message, strlen(message));
        ++index;
    }
}


void *handle_connection(void *arg) {
    HandlerArgs *t_args = (HandlerArgs *) arg;

    Connection *client_connection = t_args->client_connection;
    ServerContext *context = t_args->context;
    char buffer[MESSAGE_SIZE] = {0};
    QMessage message;

    populate_message(&message, Q_MESSAGE_OPEN_CONNECTION, client_connection, NULL);
    send_queue(context->queue, &message);
    send_recent_messages(client_connection, context->recent_messages);
    while (read_connection(client_connection, buffer, MESSAGE_BUFFER_SIZE - 1)) {
        sanitize_buffer(buffer, MESSAGE_BUFFER_SIZE);
        memcpy(message.payload, buffer, sizeof(message.payload));
        populate_message(&message, Q_MESSAGE_RECEIVED, client_connection, buffer);
        send_queue(context->queue, &message);
        memset(buffer, '\0', MESSAGE_BUFFER_SIZE);
    }
    close_connection(client_connection);
    populate_message(&message, Q_MESSAGE_CLOSE_CONNECTION, client_connection, NULL);
    send_queue(context->queue, &message);
    free(arg);
    return NULL;
}
