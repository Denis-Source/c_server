cmake_minimum_required(VERSION 3.16)
project(server C)

set(CMAKE_C_STANDARD 11)

add_executable(server main.c connection/connection.c connection/connection.h misc/misc.c misc/misc.h connection_handler/connection_handler.c connection_handler/connection_handler.h hash_table/table.c hash_table/table.h hash_table/hash.c hash_table/hash.h queue/queue.h queue/queue.c listener/listener.c listener/listener.h error_codes.h)
target_link_libraries(server -pthread)
target_link_libraries(server -lrt)
