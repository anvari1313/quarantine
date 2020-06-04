#include <stdlib.h>
#include <stdio.h>
#include "server.h"
#include <signal.h>
#include "config.h"

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

    config c = {.port = 8020, .worker_count = 10};

    server_ctx server = new_server_ctx(c);
    server_ctx_start(server);

    return EXIT_SUCCESS;
}
