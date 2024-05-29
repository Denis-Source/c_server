#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H


#include <stdio.h>
#include <mqueue.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include "../connection/connection.h"
#include "../definitions.h"


/**
 * Enumeration representing the type of messages exchanged between components.
 *
 * This enumeration defines the various types of messages that can be exchanged between different components
 * within the system. Each message type corresponds to a specific action or event, such as starting or stopping
 * listening for connections, opening or closing a connection, receiving data, and handling strikes or bans.
 *
 * The following message types are defined:
 *  - Q_MESSAGE_NOT_SPECIFIED: Indicates that the message type is not specified.
 *  - Q_MESSAGE_START_LISTENING: Indicates the start of listening for connections.
 *  - Q_MESSAGE_OPEN_CONNECTION: Indicates the opening of a connection.
 *  - Q_MESSAGE_CLOSE_CONNECTION: Indicates the closing of a connection.
 *  - Q_MESSAGE_RECEIVED: Indicates the reception of data.
 *  - Q_MESSAGE_STRIKE: Indicates a strike action, such as a warning or penalty.
 *  - Q_MESSAGE_BAN: Indicates a ban action, prohibiting further access or communication.
 *  - Q_MESSAGE_STOP_LISTENING: Indicates the stop of listening for connections.
 *
 * Example usage:
 * @code
 * QMessageType type = Q_MESSAGE_OPEN_CONNECTION;
 * @endcode
 */
typedef enum {
    Q_MESSAGE_NOT_SPECIFIED,
    Q_MESSAGE_START_LISTENING,
    Q_MESSAGE_OPEN_CONNECTION,
    Q_MESSAGE_CLOSE_CONNECTION,
    Q_MESSAGE_RECEIVED,
    Q_MESSAGE_STRIKE,
    Q_MESSAGE_BAN,
    Q_MESSAGE_STOP_LISTENING
} QMessageType;


/**
 * Enumeration representing the mode of operation for a message queue.
 *
 * This enumeration defines the different modes of operation for a message queue, specifying whether it is
 * intended for reading, writing, or both reading and writing messages. Each mode corresponds to a specific
 * behavior of the message queue in terms of message transmission and reception.
 *
 * The following queue modes are defined:
 *  - QUEUE_MODE_READ: Indicates that the message queue is intended for reading messages only.
 *  - QUEUE_MODE_WRITE: Indicates that the message queue is intended for writing messages only.
 *  - QUEUE_MODE_READWRITE: Indicates that the message queue supports both reading and writing messages.
 *
 * Example usage:
 * @code
 * QueueType type = QUEUE_MODE_READ;
 * @endcode
 */
typedef enum {
    QUEUE_MODE_READ,
    QUEUE_MODE_WRITE,
    QUEUE_MODE_READWRITE
} QueueType;


/**
 * Structure representing a message exchanged between components.
 *
 * This structure represents a message exchanged between different components within the system.
 * It contains information about the message type, the associated connection (if applicable),
 * and a payload containing the message data.
 *
 * The structure fields are defined as follows:
 *  - type: The type of the message, indicating the action or event represented by the message.
 *  - connection: A pointer to the Connection structure associated with the message, if applicable.
 *  - payload: An array containing the message data or payload.
 *
 * Example usage:
 * @code
 * QMessage message;
 * message.type = Q_MESSAGE_RECEIVED;
 * message.connection = client_connection;
 * strncpy(message.payload, data, QUEUE_PAYLOAD_SIZE);
 * @endcode
 */
typedef struct {
    QMessageType type;
    Connection *connection;
    char payload[QUEUE_PAYLOAD_SIZE];
} QMessage;


/**
 * Structure representing a message queue.
 *
 * This structure represents a message queue used for communication between components within the system.
 * It contains information about the queue type and the message queue descriptor (mqd) associated with it.
 *
 * The structure fields are defined as follows:
 *  - type: The type of the message queue, specifying whether it is intended for reading, writing, or both.
 *  - mqd: The message queue descriptor obtained from mq_open, used for interacting with the message queue.
 *
 * Example usage:
 * @code
 * Queue queue;
 * queue.type = QUEUE_MODE_READ;
 * queue.mqd = mq_open(QUEUE_NAME, O_RDONLY);
 * @endcode
 */
typedef struct {
    QueueType type;
    int32_t mqd;
} Queue;


