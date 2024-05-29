#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H


#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../hash_table/hash.h"
#include "../definitions.h"
#include "../misc/secrets.h"


/**
 * Structure representing a network connection.
 *
 * This structure represents a network connection, containing information about the socket file descriptor,
 * IP address, port number, and a unique name assigned to the connection.
 *
 * The structure fields are defined as follows:
 *  - fd: The socket file descriptor associated with the connection.
 *  - address: The IP address of the connection.
 *  - port: The port number of the connection.
 *  - name: A unique name assigned to the connection, typically derived from the combination of IP address and port.
 *
 * Example usage:
 * @code
 * Connection conn;
 * conn.fd = socket(AF_INET, SOCK_STREAM, 0);
 * conn.address = INADDR_ANY;
 * conn.port = 8080;
 * conn.name = hash((u_int8_t*)&conn.address, sizeof(conn.address)) ^ hash((u_int8_t*)&conn.port, sizeof(conn.port));
 * @endcode
 */
typedef struct {
    int32_t fd;
    u_int32_t address;
    u_int16_t port;
    u_int64_t name;
} Connection;


/**
 * Populates a Connection structure with the provided file descriptor, address, and port.
 *
 * This function initializes a Connection structure with the specified file descriptor (fd),
 * address, and port. Additionally, it generates a unique name for the connection based on
 * the provided address and port, combined with a random value.
 *
 * @param conn A pointer to the Connection structure to be populated.
 * @param fd The file descriptor associated with the connection.
 * @param address The IP address of the connection.
 * @param port The port number of the connection.

 * Example usage:
 * @code
 * Connection conn;
 * populate_connection(&conn, fd, address, port);
 * @endcode
 */
void populate_connection(Connection *conn, int32_t fd, u_int32_t address, u_int16_t port);


/**
 * Empties a Connection structure by resetting its fields to zero.
 *
 * This function resets all fields of the provided Connection structure to zero,
 * effectively clearing any existing data associated with the connection.
 *
 * @param conn A pointer to the Connection structure to be emptied.

 * Example usage:
 * @code
 * Connection conn;
 * empty_connection(&conn);
 * @endcode
 */
void empty_connection(Connection *conn);


/**
 * Binds a socket to the specified port and populates a Connection structure with the socket details.
 *
 * This function creates a socket and binds it to the specified port. It also sets the socket
 * options to allow reusing the address and port if available. After successful binding, it populates
 * the provided Connection structure with the socket file descriptor (fd), address, and port details.
 *
 * @param port The port number to bind the socket to.
 * @param conn A pointer to the Connection structure to be populated with socket details.
 *
 * @return true if the binding is successful and the Connection structure is populated, otherwise false.
 *
 * The function performs the following steps:
 * 1. Creates a new socket with the AF_INET address family and SOCK_STREAM type.
 * 2. Sets socket options to allow reusing the address and port if available.
 * 3. Defines and initializes a sockaddr_in structure with the provided port and INADDR_ANY address.
 * 4. Binds the socket to the specified port and INADDR_ANY address.
 * 5. If the binding is successful, populates the provided Connection structure with socket details.
 *    If unsuccessful at any step, returns false.
 *
 * Example usage:
 * @code
 * Connection conn;
 * if (bind_connection(8080, &conn)) {
 *     // Connection structure populated successfully.
 * } else {
 *     // Error occurred during socket binding.
 * }
 * @endcode
 */
bool bind_connection(u_int16_t port, Connection *conn);


/**
 * Accepts an incoming connection on a server socket and populates a Connection structure with client details.
 *
 * This function accepts an incoming connection on the provided server socket and populates
 * the provided client Connection structure with details of the accepted connection, including
 * the client's IP address, port, and socket file descriptor.
 *
 * @param server_connection A pointer to the Connection structure representing the server socket.
 * @param client_connection A pointer to the Connection structure to be populated with client details.
 *
 * @return true if the connection is successfully accepted and the client Connection structure is populated, otherwise false.
 *
 * The function performs the following steps:
 * 1. Calls the accept() system call to accept an incoming connection on the server socket.
 * 2. If the accept() call fails (returns a negative value), returns false.
 * 3. Populates the client Connection structure with the accepted client's socket file descriptor,
 *    IP address, and port details obtained from the accepted socket address.
 * 4. Returns true upon successful acceptance and population of the client Connection structure.
 *
 * Example usage:
 * @code
 * Connection server_conn;
 * Connection client_conn;
 * // Populate server_conn with server socket details
 * if (accept_connection(&server_conn, &client_conn)) {
 *     // Client connection accepted successfully.
 * } else {
 *     // Error occurred while accepting client connection.
 * }
 * @endcode
 */
