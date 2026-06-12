#include "http_parser.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "response_builder.h"

void send_400(int client_fd)
{
    send_error_file(
        client_fd,
        400,
        "Bad Request",
        "www/400.html"
    );
}

void send_405(int client_fd)
{
    send_error_file(
        client_fd,
        405,
        "Method Not Allowed",
        "www/405.html"
    );
}

int parse_request(
    const char *buffer,
    char *method,
    char *path,
    char *version
)
{
    return sscanf(
        buffer,
        "%15s %255s %31s",
        method,
        path,
        version
    );
}
