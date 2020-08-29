//
// Created by ahmad on 3/22/20.
//

#ifndef QUARANTINE_MESSAGE_H
#define QUARANTINE_MESSAGE_H

typedef struct {
    char name[200];
    char value[200];
} header;

// HTTP protocol versions
#define HTTP_PROTOCOL_V11 "HTTP/1.1"
#define HTTP_PROTOCOL_V10 "HTTP/1.0"

// HTTP header keys
#define HTTP_HEADER_DATE                "Date"
#define HTTP_HEADER_SERVER              "Server"
#define HTTP_HEADER_CONTENT_LENGTH      "Content-Length"
#define HTTP_HEADER_CONNECTION          "Connection"
#define HTTP_HEADER_CONTENT_TYPE        "Content-Type"

// HTTP header values
#define HTTP_HEADER_CONNECTION_CLOSED           "Closed"
#define HTTP_HEADER_CONTENT_TYPE_TEXT_HTML      "text/html; charset=iso-8859-1"
#define HTTP_HEADER_CONTENT_TYPE_APP_JSON      "application/json"

#define HTTP_HEADER_QUARANTINE_SERVER              "quarantine/0.0.1"

#endif //QUARANTINE_MESSAGE_H
