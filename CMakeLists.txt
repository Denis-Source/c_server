cmake_minimum_required(VERSION 3.16)
project(server C)

set(CMAKE_C_STANDARD 11)

add_executable(server main.c connection/connection.c connection/connection.h misc/misc.c misc/misc.h handler/handler.c handler/handler.h hash_table/table.c hash_table/table.h hash_table/hash.c hash_table/hash.h queue/queue.h queue/queue.c listener/listener.c listener/listener.h server/server.c server/server.h circular_buffer/recent_messages.c circular_buffer/recent_messages.h definitions.h server/context.c server/context.h)
target_link_libraries(server -lpthread)
target_link_libraries(server -lrt)
