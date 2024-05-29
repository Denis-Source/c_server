#ifndef SERVER_CONTEXT_H
#define SERVER_CONTEXT_H


#include "../definitions.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../circular_buffer/recent_messages.h"


/**
 * Structure representing the server context.
 *
 * This structure represents the context of the server, containing information about the connections
 * and recent messages handled by the server.
 *
 * The structure fields are defined as follows:
 *  - connections: A pointer to the KVTable structure representing the key-value table of connections.
 *  - recent_messages: A pointer to the RecentMessages structure representing the buffer of recent messages.
 *
 * Example usage:
 * @code
 * ServerContext server_ctx;
 * server_ctx.connections = init_table(CONNECTIONS_TABLE_SIZE);
 * server_ctx.recent_messages = init_recent_messages(RECENT_MESSAGES_SIZE);
 * @endcode
 */
typedef struct {
    KVTable *connections;
    RecentMessages *recent_messages;
} ServerContext;


/**
 * Initializes the server context.
 *
 * This function creates and initializes the server context, including the key-value table of connections
 * and the buffer of recent messages. It also creates a message queue for communication.
 *
 * @return A pointer to the initialized ServerContext structure if successful, otherwise NULL.
 *
 * The function performs the following steps:
 * 1. Attempts to create a message queue for communication. If unsuccessful, prints an error message and returns NULL.
 * 2. Initializes a key-value table for connections with a specified maximum capacity. If allocation fails, prints an error message and returns NULL.
 * 3. Initializes a buffer for recent messages with a specified size. If allocation fails, prints an error message and returns NULL.
 * 4. Allocates memory for the ServerContext structure. If allocation fails, prints an error message and returns NULL.
 * 5. Populates the ServerContext structure with the initialized connections table and recent messages buffer.
 * 6. Returns a pointer to the initialized ServerContext structure.
 *
 * Example usage:
 * @code
 * ServerContext *context = initialize_server_context();
 * if (context != NULL) {
 *     // Server context initialized successfully, proceed with server logic
 * } else {
 *     // Handle initialization failure
 * }
 * @endcode
 */
ServerContext *initialize_server_context();


/**
 * Frees the memory allocated for the server context.
 *
 * This function deallocates the memory associated with the server context, including
 * the key-value table of connections and the buffer of recent messages.
 *
 * @param context A pointer to the ServerContext structure to be freed.
 *
 * The function performs the following steps:
 * 1. Frees the memory allocated for the buffer of recent messages using the free_recent_messages function.
 * 2. Frees the memory allocated for the key-value table of connections using the free_table function.
 * 3. Frees the memory allocated for the ServerContext structure itself.
 *
 * Example usage:
 * @code
 * ServerContext *context = initialize_server_context();
 * // Use the server context...
 * free_server_context(context);
 * @endcode
 */
void free_server_context(ServerContext *context);


#endif //SERVER_CONTEXT_H
