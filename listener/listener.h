#ifndef SERVER_LISTENER_H
#define SERVER_LISTENER_H


#include <pthread.h>
#include <arpa/inet.h>
#include "../hash_table/table.h"
#include "../connection/connection.h"
#include "../queue/queue.h"
#include "../handler/handler.h"
#include "../server/context.h"
#include "../definitions.h"


/**
 * Structure representing arguments for a listener thread.
 *
 * This structure represents the arguments passed to a listener thread.
 * It contains a pointer to the server context associated with the listener.
 *
 * The structure fields are defined as follows:
 *  - context: A pointer to the server context structure containing context information for the listener.
 *
 * Example usage:
 * @code
 * ListenerArgs args;
 * args.context = server_ctx;
 * pthread_create(&thread_id, NULL, listen_connections, (void*)&args);
 * @endcode
 */
typedef struct {
    ServerContext *context;
} ListenerArgs;


/**
 * Listens for incoming connections and handles them in separate threads.
 *
 * This function listens for incoming connections on the specified port and handles each connection
 * in a separate thread. It communicates with the main server thread using a message queue to send
 * notifications about the start and stop of the listening process.
 *
 * @param args A pointer to a structure containing arguments for the listener function.
 *             It must be of type ListenerArgs, containing the server context.
 *
 * @return NULL after handling connections.
 *
 * The function performs the following steps:
 * 1. Initializes a message queue for communication with the main server thread.
 * 2. Creates a server connection and binds it to the specified port.
 * 3. Sends a start listening message to the main server thread via the message queue.
 * 4. Enters a loop to listen for incoming connections and handle each connection in a separate thread.
 * 5. Sends a stop listening message to the main server thread via the message queue when listening ends.
 * 6. Frees memory allocated for the message queue, arguments structure, and server connection.
 *
 * Example usage:
 * @code
 * ListenerArgs *args = (ListenerArgs*)malloc(sizeof(ListenerArgs));
 * args->context = &server_context;
 * pthread_create(&thread_id, NULL, listen_connections, (void*)args);
 * @endcode
 */
void *listen_connections(void *args);


#endif //SERVER_LISTENER_H