bool accept_connection(Connection *server_connection, Connection *client_connection);


/**
 * Starts listening for incoming connections on the provided socket.
 *
 * This function starts listening for incoming connections on the provided socket.
 * It sets the socket to the listening state with a maximum backlog of pending connections.
 *
 * @param connection A pointer to the Connection structure representing the socket to listen on.
 *
 * @return true if the socket is successfully set to the listening state, otherwise false.
 *
 * The function performs the following steps:
 * 1. Calls the listen() system call to start listening for incoming connections on the socket.
 * 2. Sets the maximum backlog of pending connections to SOCKET_MAX_CONNECTIONS.
 * 3. Returns true if the listen() call succeeds (returns 0), indicating that the socket is
 *    now in the listening state. Returns false otherwise.
 *
 * Example usage:
 * @code
 * Connection conn;
 * // Populate conn with socket details
 * if (listen_on_connection(&conn)) {
 *     // Socket is now listening for incoming connections.
 * } else {
 *     // Error occurred while setting socket to listening state.
 * }
 * @endcode
 */
bool listen_on_connection(Connection *connection);


/**
 * Reads data from the specified connection socket into the provided buffer.
 *
 * This function reads data from the specified connection socket into the provided buffer.
 * It attempts to receive data from the socket and store it in the buffer.
 *
 * @param conn A pointer to the Connection structure representing the connection socket.
 * @param buffer A pointer to the buffer where the received data will be stored.
 * @param buffer_size The size of the buffer in bytes.
 *
 * @return true if data is successfully read from the socket and stored in the buffer, otherwise false.
 *
 * The function performs the following steps:
 * 1. Calls the recv() system call to attempt to receive data from the connection socket.
 * 2. Copies the received data into the provided buffer.
 * 3. Returns true if the recv() call succeeds and data is read from the socket.
 *    Returns false if the recv() call fails (returns a non-positive value), indicating
 *    either an error or that the connection has been closed by the peer.
 *
 * Example usage:
 * @code
 * Connection conn;
 * // Populate conn with connection details
 * char buffer[1024];
 * if (read_connection(&conn, buffer, sizeof(buffer))) {
 *     // Data successfully read from the connection socket.
 * } else {
 *     // Error occurred while reading from the socket or connection closed by the peer.
 * }
 * @endcode
 */
bool read_connection(Connection *conn, void *buffer, size_t buffer_size);


/**
 * Sends data from the provided buffer over the specified connection socket.
 *
 * This function sends data from the provided buffer over the specified connection socket.
 * It attempts to send the data to the connected peer.
 *
 * @param conn A pointer to the Connection structure representing the connection socket.
 * @param buffer A pointer to the buffer containing the data to be sent.
 * @param buffer_size The size of the data to be sent in bytes.
 *
 * @return true if the data is successfully sent over the connection socket, otherwise false.
 *
 * The function performs the following steps:
 * 1. Calls the send() system call to attempt to send data from the buffer over the connection socket.
 * 2. Returns true if the send() call succeeds (returns the number of bytes sent),
 *    indicating that the data is successfully sent over the socket.
 *    Returns false if the send() call fails (returns -1), indicating an error occurred
 *    while attempting to send the data.
 *
 * Example usage:
 * @code
 * Connection conn;
 * // Populate conn with connection details
 * char buffer[] = "Hello, world!";
 * if (send_connection(&conn, buffer, sizeof(buffer))) {
 *     // Data successfully sent over the connection socket.
 * } else {
 *     // Error occurred while sending data over the socket.
 * }
 * @endcode
 */
bool send_connection(Connection *conn, void *buffer, size_t buffer_size);


/**
 * Closes the specified connection socket.
 *
 * This function closes the specified connection socket, releasing associated resources
 * and terminating the connection. After calling this function, the connection socket
 * should no longer be used.
 *
 * @param conn A pointer to the Connection structure representing the connection socket to close.
 *
 * The function performs the following steps:
 * 1. Calls the close() system call to close the file descriptor associated with the connection socket.
 *
 * Example usage:
 * @code
 * Connection conn;
 * // Populate conn with connection details
 * close_connection(&conn);
 * @endcode
 */
void close_connection(Connection *conn);


#endif //SERVER_CONNECTION_H
