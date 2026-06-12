#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <signal.h>

#include "core_network.h"
#include "response_builder.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf(
            "Usage: %s <port>\n",
            argv[0]
        );
        return 1;
    }

    int port =
        atoi(argv[1]);

    signal(
        SIGCHLD,
        sigchld_handler
    );

    int server_fd =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

    if(server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    int opt = 1;

    setsockopt(
        server_fd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );

    struct sockaddr_in server_addr;

    memset(
        &server_addr,
        0,
        sizeof(server_addr)
    );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(
        server_fd,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr)
    ) < 0)
    {
        perror("bind");
        return 1;
    }

    if(listen(server_fd, 10) < 0)
    {
        perror("listen");
        return 1;
    }

    printf(
        "Mini HTTP Server Running on Port %d\n",
        port
    );

    while(1)
    {
        int client_fd =
            accept(
                server_fd,
                NULL,
                NULL
            );

        if(client_fd < 0)
        {
            continue;
        }

        pid_t pid = fork();

        if(pid == 0)
        {
            close(server_fd);

            handle_client(
                client_fd
            );

            close(client_fd);

            exit(0);
        }

        close(client_fd);
    }

    close(server_fd);

    return 0;
}
