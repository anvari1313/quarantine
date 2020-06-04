//
// Created by ahmad on 6/3/20.
//

#ifndef QUARANTINE_SERVER_H
#define QUARANTINE_SERVER_H

#include <stdlib.h>
#include "config.h"
#include "worker.h"

typedef struct {
    unsigned int port;
    worker *workers;
    size_t workers_count;
} server_ctx_s;

typedef server_ctx_s *server_ctx;

server_ctx new_server_ctx(config);
void server_ctx_start(server_ctx);

#endif //QUARANTINE_SERVER_H
