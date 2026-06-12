#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

void send_400(int client_fd);
void send_405(int client_fd);

int parse_request(
    const char *buffer,
    char *method,
    char *path,
    char *version
);

void send_error_file(
    int client_fd,
    int status_code,
    const char *status_text,
    const char *filepath
);

#endif