/**
 * Populates a QMessage structure with the specified type, connection, and payload.
 *
 * This function populates a QMessage structure with the specified message type, connection,
 * and payload data. It assigns the provided values to the respective fields of the QMessage structure.
 *
 * @param message A pointer to the QMessage structure to be populated.
 * @param type The type of the message.
 * @param connection A pointer to the Connection structure associated with the message.
 * @param payload A pointer to the payload data to be copied into the message structure.
 *                If NULL, the payload field of the message structure remains unchanged.
 *
 * The function performs the following steps:
 * 1. Assigns the specified message type to the 'type' field of the QMessage structure.
 * 2. Assigns the provided connection pointer to the 'connection' field of the QMessage structure.
 * 3. If a non-NULL payload pointer is provided, copies the payload data into the 'payload' field
 *    of the QMessage structure using memcpy.
 *
 * Example usage:
 * @code
 * QMessage message;
 * Connection *connection = create_connection();
 * char payload[QUEUE_PAYLOAD_SIZE] = "Hello, world!";
 * populate_message(&message, Q_MESSAGE_RECEIVED, connection, payload);
 * @endcode
 */
void populate_message(QMessage *message, QMessageType type, Connection *connection, char *payload);


/**
 * Populates a Queue structure with the specified type and message queue descriptor.
 *
 * This function populates a Queue structure with the specified queue type and message queue descriptor (mqd).
 * It assigns the provided queue type and message queue descriptor to the respective fields
 * in the Queue structure.
 *
 * @param queue A pointer to the Queue structure to be populated.
 * @param type The type of the message queue.
 * @param mqd The message queue descriptor obtained from mq_open.
 *
 * The function performs the following steps:
 * 1. Assigns the specified queue type to the 'type' field of the Queue structure.
 * 2. Assigns the provided message queue descriptor to the 'mqd' field of the Queue structure.
 *
 * Example usage:
 * @code
 * Queue queue;
 * populate_queue(&queue, QUEUE_MODE_READWRITE, mqd);
 * @endcode
 */
void populate_queue(Queue *queue, QueueType type, int32_t mqd);


/**
 * Unlinks (deletes) a message queue from the system.
 *
 * This function unlinks (deletes) the message queue with the specified name from the system.
 * It removes the message queue from the system's namespace, making it inaccessible for further use.
 * If the queue deletion operation is successful, the function completes without returning any value.
 * If an error occurs during queue deletion, it may print an error message, but it does not return any
 * explicit indication of success or failure.
 *
 * The function performs the following steps:
 * 1. Calls mq_unlink function to unlink (delete) the message queue with the specified name from the system.
 * 2. If the operation is successful, the message queue is removed from the system's namespace.
 *    If an error occurs, the function may print an error message, but it does not return any explicit indication.
 *
 * Example usage:
 * @code
 * unlink_queue();
 * @endcode
 */
void unlink_queue();


/**
 * Creates an OS message queue for inter-process communication.
 *
 * This function creates a message queue for inter-process communication.
 * It sets up the message queue attributes including message size, maximum number of messages,
 * and permissions. If a message queue with the same name already exists, it is unlinked before
 * creating a new one. The function returns true upon successful creation of the message queue
 * and false otherwise, along with an error message if applicable.
 *
 * @return true if the message queue is successfully created, otherwise false.
 *
 * The function performs the following steps:
 * 1. Calculates the message size based on the payload size, message type, connection structure size,
 *    and an additional byte for null terminator.
 * 2. Sets up the message queue attributes including flags, maximum number of messages, message size,
 *    and current number of messages.
 * 3. Unlinks any existing message queue with the same name to ensure a fresh creation.
 * 4. Opens or creates the message queue with the specified name, permissions, and attributes.
 * 5. Returns true if the message queue is successfully created.
 *    Returns false if an error occurs during message queue creation, along with an error message.
 *
 * Example usage:
 * @code
 * if (create_queue()) {
 *     // Message queue created successfully.
 * } else {
 *     // Error: Failed to create message queue.
 * }
 * @endcode
 */
bool create_queue();


