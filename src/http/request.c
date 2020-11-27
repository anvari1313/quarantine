//
// Created by ahmad on 3/20/20.
//

#include "request.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define REQUEST_BUFFER_SIZE 2048

request read_request(int socket_id)
{
    request result;

    char buffer[REQUEST_BUFFER_SIZE];
    size_t read_bytes;
    size_t i, j;

    do
    {
        read_bytes = read(socket_id, buffer, REQUEST_BUFFER_SIZE);

        i = 0;

        // Parse method
        j = 0;
        while ((i < read_bytes) && (buffer[i] != ' '))
        {
            result.method[j++] = buffer[i++];
        }
        result.method[j] = '\0';

        // Parse path
        j = 0;
        i++;
        while ((i < read_bytes) && (buffer[i] != ' '))
        {
            result.path[j++] = buffer[i++];
        }
        result.path[j] = '\0';

        // Parse protocol
        j = 0;
        i++;
        do
        {
            result.protocol[j++] = buffer[i++];
        }
        while ((i < read_bytes) && (buffer[i] != '\r') && (buffer[i] != '\n'));
        result.protocol[j] = '\0';

        // Parse headers
        i += 2;

        result.headers = malloc(500 * sizeof(header));
        result.header_size = 0;

        while (! ((buffer[i] == '\r') && (buffer[i + 1] == '\n')))
        {
            j = 0;
            while ((i < read_bytes) && (buffer[i] != ':'))
            {
                result.headers[result.header_size].name[j++] = buffer[i++];
            }

            result.headers[result.header_size].name[j] = '\0';

            i += 2;

            j = 0;
            while ((i < read_bytes) && (buffer[i] != '\r') && (buffer[i] != '\n'))
            {
                result.headers[result.header_size].value[j++] = buffer[i++];
            }
            result.headers[result.header_size].value[j] = '\0';

            result.header_size ++;
            i += 2;
        }

        if (strcmp(result.method, HTTP_METHOD_POST) == 0)
        {
            result.body_size = read_bytes - i + 1;
            result.body = malloc(result.body_size * sizeof(char));
            i += 2;                                     // Pass the header and body divider
        }
        else
        {
            result.body_size = 0;
        }

        for (j = 0; j < result.body_size; ++j)
        {
            result.body[j] = buffer[i++];
        }
        if (result.body_size != 0)
        {
            result.body[j] = '\0';
        }

    }
    while (read_bytes >= REQUEST_BUFFER_SIZE);

    return result;
}
