#include "queue.h"


void populate_message(QMessage *message, QMessageType type, Connection *connection, char *payload) {
    message->type = type;
    message->connection = connection;
    if (payload != NULL) memcpy(message->payload, payload, QUEUE_PAYLOAD_SIZE);
}


void populate_queue(Queue *queue, QueueType type, int32_t mqd) {
    queue->type = type;
    queue->mqd = mqd;
}


bool create_queues() {
    size_t queue_message_size = QUEUE_PAYLOAD_SIZE + sizeof(QMessageType) + sizeof(Connection) + 1;

    struct mq_attr attr = {
            .mq_flags = 0,
            .mq_maxmsg = QUEUE_MAX_MESSAGES,
            .mq_msgsize = queue_message_size,
            .mq_curmsgs = 0,
    };
    mq_unlink(QUEUE_NAME);
    mqd_t mqd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, QUEUE_PERMISSIONS, &attr);
    if (mqd == (mqd_t) -1) {
        perror("mq_open");
        return false;
    }
    return true;
}


Queue *init_queue(QueueType type) {
    int32_t mode;
    switch (type) {
        case QUEUE_MODE_READ:
            mode = O_RDONLY;
            break;
        case QUEUE_MODE_WRITE:
            mode = O_WRONLY;
            break;
        default:
            mode = O_RDWR;
            break;
    }
    mqd_t mqd = mq_open(QUEUE_NAME, mode);
    if (mqd == -1) {
        perror("mq_send");
        return NULL;
    }
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) return NULL;

    populate_queue(queue, type, mqd);
    return queue;
}


bool free_queue(Queue *queue) {
    if (mq_close(queue->mqd) == -1) return false;
    free(queue);
    queue = NULL;

    return true;
}


bool send_queue(Queue *queue, QMessage *message) {
    if (queue->type == QUEUE_MODE_READ) return false;

    size_t queue_message_size = QUEUE_PAYLOAD_SIZE + sizeof(QMessageType) + sizeof(Connection) + 1;
    char buffer[queue_message_size];
    memcpy(buffer, &message->type, sizeof(QMessageType));
    memcpy(buffer + sizeof(QMessageType), &message->connection, sizeof(Connection));
    memcpy(buffer + sizeof(QMessageType) + sizeof(Connection), message->payload, QUEUE_PAYLOAD_SIZE);

    if (mq_send(queue->mqd, buffer, queue_message_size, 0) == -1) {
        perror("mq_send");
        return false;
    }
    return true;
}


bool read_queue(Queue *queue, QMessage *message) {
    if (queue->type == QUEUE_MODE_WRITE) return false;

    size_t queue_message_size = QUEUE_PAYLOAD_SIZE + sizeof(QMessageType) + sizeof(Connection);
    char buffer[queue_message_size];

    if (mq_receive(queue->mqd, buffer, queue_message_size + 1, NULL) == -1) {
        perror("mq_receive");
        return false;
    }

    memcpy(&message->type, buffer, sizeof(QMessageType));
    memcpy(&message->connection, buffer + sizeof(QMessageType), sizeof(Connection));
    memcpy(message->payload, buffer + sizeof(QMessageType) + sizeof(Connection), QUEUE_PAYLOAD_SIZE);

    return true;
}


void unlink_queues() {
    mq_unlink(QUEUE_NAME);
}