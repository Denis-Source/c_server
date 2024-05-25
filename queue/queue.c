#include <stdio.h>
#include "queue.h"
#include "memory.h"

void populate_queue(Queue *queue, mqd_t mqd, char *name) {
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
    populate_queue(queue, mqd, name);
    return true;
}

bool send_queue(Queue *queue, Message *message) {
    char buffer[sizeof(MessageType) + MESSAGE_BUFF_SIZE];
    memcpy(buffer, &message->type, sizeof(MessageType));
    memcpy(buffer + sizeof(MessageType), message->buff, MESSAGE_BUFF_SIZE);

    mqd_t mq = mq_open(queue->name, O_WRONLY);

    if (mq_send(mq, buffer, 256, 0) == -1) {
        perror("mq_send");
        mq_close(mq);
        return false;
    };

    return true;
}

bool read_queue(Queue *queue, Message *message) {
    char buffer[sizeof(MessageType) + MESSAGE_BUFF_SIZE];

    mqd_t mq = mq_open(queue->name, O_RDONLY);
    if (mq == (mqd_t)-1) return false;

    if (mq_receive(mq, buffer, sizeof(buffer), NULL) == -1) {
        mq_close(mq);
        return false;
    }
    mq_close(mq);

    memcpy(&message->type, buffer, sizeof(MessageType));
    memcpy(message->buff, buffer + sizeof(MessageType), MESSAGE_BUFF_SIZE);

    return true;
}

//void close_queue(Queue *queue) {
//    mq_close(queue->mqd);
//    mq_unlink(queue->name);
//}