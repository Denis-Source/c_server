#include <stdio.h>
#include "queue.h"
#include "memory.h"

void populate_message(Message *message, MessageType type, Connection *connection, char *payload) {
    message->type= type;
    message->connection = connection;
    if (payload != NULL) memcpy(message->payload, payload, MESSAGE_BUFF_SIZE);
}

void populate_queue(Queue *queue, char *name, bool valid) {
    queue->valid = valid;
    queue->name = name;
}

bool create_queue(char *name, Queue *queue) {
    struct mq_attr attr = {
            .mq_flags = 0,
            .mq_maxmsg = MAX_MESSAGES,
            .mq_msgsize = MAX_MSG_SIZE,
            .mq_curmsgs = 0,
    };

    mqd_t mqd = mq_open(name, O_CREAT | O_RDWR, 0660, &attr);
    if (mqd == (mqd_t) -1) return false;
    populate_queue(queue, name, true);
    return true;
}

bool send_queue(Queue *queue, Message *message) {
    size_t data_size = sizeof(MessageType) + sizeof(Connection);
    char buffer[data_size + MESSAGE_BUFF_SIZE];
    memcpy(buffer, &message->type, sizeof(MessageType));
    memcpy(buffer + sizeof(MessageType), &message->connection, sizeof(Connection));
    memcpy(buffer + data_size, message->payload, MESSAGE_BUFF_SIZE);

    mqd_t mq = mq_open(queue->name, O_WRONLY);


    if (mq_send(mq, buffer, 256, 0) == -1) {
        perror("mq_send");
        queue->valid = false;
        mq_close(mq);
        return false;
    };

    return true;
}

bool read_queue(Queue *queue, Message *message) {
    size_t data_size = sizeof(MessageType) + sizeof(Connection);
    char buffer[data_size + MESSAGE_BUFF_SIZE];

    mqd_t mq = mq_open(queue->name, O_RDONLY);
    if (mq == (mqd_t)-1) return false;

    if (mq_receive(mq, buffer, sizeof(buffer), NULL) == -1) {
        queue->valid = false;
        mq_close(mq);
        return false;
    }
    mq_close(mq);

    memcpy(&message->type, buffer, sizeof(MessageType));
    memcpy(&message->connection, buffer + sizeof(MessageType), sizeof(Connection));
    memcpy(message->payload, buffer + data_size, MESSAGE_BUFF_SIZE);

    return true;
}
