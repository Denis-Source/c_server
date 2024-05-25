#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H

#include "stdlib.h"
#include <mqueue.h>
#include <stdbool.h>

#define MESSAGE_BUFF_SIZE 256
#define MAX_MESSAGES 16
#define MAX_MSG_SIZE 1024


typedef enum {
    NOT_SPECIFIED,
    OPEN_CONNECTION,
    CLOSE_CONNECTION,
    UNICAST,
    BROADCAST,
    BAN
} MessageType;

typedef struct {
    MessageType type;
    char buff[MESSAGE_BUFF_SIZE];
} Message;

typedef struct {
    char *name;
} Queue;

bool create_queue(char *name, Queue *queue);

bool send_queue(Queue *queue, Message *message);

bool read_queue(Queue *queue, Message *message);

#endif //SERVER_QUEUE_H
