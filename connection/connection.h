#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include "stdlib.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>


typedef struct {
    int32_t fd;
    u_int32_t address;
    u_int16_t port;
} Connection;

void empty_connection(Connection *conn);

bool bind_connection(u_int16_t port, Connection *conn);

bool accept_connection(Connection *server_connection, Connection *client_connection);

bool listen_on_connection(Connection *connection);

bool read_connection(Connection *conn, void *buffer, size_t buffer_size);

bool send_connection(Connection *conn, void *buffer, size_t buffer_size);

void close_connection(Connection *conn);

#endif //SERVER_CONNECTION_H
