#include "server.h"


void server_broadcast_message(char *buffer, QMessage *q_message, ServerContext *context, bool send_to_author) {
    for (size_t i = 0; i < context->connections->size; ++i) {
        Connection *client_connection = context->connections->storage[i].value;
        if (client_connection == NULL) continue;
        if (client_connection == q_message->connection && !send_to_author) continue;

        send_connection(client_connection, buffer, strlen(buffer));
    }
}

void server_handle_start_listening(QMessage *q_message, ServerContext *context) {
    char buffer[MESSAGE_SIZE] = {0};

    sprintf(buffer, "Started listening\n");
    add_recent_messages(context->recent_messages, buffer);
    printf("%s", buffer);
}

void server_handle_stop_listening(QMessage *qMessage, ServerContext *context) {
    printf("Stopped listening\n");
}

void server_handle_open_connection(QMessage *q_message, ServerContext *context) {
    char buffer[MESSAGE_SIZE] = {0};

    set_table(
            context->connections,
            &q_message->connection->fd,
            sizeof(q_message->connection->fd),
            q_message->connection);
    format_message(
            buffer,
            q_message->payload,
            q_message->connection,
            MESSAGE_CONNECTED);
    add_recent_messages(
            context->recent_messages,
            buffer);
    server_broadcast_message(
            buffer,
            q_message,
            context,
            false);
    printf("%s", buffer);
}


void server_handle_close_connection(QMessage *q_message, ServerContext *context) {
    char buffer[MESSAGE_SIZE] = {0};

    sprintf(buffer, "Closed connection with %lx\n", q_message->connection->name);
    clear_table(
            context->connections,
            &q_message->connection->fd,
            sizeof(q_message->connection->fd));
    format_message(
            buffer,
            q_message->payload,
            q_message->connection,
            MESSAGE_DISCONNECTED);
    add_recent_messages(
            context->recent_messages,
            buffer);
    server_broadcast_message(
            buffer,
            q_message,
            context,
            false);
    empty_connection(q_message->connection);
    free(q_message->connection);
    printf("%s", buffer);
}

void server_handle_received_message(QMessage *q_message, ServerContext *context) {
    char buffer[MESSAGE_SIZE] = {0};
    format_message(
            buffer,
            q_message->payload,
            q_message->connection,
            MESSAGE_SENT);
    add_recent_messages(
            context->recent_messages,
            q_message->payload);
    server_broadcast_message(
            buffer,
            q_message,
            context,
            false);
    printf("QMessage received (%lu) from %lx\n",
           strlen(q_message->payload),
           q_message->connection->name);
}

void server_handle_queue(QMessage *q_message, ServerContext *context) {
    switch (q_message->type) {
        case Q_MESSAGE_NOT_SPECIFIED:
            break;
        case Q_MESSAGE_START_LISTENING:
            server_handle_start_listening(q_message, context);
            break;
        case Q_MESSAGE_OPEN_CONNECTION:
            server_handle_open_connection(q_message, context);
            break;
        case Q_MESSAGE_CLOSE_CONNECTION:
            server_handle_close_connection(q_message, context);
            break;
        case Q_MESSAGE_RECEIVED:
            server_handle_received_message(q_message, context);
            break;
        case Q_MESSAGE_STRIKE:
        case Q_MESSAGE_BAN:
        case Q_MESSAGE_STOP_LISTENING:
            server_handle_stop_listening(q_message, context);
            break;
    }
}

void server_serve() {
    ServerContext *context = initialize_server_context();
    if (context == NULL) {
        printf("Cannot allocate context\n");
        return;
    }
    Queue *queue = init_queue(QUEUE_MODE_READ);
    if (queue == NULL) {
        printf("Cannot open mqueue\n");
        return;
    }
    QMessage q_message = {0};

    pthread_t thread_id;
    ListenerArgs *listener_args = malloc(sizeof(ListenerArgs));
    if (listener_args == NULL) {
        printf("Cannot allocate listener\n");
        return;
    }
    listener_args->context = context;
    pthread_create(&thread_id, NULL, listen_connections, (void *) listener_args);

    while (read_queue(queue, &q_message)) {
        server_handle_queue(&q_message, context);
    }
    printf("Main Loop left\n");
    free_server_context(context);
    free_queue(queue);
}