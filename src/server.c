//
// Created by ahmad on 6/3/20.
//

#include "server.h"
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "worker.h"

void *connection_handler(void *args);

server_ctx new_server_ctx(config c)
{
    server_ctx ctx = malloc(sizeof(server_ctx_s));
    ctx->port = c.port;
    ctx->workers_count = c.worker_count;

    ctx->workers = malloc(ctx->workers_count * sizeof(worker));
    for (int i = 0; i < c.worker_count; i++)
    {
        ctx->workers[i] = new_worker();
        worker_start(ctx->workers[i]);
    }

    return ctx;
}

void server_ctx_start(server_ctx ctx)
{
    pthread_t connection_handler_tid;

    pthread_create(&connection_handler_tid, NULL, connection_handler, ctx);

    pthread_join(connection_handler_tid, NULL);
}

void *connection_handler(void *args)
{
    server_ctx ctx = (server_ctx) args;

    err_t err;
    size_t client_counts = 0;

    int socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_id == -1)
    {
        fprintf(stderr, "Failed to open socket: `%s`", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ctx->port);
    server_addr.sin_addr.s_addr = INADDR_ANY;


    err = bind(socket_id, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (err == -1)
    {
        fprintf(stderr, "Failed: bind()\n%s\n", strerror(errno));
        close(socket_id);    // Don't forget to close the socket.
        exit(EXIT_FAILURE);
    }

    err = listen(socket_id, 5);
    if (err == -1)
    {
        fprintf(stderr, "Failed: connect()\n%s\n", strerror(errno));
        close(socket_id);    // Don't forget to close the socket.
        exit(1);
    }

    printf("Server started on: %d\n", ctx->port);

    bool finished = false;
    do
    {
        int client_socket_id = 0;
        struct sockaddr_storage server_storage;
        socklen_t addr_size = sizeof server_storage;
        client_socket_id = accept(socket_id, (struct sockaddr *) &server_storage, &addr_size);
        client_counts++;
        worker_enq_connection(ctx->workers[client_counts % ctx->workers_count], client_socket_id);
    }
    while (!finished);

    close(socket_id);

    return NULL;
}
