#ifndef RESPONSE_BUILDER_H
#define RESPONSE_BUILDER_H

void handle_client(int client_fd);

void send_404(int client_fd);

const char *get_mime_type(
    const char *path
);

void send_error_file(
    int client_fd,
    int status_code,
    const char *status_text,
    const char *filepath
);

#endif
