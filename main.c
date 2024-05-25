#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include "connection/connection.h"
#include "connection_handler/connection_handler.h"
#include "hash_table/table.h"
#include "queue/queue.h"

#define PORT 8000
#define MAX_CONNECTIONS 256

int main() {
    pthread_t thread_id;
    Connection server_conn = {0};
    Queue queue;


    KVTable table = *table_init(MAX_CONNECTIONS);

    if (bind_connection(PORT, &server_conn) != true) return 1;
    if (create_queue("/my_queue", &queue) != true) return 1;

    listen_on_connection(&server_conn);
    Connection *client_conn = malloc(sizeof(Connection));
    table_set(&table, &client_conn->fd, sizeof(client_conn->fd), client_conn);
    ThreadArgs *args = malloc(sizeof(ThreadArgs));
    args->conn = client_conn;
    args->queue = &queue;
    if (!accept_connection(&server_conn, client_conn)) return 1;

    pthread_create(&thread_id, NULL, handle_connection, (void *) args);

    while (1) {
        Message message = {0};
        if (read_queue(&queue, &message) != true) break;
        printf("%s", message.buff);
    }

    return 0;
}
