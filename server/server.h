#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "stdio.h"

#include "../misc/misc.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../connection_handler/connection_handler.h"
#include "../listener/listener.h"


#define PORT 6969
#define MAX_CONNECTIONS 256
#define QUEUE_NAME "/my_queue"

void server_serve();

#endif //SERVER_SERVER_H
