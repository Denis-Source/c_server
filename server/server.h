#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <stdio.h>

#include "../misc/misc.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../handler/handler.h"
#include "../listener/listener.h"


#define MAX_CONNECTIONS 256
#define QUEUE_NAME "/s2_server"

void server_serve();

#endif //SERVER_SERVER_H
