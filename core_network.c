#include "core_network.h"

#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void sigchld_handler(int sig)
{
    (void)sig;

    while(waitpid(-1, NULL, WNOHANG) > 0);
}