/**
 * Opens a message queue for reading or writing.
 *
 * This function opens a message queue for reading, writing, or both, based on the specified queue type.
 * It translates the queue type into the appropriate mode for opening the queue (read, write, or read-write).
 * Upon successful opening of the queue, it initializes a Queue structure to represent the opened queue
 * and returns a pointer to the Queue structure. If opening the queue fails, it returns NULL along with
 * an error message.
 *
 * @param type The type of queue to be opened (read, write, or read-write).
 *
 * @return A pointer to the opened Queue structure if successful, otherwise NULL.
 *
 * The function performs the following steps:
 * 1. Determines the mode for opening the queue based on the specified queue type (read, write, or read-write).
 * 2. Opens the message queue with the specified name and mode using mq_open function.
 * 3. Checks if the queue opening operation was successful. If not, it prints an error message and returns NULL.
 * 4. Allocates memory for a Queue structure to represent the opened queue.
 * 5. Populates the Queue structure with the necessary information, including type and message queue descriptor.
 * 6. Returns a pointer to the initialized Queue structure if successful.
 *
 * Example usage:
 * @code
 * Queue *read_queue = open_queue(QUEUE_MODE_READ);
 * if (read_queue != NULL) {
 *     // Message queue opened for reading successfully.
 * } else {
 *     // Error: Failed to open message queue for reading.
 * }
 * @endcode
 */
Queue *open_queue(QueueType type);


/**
 * Closes a message queue and frees associated memory.
 *
 * This function closes the specified message queue and frees the memory associated with the Queue structure.
 * It also sets the queue pointer to NULL to prevent accidental access to the closed queue.
 * If the queue is successfully closed, the function returns true; otherwise, it returns false.
 *
 * @param queue A pointer to the Queue structure representing the message queue to be closed.
 *
 * @return true if the queue is successfully closed, otherwise false.
 *
 * The function performs the following steps:
 * 1. Closes the message queue specified by the message queue descriptor (mqd) in the Queue structure.
 * 2. Checks if the queue closing operation was successful. If not, it returns false.
 * 3. Frees the memory allocated for the Queue structure to prevent memory leaks.
 * 4. Sets the queue pointer to NULL to indicate that the queue has been closed.
 * 5. Returns true to indicate successful closure of the queue.
 *
 * Example usage:
 * @code
 * if (close_queue(queue)) {
 *     // Message queue closed successfully.
 * } else {
 *     // Error: Failed to close message queue.
 * }
 * @endcode
 */
bool close_queue(Queue *queue);


/**
 * Sends a message through a message queue.
 *
 * This function sends a message through the specified message queue. It constructs a buffer containing
 * the message type, connection information, and payload, and sends this buffer through the queue.
 * If the queue type is set to read-only, indicating that it cannot be used for sending messages,
 * the function returns false. If the message is successfully sent, the function returns true; otherwise,
 * it returns false along with an error message.
 *
 * @param queue A pointer to the Queue structure representing the message queue.
 * @param message A pointer to the QMessage structure containing the message to be sent.
 *
 * @return true if the message is successfully sent, otherwise false.
 *
 * The function performs the following steps:
 * 1. Checks if the queue type is set to read-only. If so, returns false as read-only queues cannot send messages.
 * 2. Constructs a buffer containing the message type, connection information, and payload.
 * 3. Sends the constructed buffer through the message queue using mq_send function.
 * 4. Checks if the message sending operation was successful. If not, prints an error message and returns false.
 * 5. Returns true to indicate that the message was successfully sent.
 *
 * Example usage:
 * @code
 * QMessage message;
 * // Populate message with required data...
 * if (send_queue(queue, &message)) {
 *     // Message sent successfully.
 * } else {
 *     // Error: Failed to send message.
 * }
 * @endcode
 */
bool send_queue(Queue *queue, QMessage *message);


/**
 * Reads a message from the message queue.
 *
 * This function reads a message from the specified message queue and populates the provided QMessage structure with the message content.
 *
 * @param queue A pointer to the Queue structure representing the message queue.
 * @param message A pointer to the QMessage structure where the read message will be stored.
 * @return true if the message is successfully read and populated into the QMessage structure, false otherwise.
 *
 * The function performs the following steps:
 * 1. Checks if the queue type is QUEUE_MODE_WRITE, indicating that reading is not allowed. If so, returns false.
 * 2. Calculates the size of the message to be received based on the payload size, message type size, and connection size.
 * 3. Declares a buffer to store the received message content.
 * 4. Attempts to receive a message from the message queue using mq_receive.
 * 5. If the receive operation fails, prints an error message and returns false.
 * 6. Extracts the message type, connection information, and payload from the received buffer and populates the provided QMessage structure.
 * 7. Returns true to indicate successful message read and population.
 *
 * Example usage:
 * @code
 * Queue *queue = open_queue(QUEUE_MODE_READ);
 * QMessage message;
 * if (read_queue(queue, &message)) {
 *     // Process the received message
 * } else {
 *     // Handle the read failure
 * }
 * close_queue(queue);
 * @endcode
 */
bool read_queue(Queue *queue, QMessage *message);


#endif //SERVER_QUEUE_H
