#include "queue.h"


void populate_message(QMessage *message, QMessageType type, Connection *connection, char *payload) {
    message->type= type;
    message->connection = connection;
    if (payload != NULL) memcpy(message->payload, payload, MESSAGE_BUFF_SIZE);
}

void populate_queue(Queue *queue, char *name, bool valid) {
    queue->valid = valid;
    queue->name = name;
}

Queue *create_queue(char *name) {
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) return NULL;

    struct mq_attr attr = {
            .mq_flags = 0,
            .mq_maxmsg = MAX_MESSAGES,
            .mq_msgsize = MAX_MSG_SIZE,
            .mq_curmsgs = 0,
    };

    mqd_t mqd = mq_open(name, O_CREAT | O_RDWR, QUEUE_PERMISSIONS, &attr);
    if (mqd == (mqd_t) -1) return NULL;

    populate_queue(queue, name, true);
    return queue;
}

bool send_queue(Queue *queue, QMessage *message) {
    size_t data_size = sizeof(QMessageType) + sizeof(Connection);
    char buffer[data_size + MESSAGE_BUFF_SIZE];
    memcpy(buffer, &message->type, sizeof(QMessageType));
    memcpy(buffer + sizeof(QMessageType), &message->connection, sizeof(Connection));
    memcpy(buffer + data_size, message->payload, MESSAGE_BUFF_SIZE);

    mqd_t mq = mq_open(queue->name, O_WRONLY);


    if (mq_send(mq, buffer, 256, 0) == -1) {
        perror("mq_send");
        queue->valid = false;
        mq_close(mq);
        return false;
    }
    mq_close(mq);
    return true;
}

bool read_queue(Queue *queue, QMessage *message) {
    size_t data_size = sizeof(QMessageType) + sizeof(Connection);
    char buffer[data_size + MESSAGE_BUFF_SIZE];

    mqd_t mq = mq_open(queue->name, O_RDONLY);
    if (mq == (mqd_t)-1) return false;

    if (mq_receive(mq, buffer, sizeof(buffer), NULL) == -1) {
        queue->valid = false;
        mq_close(mq);
        return false;
    }
    mq_close(mq);

    memcpy(&message->type, buffer, sizeof(QMessageType));
    memcpy(&message->connection, buffer + sizeof(QMessageType), sizeof(Connection));
    memcpy(message->payload, buffer + data_size, MESSAGE_BUFF_SIZE);

    return true;
}
