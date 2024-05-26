#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H

#include "stdlib.h"
#include "../connection/connection.h"
#include <mqueue.h>
#include <stdbool.h>

#define MESSAGE_BUFF_SIZE 256
#define MAX_MESSAGES 16
#define MAX_MSG_SIZE 1024


typedef enum {
    MESSAGE_NOT_SPECIFIED,
    MESSAGE_LISTENING,
    MESSAGE_OPEN_CONNECTION,
    MESSAGE_CLOSE_CONNECTION,
    MESSAGE_RECEIVED,
    MESSAGE_STRIKE,
    MESSAGE_STOP_LISTENING
} MessageType;

typedef struct {
    MessageType type;
    Connection *connection;
    char payload[MESSAGE_BUFF_SIZE];
} Message;

typedef struct {
    bool valid;
    char *name;
} Queue;

void populate_message(Message *message, MessageType type, Connection *connection, char *payload);

bool create_queue(char *name, Queue *queue);

bool send_queue(Queue *queue, Message *message);

bool read_queue(Queue *queue, Message *message);

#endif //SERVER_QUEUE_H
