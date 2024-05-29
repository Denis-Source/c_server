#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <stdio.h>
#include "../definitions.h"
#include "../misc/formatting.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../handler/handler.h"
#include "../listener/listener.h"
#include "../circular_buffer/recent_messages.h"
#include "context.h"


/**
 * Starts serving requests on the server.
 *
 * This function initializes the server context, opens a message queue for reading,
 * and starts a listener thread to accept incoming connections. It then enters a loop
 * to continuously read messages from the message queue and handle them using the
 * server_handle_queue function. Finally, it cleans up resources and exits.
 *
 * The function performs the following steps:
 * 1. Initializes the server context using the initialize_server_context function.
 *    If initialization fails, prints an error message and returns.
 * 2. Opens a message queue for reading using the open_queue function.
 *    If opening the queue fails, prints an error message and returns.
 * 3. Creates a listener thread to accept incoming connections using the listen_connections function.
 *    If thread creation fails or memory allocation fails, prints an error message and returns.
 * 4. Enters a loop to continuously read messages from the message queue using the read_queue function.
 *    For each message read, it is handled using the server_handle_queue function.
 * 5. Prints a message indicating that the main loop has exited.
 * 6. Frees the memory associated with the server context using the free_server_context function.
 * 7. Closes the message queue.
 *
 * Example usage:
 * @code
 * server_serve();
 * @endcode
 */
void server_serve();


#endif //SERVER_SERVER_H
