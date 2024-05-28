#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H

#include <stdio.h>
#include <mqueue.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

#include "../connection/connection.h"

#define QUEUE_MAX_MESSAGES 10
#define QUEUE_PAYLOAD_SIZE 256
#define QUEUE_PERMISSIONS 0660

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


typedef struct {
    QMessageType type;
    Connection *connection;
    char payload[QUEUE_PAYLOAD_SIZE];
} QMessage;

typedef struct {
    bool valid;
    char *name;
} Queue;

void populate_message(QMessage *message, QMessageType type, Connection *connection, char *payload);

Queue *create_queue(char *name);

bool send_queue(Queue *queue, QMessage *message);

bool read_queue(Queue *queue, QMessage *message);

void free_queue(Queue *queue);

#endif //SERVER_QUEUE_H
