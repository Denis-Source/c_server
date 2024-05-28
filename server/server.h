#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <stdio.h>
#include "../definitions.h"
#include "../misc/misc.h"
#include "../queue/queue.h"
#include "../hash_table/table.h"
#include "../handler/handler.h"
#include "../listener/listener.h"
#include "../circular_buffer/recent_messages.h"
#include "context.h"


void server_serve();


#endif //SERVER_SERVER_H
