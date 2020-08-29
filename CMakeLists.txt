cmake_minimum_required(VERSION 3.15)
project(quarantine C)

set(CMAKE_C_STANDARD 11)

add_executable(quarantine src/main.c src/request.c src/request.h src/response.c src/response.h src/message.h src/conn_q.c src/conn_q.h src/server.c src/server.h src/types.h src/worker.c src/worker.h src/config.h src/log.h)

target_link_libraries(quarantine pthread)

enable_testing()