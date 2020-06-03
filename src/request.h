//
// Created by ahmad on 3/20/20.
//

#ifndef QUARANTINE_REQUEST_H
#define QUARANTINE_REQUEST_H

#include <stdlib.h>
#include <stdbool.h>
#include "message.h"

typedef struct {
    char method[10];
    char path[200];
    char protocol[10];
    header *headers;
    size_t header_size;

    char *body;
    size_t body_size;
} request;

bool is_request_complete(const char request[], size_t request_size);
request parse_request(const char *request_stream, size_t stream_size);
request read_request(int socket_id);


#endif //QUARANTINE_REQUEST_H
