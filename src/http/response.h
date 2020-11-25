//
// Created by ahmad on 3/21/20.
//

#ifndef QUARANTINE_RESPONSE_H
#define QUARANTINE_RESPONSE_H

#include "header.h"

typedef struct {
    char protocol[20];
    char status_code[50];
    header *headers;
    size_t header_size;

    char *body;
    size_t body_size;
} response;


//////////////////// HTTP Status Codes /////////////////////////
// 2XX Status Codes
#define HTTP_STATUS_OK          "200 OK"
#define HTTP_STATUS_CREATED     "201 Created"
#define HTTP_STATUS_ACCEPTED    "202 Accepted"

// 4XX Status Codes
#define HTTP_STATUS_NOT_FOUND    "404 Not Found"


char *raw_response(response res);

response generate_sample_response();
response generate_file_response(const char path[]);
response generate_not_found_response();

#endif //QUARANTINE_RESPONSE_H
