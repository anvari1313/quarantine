//
// Created by ahmad on 3/21/20.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "response.h"
#include "header.h"

char *raw_response(response res)
{
    char *raw = NULL, *raw_header = NULL;

    raw_header = malloc((50 + res.header_size * sizeof(header)) * sizeof(char));

    sprintf(raw_header, "%s %s\r\n", res.protocol, res.status_code);

    for (int i = 0; i < res.header_size; ++i)
    {
        char header[100];
        sprintf(header, "%s: %s\r\n", res.headers[i].name, res.headers[i].value);
        strcat(raw_header, header);
    }

    free(res.headers);

    strcat(raw_header, "\r\n");

    size_t len = (strlen(raw_header) + res.body_size + 1) * sizeof(char);
    raw = malloc(len);
    if (raw == NULL) {
        fprintf(stderr, "Can not allocate memory here");
        exit(-1);
    }
//    printf("%d", strlen(raw_header));
    for (int i = 0; i < strlen(raw_header); ++i) {
        raw[i] = raw_header[i];
    }
//    strcpy(raw, raw_header);
    free(raw_header);

    if (res.body_size != 0)
    {
        strcat(raw, res.body);
        free(res.body);
    }

    return raw;
}

response generate_sample_response()
{
    response r;

    strcpy(r.protocol, HTTP_PROTOCOL_V11);
    strcpy(r.status_code, HTTP_STATUS_OK);

    r.body = malloc(1024 * sizeof(char));
    strcpy(r.body, "<html>\n"
                   "<head>\n"
                   "   <title>Simple form</title>\n"
                   "</head>\n"
                   "<body>\n"
                   "   <h1>Simple form</h1>\n"
                   "<form action=\"/action_page.php\" method=\"post\">\n"
                   "  <label for=\"fname\">First name:</label><br>\n"
                   "  <input type=\"text\" id=\"fname\" name=\"fname\" value=\"John\"><br>\n"
                   "  <label for=\"lname\">Last name:</label><br>\n"
                   "  <input type=\"text\" id=\"lname\" name=\"lname\" value=\"Doe\"><br><br>\n"
                   "  <input type=\"submit\" value=\"Submit\">\n"
                   "</form>"
                   "</body>\n"
                   "</html>");
    r.body_size = strlen(r.body);

    char content_len_value[10];
    sprintf(content_len_value, "%ld", r.body_size);
    header content_len = {HTTP_HEADER_CONTENT_LENGTH, ""};
    strcpy(content_len.value, content_len_value);

    header date = {HTTP_HEADER_DATE, "Sun, 18 Oct 2012 10:36:20 GMT"};
    header server = {HTTP_HEADER_SERVER, "Apache/2.2.14 (Win32)"};
    header connection = {HTTP_HEADER_CONNECTION, HTTP_HEADER_CONNECTION_CLOSED};
    header content_type = {HTTP_HEADER_CONTENT_TYPE, HTTP_HEADER_CONTENT_TYPE_TEXT_HTML};

    r.header_size = 5;
    r.headers = malloc(r.header_size * sizeof(header));
    r.headers[0] = date;
    r.headers[1] = server;
    r.headers[2] = content_len;
    r.headers[3] = connection;
    r.headers[4] = content_type;


    return r;
}

response generate_file_response(const char *path)
{
    response r;

    char file_path[50];
    strcpy(file_path, "/home/ahmad");
    strcat(file_path, path);

    FILE *f = fopen(file_path, "rb");
    if (f == NULL)
    {
        return generate_not_found_response();
    }

    fseek(f, 0, SEEK_END);          // Jump to the end of the file
    size_t len = ftell(f);             // Get the current byte offset in the file
    rewind(f);                      // Jump back to the beginning of the file

    char *buffer = (char *)malloc(len * sizeof(char)); // Enough memory for the file
    fread(buffer, len, 1, f); // Read in the entire file
    fclose(f); // Close the file

    r.body_size = len;
    r.body = malloc((r.body_size + 1) * sizeof(char));
//    strcpy(r.body, buffer);

    strcpy(r.protocol, HTTP_PROTOCOL_V11);
    strcpy(r.status_code, HTTP_STATUS_OK);

    char content_len_value[10];
    sprintf(content_len_value, "%ld", r.body_size);
    header content_len = {HTTP_HEADER_CONTENT_LENGTH, ""};
    strcpy(content_len.value, content_len_value);

    header date = {HTTP_HEADER_DATE, "Sun, 18 Oct 2012 10:36:20 GMT"};
    header server = {HTTP_HEADER_SERVER, "Apache/2.2.14 (Win32)"};
    header connection = {HTTP_HEADER_CONNECTION, HTTP_HEADER_CONNECTION_CLOSED};
//    header content_type = {HTTP_HEADER_CONTENT_TYPE, HTTP_HEADER_CONTENT_TYPE_TEXT_HTML};
    header content_type = {HTTP_HEADER_CONTENT_TYPE, HTTP_HEADER_CONTENT_TYPE_APP_JSON};

    r.header_size = 5;
    r.headers = malloc(r.header_size * sizeof(header));
    r.headers[0] = date;
    r.headers[1] = server;
    r.headers[2] = content_len;
    r.headers[3] = connection;
    r.headers[4] = content_type;

    return r;
}

response generate_not_found_response()
{
    response r;

    strcpy(r.protocol, HTTP_PROTOCOL_V11);
    strcpy(r.status_code, HTTP_STATUS_NOT_FOUND);

    r.body = malloc(1024 * sizeof(char));
    strcpy(r.body, "<html>"
                   "<head>"
                   "<title>Resource Not Found</title>\n"
                   "</head>"
                   "<body><h1>404 - Resource Not Found</h1>"
                   "The resource you are looking for is NOT found"
                   "</body>"
                   "</html>");
    r.body_size = strlen(r.body);

    char content_len_value[10];
    sprintf(content_len_value, "%ld", r.body_size);
    header content_len = {HTTP_HEADER_CONTENT_LENGTH, ""};
    strcpy(content_len.value, content_len_value);

    header date = {HTTP_HEADER_DATE, "Sun, 18 Oct 2012 10:36:20 GMT"};
    header server = {HTTP_HEADER_SERVER, HTTP_HEADER_QUARANTINE_SERVER};
    header connection = {HTTP_HEADER_CONNECTION, HTTP_HEADER_CONNECTION_CLOSED};
    header content_type = {HTTP_HEADER_CONTENT_TYPE, HTTP_HEADER_CONTENT_TYPE_TEXT_HTML};

    r.header_size = 5;
    r.headers = malloc(r.header_size * sizeof(header));
    r.headers[0] = date;
    r.headers[1] = server;
    r.headers[2] = content_len;
    r.headers[3] = connection;
    r.headers[4] = content_type;

    return r;
}
