#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "request.h"
#include "response.h"


#define PORT 8020

typedef int err_t;

void *connection_handler(void *input);

static void catch_function(int signo)
{
    puts("Interactive attention signal caught.");
    exit(0);
}

int main()
{
    signal(SIGINT, catch_function);
    signal(SIGTERM, catch_function);
    signal(SIGSEGV, catch_function);
    signal(SIGABRT, catch_function);
    signal(SIGKILL, catch_function);

    pthread_t connection_handler_tid;
    pthread_create(&connection_handler_tid, NULL, connection_handler, NULL);


    pthread_join(connection_handler_tid, NULL);

    return EXIT_SUCCESS;
}

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

void *connection_handler(void *input)
{
    err_t err;
    int client_socket_ids[50];
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
    server_addr.sin_port = htons(PORT);
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

    printf("Server started on: %d\n", PORT);

    bool finished = false;
    do
    {
        int client_socket_id = 0;
        struct sockaddr_storage server_storage;
        socklen_t addr_size = sizeof server_storage;
        client_socket_id = accept(socket_id, (struct sockaddr *) &server_storage, &addr_size);
        client_socket_ids[client_counts] = client_socket_id;
        pthread_t client_connection_handler_tid;
        pthread_create(&client_connection_handler_tid, NULL, client_connection_handler, &client_socket_ids[client_counts]);
        client_counts++;
    }
    while (!finished);

    close(socket_id);

    return NULL;
}
