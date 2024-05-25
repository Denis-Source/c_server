#include "connection_handler.h"
#include "stdio.h"
#include "../queue/queue.h"
#include "string.h"

#define BUFFER_SIZE 1024

void *handle_connection(void *arg) {
    ThreadArgs *t_args = (ThreadArgs *) arg;

    printf("Connected to %d\n", t_args->conn->fd);
    char buffer[BUFFER_SIZE] = {0};
    Message message;

    while (read_connection(t_args->conn, buffer, BUFFER_SIZE)) {
        memcpy(message.buff, buffer, sizeof(message.buff));
        message.type = BROADCAST;

        send_queue(t_args->queue, &message);
        printf("%d, %s\n", t_args->conn->fd, buffer);
    }


    close_connection(t_args->conn);
    printf("Disconnected\n");
    return NULL;
}
