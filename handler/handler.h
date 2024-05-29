#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H


#include <string.h>
#include "../connection/connection.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../misc/formatting.h"
#include "../server/context.h"
#include "../queue/queue.h"


/**
 * Structure representing arguments for a connection handler thread.
 *
 * This structure represents the arguments passed to a connection handler thread.
 * It contains pointers to the client connection and the server context associated with the handler.
 *
 * The structure fields are defined as follows:
 *  - client_connection: A pointer to the client connection structure associated with the handler.
 *  - context: A pointer to the server context structure containing context information for the handler.
 *
 * Example usage:
 * @code
 * HandlerArgs args;
 * args.client_connection = client_conn;
 * args.context = server_ctx;
 * pthread_create(&thread_id, NULL, handle_connection, (void*)&args);
 * @endcode
 */
typedef struct {
    Connection *client_connection;
    ServerContext *context;
} HandlerArgs;


/**
 * Sends recent messages stored in a RecentMessages buffer to the specified connection.
 *
 * This function sends recent messages stored in a RecentMessages buffer to the specified connection.
 * It iterates through the messages from the tail of the buffer and sends each message to the connection.
 *
 * @param connection A pointer to the Connection structure representing the destination connection.
 * @param recent_messages A pointer to the RecentMessages buffer containing recent messages to be sent.
 *
 * The function performs the following steps:
 * 1. Initializes a message buffer to store each message.
 * 2. Iterates through the recent messages in the buffer starting from the tail.
 * 3. Retrieves each message from the buffer and sends it to the specified connection.
 *
 * Example usage:
 * @code
 * Connection conn;
 * RecentMessages *messages;
 * // Populate conn with connection details
 * // Populate messages with recent messages
 * send_recent_messages(&conn, messages);
 * @endcode
 */
void send_recent_messages(Connection *connection, RecentMessages *recent_messages);


/**
 * Thread function for handling communication with a client connection.
 *
 * This function is invoked as a separate thread to handle communication with a client connection.
 * It reads incoming messages from the client, processes them, and sends responses accordingly.
 * Communication with the main server thread is facilitated through a message queue.
 *
 * @param arg A pointer to a structure containing arguments for the thread function.
 *            It must be of type HandlerArgs, containing a client connection and server context.
 *
 * @return NULL after handling the connection.
 *
 * The function performs the following steps:
 * 1. Initializes a message queue for communication with the main server thread.
 * 2. Sends an open connection message to the main server thread via the message queue.
 * 3. Sends recent messages to the client.
 * 4. Enters a loop to read incoming messages from the client, sanitize them, and send them to the main server thread
 *    via the message queue.
 * 5. Closes the client connection when communication ends and sends a close connection message to the main server
 *    thread via the message queue.
 * 6. Frees memory allocated for argument structure and message queue.
 *
 * Example usage:
 * @code
 * pthread_t thread;
 * HandlerArgs *args = (HandlerArgs*)malloc(sizeof(HandlerArgs));
 * args->client_connection = &client_conn;
 * args->context = &server_context;
 * pthread_create(&thread, NULL, handle_connection, (void*)args);
 * @endcode
 */
void *handle_connection(void *arg);


#endif //SERVER_HANDLER_H
