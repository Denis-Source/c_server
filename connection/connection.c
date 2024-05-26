#include <stdio.h>
#include "connection.h"
#include "../hash_table/hash.h"

#define CONNECTIONS_LIMIT 256
#define CONNECTION_MASK 0x4C5A3F2C1B8D

void populate_connection(Connection *conn, int32_t fd, u_int32_t address, u_int16_t port) {
    conn->fd = fd;
    conn->address = address;
    conn->port = port;
    conn->name = (((u_int64_t) address << 16) + port) ^ CONNECTION_MASK;
}

void empty_connection(Connection *conn) {
    conn->fd = 0;
    conn->address = 0;
    conn->port = 0;
    conn->name = 0;
}

bool bind_connection(u_int16_t port, Connection *conn) {
    int32_t fd;
    int32_t opt = 1;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == 0) return false;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) return false;

    struct sockaddr_in address = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(port)
    };
    if (bind(fd, (struct sockaddr *) &address, sizeof(address)) != 0) return false;

    populate_connection(conn, fd, ntohl(address.sin_addr.s_addr), port);
    return true;
}

bool accept_connection(Connection *server_connection, Connection *client_connection) {
    struct sockaddr_in client_socket_address;
    int32_t client_fd;
    uint32_t client_socket_address_size = sizeof(client_socket_address);

    client_fd = accept(server_connection->fd, (struct sockaddr *) &client_socket_address, &client_socket_address_size);
    if (client_fd < 0) return false;
    populate_connection(
            client_connection,
            client_fd,
            ntohl(client_socket_address.sin_addr.s_addr),
            ntohs(client_socket_address.sin_port)
    );

    return true;
}

bool listen_on_connection(Connection *connection) {
    return listen(connection->fd, CONNECTIONS_LIMIT) == 0;
}

bool read_connection(Connection *conn, void *buffer, size_t buffer_size) {
    if (recv(conn->fd, buffer, buffer_size, 0) <= 0) return false;
    return true;
}

bool send_connection(Connection *conn, void *buffer, size_t buffer_size) {
    if (send(conn->fd, buffer, buffer_size, 0) == -1) return false;
    return true;
}

void close_connection(Connection *conn) {
    close(conn->fd);
    empty_connection(conn);
}