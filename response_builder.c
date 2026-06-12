#include "response_builder.h"
#include "http_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

void send_error_file(
    int client_fd,
    int status_code,
    const char *status_text,
    const char *filepath
)
{
    FILE *fp = fopen(filepath, "rb");

    if(fp == NULL)
        return;

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    char *content = malloc(filesize);

    fread(
        content,
        1,
        filesize,
        fp
    );

    fclose(fp);

    char header[512];

    sprintf(
        header,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        status_code,
        status_text,
        filesize
    );

    send(
        client_fd,
        header,
        strlen(header),
        0
    );

    send(
        client_fd,
        content,
        filesize,
        0
    );

    free(content);
}

void send_404(int client_fd)
{
    send_error_file(
        client_fd,
        404,
        "Not Found",
        "www/404.html"
    );
}

const char *get_mime_type(const char *path)
{
    const char *ext = strrchr(path, '.');

    if(ext == NULL)
        return "application/octet-stream";

    if(strcmp(ext, ".html") == 0)
        return "text/html";

    if(strcmp(ext, ".css") == 0)
        return "text/css";

    if(strcmp(ext, ".js") == 0)
        return "application/javascript";

    if(strcmp(ext, ".png") == 0)
        return "image/png";

    if(strcmp(ext, ".jpg") == 0)
        return "image/jpeg";

    if(strcmp(ext, ".jpeg") == 0)
        return "image/jpeg";

    if(strcmp(ext, ".gif") == 0)
        return "image/gif";

    return "application/octet-stream";
}

void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE];

    int bytes_received =
        recv(
            client_fd,
            buffer,
            sizeof(buffer)-1,
            0
        );

    if(bytes_received <= 0)
    {
        return;
    }

    buffer[bytes_received] = '\0';

    printf("\n===== REQUEST DITERIMA =====\n");
    printf("%s\n", buffer);

    char method[16];
    char path[256];
    char version[32];

    int result =
        parse_request(
            buffer,
            method,
            path,
            version
        );

    printf("\n===== HASIL PARSING =====\n");
    printf("Method  : %s\n", method);
    printf("Path    : %s\n", path);
    printf("Version : %s\n", version);

    if(result != 3)
    {
        send_400(client_fd);
        return;
    }

    printf("Method  : %s\n", method);
    printf("Path    : %s\n", path);
    printf("Version : %s\n", version);

    if(
        strcmp(version, "HTTP/1.1") != 0 &&
        strcmp(version, "HTTP/1.0") != 0
    )
    {
        printf("HTTP Version tidak valid\n");

        send_400(client_fd);

        return;
    }

    if(strcmp(method, "GET") != 0)
    {
        send_405(client_fd);
        return;
    }

    if(strcmp(path, "/test400") == 0)
    {
        printf("TEST 400 DIPANGGIL\n");

        send_400(client_fd);

        return;
    }

    char filepath[512];

    if(strcmp(path, "/") == 0)
    {
        strcpy(
            filepath,
            "www/index.html"
        );
    }
    else
    {
        sprintf(
            filepath,
            "www%s",
            path
        );
    }

    struct stat file_stat;

    if(stat(filepath, &file_stat) < 0)
    {
        send_404(client_fd);
        return;
    }

    FILE *fp =
        fopen(filepath, "rb");

    if(fp == NULL)
    {
        send_404(client_fd);
        return;
    }

    long filesize =
        file_stat.st_size;

    char *content =
        malloc(filesize);

    fread(
        content,
        1,
        filesize,
        fp
    );

    fclose(fp);

    const char *mime_type =
        get_mime_type(filepath);

    char header[512];

    sprintf(
        header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        mime_type,
        filesize
    );

    send(
        client_fd,
        header,
        strlen(header),
        0
    );

    send(
        client_fd,
        content,
        filesize,
        0
    );

    free(content);
}
