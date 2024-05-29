#ifndef SERVER_RECENT_MESSAGES_H
#define SERVER_RECENT_MESSAGES_H


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../definitions.h"


// In case of freshly initialized buffer
// Invalid addresses are set to tail and head
#define INVALID_ADDRESS -1


/**
 * Structure representing a buffer for storing recent messages.
 *
 * This structure represents a buffer used for storing recent messages. It maintains information about
 * the size of the buffer, the current head and tail positions within the buffer, and a pointer to the
 * storage array containing the messages.
 *
 * The structure fields are defined as follows:
 *  - size: The size of the buffer, indicating the maximum number of messages it can store.
 *  - head: The index of the newest message in the buffer (current head position).
 *  - tail: The index of the oldest message in the buffer (current tail position).
 *  - storage: A pointer to an array of pointers to char arrays (messages), representing the storage buffer.
 *
 * Example usage:
 * @code
 * RecentMessages messages;
 * messages.size = 100;
 * messages.head = -1;
 * messages.tail = -1;
 * messages.storage = (char **)malloc(messages.size * sizeof(char *));
 * @endcode
 */
typedef struct {
    u_int16_t size;
    int32_t head;
    int32_t tail;
    char **storage;
} RecentMessages;


/**
 * @brief Initializes a RecentMessages buffer.
 *
 * Allocates memory for a RecentMessages buffer and initializes its storage array
 * with the specified size. Each element in the storage array is allocated memory for holding
 * a message of size MESSAGE_SIZE. The function also sets the head and tail to INVALID_ADDRESS.
 *
 * @param size The number of messages the buffer can hold.
 *
 * @return A pointer to the initialized RecentMessages buffer, or NULL if the allocation fails.
 *
 * The function performs the following steps:
 * 1. Allocates memory for the RecentMessages structure.
 * 2. Allocates memory for the storage array within the structure.
 * 3. Allocates memory for each message slot in the storage array.
 * 4. Sets the head to INVALID_ADDRESS.
 *
 * If any memory allocation fails, the function frees all previously allocated memory and returns NULL.
 *
 * Example usage:
 * @code
 * RecentMessages *messages = init_recent_messages(10);
 * if (messages == NULL) {
 *     // Handle allocation failure
 * }
 * @endcode
 */
RecentMessages *init_recent_messages(size_t size);


/**
 * Adds a new message to the RecentMessages buffer.
 *
 * Inserts a new message into the RecentMessages buffer at the current head position.
 * If the buffer is full, the oldest message is overwritten. The function updates the
 * head position and, if necessary, the tail position.
 *
 * @param recent_messages A pointer to the RecentMessages buffer.
 * @param data The message to be added to the buffer. The message must be no longer
 *             than MESSAGE_SIZE.
 *
 * @return A boolean value indicating whether an existing message was replaced:
 *         - true: an existing message was replaced.
 *         - false: no existing message was replaced.
 *
 * The function performs the following steps:
 * 1. If the buffer is empty (head is INVALID_ADDRESS), initialize the head and tail to 0.
 * 2. Otherwise, update the head to the next position in the circular buffer.
 * 3. If the head position equals the tail position, update the tail to the next position
 *    in the circular buffer and set the replaced flag to true.
 * 4. Copy the new message into the storage at the head position.
 *
 * Example usage:
 * @code
 * RecentMessages *messages = init_recent_messages(10);
 * if (messages != NULL) {
 *     bool replaced = add_recent_messages(messages, "Hello, world!");
 *     if (replaced) {
 *         // Handle message replacement logic
 *     }
 * }
 * @endcode
 */
bool add_recent_messages(RecentMessages *recent_messages, char *data);


/**
 * Retrieves a message from the RecentMessages buffer based on the given index.
 *
 * Fetches the message at the specified index relative to the current head of the buffer.
 * The index is used to access messages in a reverse chronological order, with 0 being the
 * most recent message.
 *
 * @param recent_messages A pointer to the RecentMessages buffer.
 * @param result A pointer to a buffer where the retrieved message will be copied. The buffer
 *               must be large enough to hold a message of size MESSAGE_SIZE.
 * @param index The index relative to the current head position. An index of 0 fetches the
 *              most recent message, 1 fetches the message before that, and so on.
 *
 * @return A boolean value indicating whether the retrieval was successful:
 *         - true: the message was successfully retrieved and copied to the result buffer.
 *         - false: the buffer is empty or the index is out of range.
 *
 * The function performs the following checks and steps:
 * 1. If the buffer is empty (head is INVALID_ADDRESS), return false.
 * 2. If the index is out of range (greater than or equal to the buffer size), return false.
 * 3. Calculate the real index in the circular buffer based on the head and the given index.
 * 4. Copy the message at the calculated index to the result buffer.
 *
 * Example usage:
 * @code
 * char result[MESSAGE_SIZE];
 * bool success = get_head_recent_messages(messages, result, 0);
 * if (success) {
 *     // Use the retrieved message stored in result
 * } else {
 *     // Handle retrieval failure
 * }
 * @endcode
 */
bool get_head_recent_messages(RecentMessages *recent_messages, char *result, size_t index);


/**
 * Retrieves a message from the RecentMessages buffer based on the given index.
 *
 * Fetches the message at the specified index relative to the current tail of the buffer.
 * The index is used to access messages in a chronological order, with 0 being the oldest message.
 *
 * @param recent_messages A pointer to the RecentMessages buffer.
 * @param result A pointer to a buffer where the retrieved message will be copied. The buffer
 *               must be large enough to hold a message of size MESSAGE_SIZE.
 * @param index The index relative to the current tail position. An index of 0 fetches the
 *              oldest message, 1 fetches the next oldest message, and so on.
 *
 * @return A boolean value indicating whether the retrieval was successful:
 *         - true: the message was successfully retrieved and copied to the result buffer.
 *         - false: the buffer is empty or the index is out of range.
 *
 * The function performs the following checks and steps:
 * 1. If the buffer is empty (tail is INVALID_ADDRESS), return false.
 * 2. If the index is out of range (greater than or equal to the buffer size), return false.
 * 3. Calculate the real index in the circular buffer based on the tail and the given index.
 * 4. Copy the message at the calculated index to the result buffer.
 *
 * Example usage:
 * @code
 * char result[MESSAGE_SIZE];
 * bool success = get_tail_recent_messages(messages, result, 0);
 * if (success) {
 *     // Use the retrieved message stored in result
 * } else {
 *     // Handle retrieval failure
 * }
 * @endcode
 */
bool get_tail_recent_messages(RecentMessages *recent_messages, char *result, size_t index);


/**
 * Frees the memory allocated for a RecentMessages buffer.
 *
 * Deallocates the memory used by the RecentMessages buffer and its associated storage array.
 * This function should be called when the buffer is no longer needed to prevent memory leaks.
 *
 * @param recent_messages A pointer to the RecentMessages buffer to be freed.
 *
 * The function performs the following steps:
 * 1. Frees the memory allocated for each message slot in the storage array.
 * 2. Frees the memory allocated for the storage array itself.
 * 3. Frees the memory allocated for the RecentMessages structure.
 *
 * Example usage:
 * @code
 * free_recent_messages(messages);
 * @endcode
 */
void free_recent_messages(RecentMessages *recent_messages);


#endif //SERVER_RECENT_MESSAGES_H
