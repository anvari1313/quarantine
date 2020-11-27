cmake_minimum_required(VERSION 3.15)
project(quarantine C)

set(CMAKE_C_STANDARD 11)

add_executable(quarantine src/main.c src/http/request.c src/http/request.h src/http/response.c src/http/response.h src/http/header.h src/conn_q.c src/conn_q.h src/server.c src/server.h src/types.h src/worker.c src/worker.h src/config.h src/log.h src/log.c)

target_link_libraries(quarantine pthread)

enable_testing()