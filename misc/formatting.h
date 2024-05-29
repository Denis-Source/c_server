#ifndef SERVER_FORMATTING_H
#define SERVER_FORMATTING_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../connection/connection.h"
#include "../definitions.h"


/**
 * Enum representing types of messages.
 *
 * This enum represents different types of messages that can be exchanged
 * within the messaging system. It includes types such as MESSAGE_CONNECTED,
 * MESSAGE_DISCONNECTED, and MESSAGE_SENT.
 *
 * The enum values are defined as follows:
 *  - MESSAGE_CONNECTED: Indicates a message indicating successful connection.
 *  - MESSAGE_DISCONNECTED: Indicates a message indicating disconnection.
 *  - MESSAGE_SENT: Indicates a message indicating successful transmission.
 *
 * Example usage:
 * @code
 * MessageType type = MESSAGE_CONNECTED;
 * @endcode
 */
typedef enum {
    MESSAGE_CONNECTED,
    MESSAGE_DISCONNECTED,
    MESSAGE_SENT,
} MessageType;


/**
 * Checks if a character is allowed based on a predefined set of symbols.
 *
 * This function checks whether a given character is allowed based on a predefined set of symbols.
 * It searches for the character in the MESSAGE_ALLOWED_SYMBOLS string and returns true if found,
 * indicating that the character is allowed, and false otherwise.
 *
 * @param c The character to be checked.
 *
 * @return true if the character is allowed, otherwise false.
 *
 * The function performs the following steps:
 * 1. Searches for the given character in the MESSAGE_ALLOWED_SYMBOLS string using strchr function.
 * 2. Returns true if the character is found in the allowed symbols string, indicating it's allowed.
 *    Returns false if the character is not found, indicating it's not allowed.
 *
 * Example usage:
 * @code
 * char character = '*';
 * if (is_allowed_char(character)) {
 *     // Character is allowed.
 * } else {
 *     // Character is not allowed.
 * }
 * @endcode
 */
bool is_allowed_char(char c);


/**
 * Sanitizes a buffer by removing disallowed characters.
 *
 * This function sanitizes a buffer by removing any characters that are not allowed
 * based on a predefined set of symbols. It iterates through the buffer and retains
 * only the allowed characters, replacing disallowed characters with null terminators.
 *
 * @param buffer A pointer to the buffer to be sanitized.
 * @param size The size of the buffer in bytes.
 *
 * @return true if the buffer contains only allowed characters after sanitization, otherwise false.
 *
 * The function performs the following steps:
 * 1. Iterates through the buffer character by character.
 * 2. Checks if each character is allowed using the is_allowed_char function.
 * 3. Retains allowed characters and replaces disallowed characters with null terminators.
 * 4. Returns true if the buffer contains only allowed characters after sanitization,
 *    indicating successful sanitization. Returns false otherwise.
 *
 * Example usage:
 * @code
 * char buffer[100] = "Hello$World!";
 * size_t buffer_size = strlen(buffer);
 * if (sanitize_buffer(buffer, buffer_size)) {
 *     // Buffer sanitized successfully.
 * } else {
 *     // Error: Buffer contains disallowed characters after sanitization.
 * }
 * @endcode
 */
bool sanitize_buffer(char *buffer, size_t size);


/**
 * Formats a message based on the message type and connection information.
 *
 * This function formats a message based on the specified message type and connection information.
 * It generates different formats for different message types such as connected, disconnected,
 * message sent, and default message types. The formatted message is stored in the provided result buffer.
 *
 * @param result A pointer to the buffer where the formatted message will be stored.
 * @param message A pointer to the message content to be included in the formatted message.
 *                This parameter is used for MESSAGE_SENT type messages.
 * @param connection A pointer to the Connection structure containing connection information.
 * @param type The type of the message to be formatted (MESSAGE_CONNECTED, MESSAGE_DISCONNECTED,
 *             MESSAGE_SENT, or default).
 *
 * The function performs the following steps:
 * 1. Formats the message based on the specified message type and connection information.
 * 2. Stores the formatted message in the provided result buffer.
 * 3. Appends a newline character at the end of the formatted message if it's not already present.
 *
 * Example usage:
 * @code
 * char result[256];
 * char message[] = "Hello, world!";
 * Connection *connection;
 * MessageType type = MESSAGE_SENT;
 * format_message(result, message, connection, type);
 * printf("%s", result); // Output: "<connection_name>: Hello, world!\n"
 * @endcode
 */
void format_message(char *message, char *result, Connection *connection, MessageType type);


#endif //SERVER_FORMATTING_H
