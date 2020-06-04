//
// Created by ahmad on 6/3/20.
//

#include "worker.h"
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "request.h"
#include "response.h"

void *worker_handler(void *args);

void *client_connection_handler(void *input)
{
    request request;
    char *response;

    int client_socket_id = *((int *)input);

    request = read_request(client_socket_id);
    printf("|%s|%s|%s|(", request.method, request.path, request.protocol);
    for (int i = 0; i < request.header_size; ++i)
    {
        printf("|%s:%s|", request.headers[i].name, request.headers[i].value);
    }
    printf(")\n");
    if (request.body_size != 0)
    {
        printf("body: %s\n", request.body);
    }

    response = raw_response(generate_file_response(request.path));
    write(client_socket_id, response, strlen(response));

    fprintf(stdout, "Message Complete\n");

    close(client_socket_id);


    return NULL;
}

void worker_start(worker w)
{
    w->state = RUNNING;
    pthread_create(&(w->tid), NULL, worker_handler, w);
}

worker new_worker()
{
    worker w = malloc(sizeof(worker_s));
    w->cq = new_conn_q();
    w->state = INIT;

    return w;
}

void worker_enq_connection(worker w, int client_socket_id)
{
    conn_q_enqueue(w->cq, client_socket_id);
}

void *worker_handler(void *args)
{
    printf("Spawn a new worker thread\n");
    worker w = args;

    while (w->state == RUNNING)
    {
        conn_q_wait_on_empty(w->cq);
        int client_socket_id = conn_q_dequeue(w->cq);

        request request;
        char *response;

        request = read_request(client_socket_id);
        printf("|%s|%s|%s|(", request.method, request.path, request.protocol);
        for (int i = 0; i < request.header_size; ++i)
        {
            printf("|%s:%s|", request.headers[i].name, request.headers[i].value);
        }
        printf(")\n");
        if (request.body_size != 0)
        {
            printf("body: %s\n", request.body);
        }

        response = raw_response(generate_file_response(request.path));
        write(client_socket_id, response, strlen(response));

        fprintf(stdout, "Message Complete\n");

        close(client_socket_id);
    }

    return NULL;
}