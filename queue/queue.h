#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H


#include <stdio.h>
#include <mqueue.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include "../connection/connection.h"
#include "../definitions.h"


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

typedef enum {
    QUEUE_MODE_READ,
    QUEUE_MODE_WRITE,
    QUEUE_MODE_READWRITE
} QueueType;

typedef struct {
    QMessageType type;
    Connection *connection;
    char payload[QUEUE_PAYLOAD_SIZE];
} QMessage;

typedef struct {
    QueueType type;
    int32_t mqd;
} Queue;


void populate_message(QMessage *message, QMessageType type, Connection *connection, char *payload);

void populate_queue(Queue *queue, QueueType type, int32_t mqd);

bool create_queues();

Queue *init_queue(QueueType type);

bool free_queue(Queue *queue);

bool send_queue(Queue *queue, QMessage *message);

bool read_queue(Queue *queue, QMessage *message);

void unlink_queues();

#endif //SERVER_QUEUE_H
