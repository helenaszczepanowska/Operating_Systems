#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

void sigusr1_handler(int sig) {
    printf("SIGUSR1 handled\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [none|ignore|handler|mask]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!strcmp(argv[1], "ignore")) {
        signal(SIGUSR1, SIG_IGN);
        printf("Ignoring SIGUSR1\n");
    
    } else if (!strcmp(argv[1], "handler")) {
        signal(SIGUSR1, sigusr1_handler);

    } else if (!strcmp(argv[1], "mask")) {
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);
        printf("SIGUSR1 masked\n");

    }
    else{
        signal(SIGUSR1, SIG_DFL);
    }

    raise(SIGUSR1);

    if (strcmp(argv[1], "mask") == 0) {
        sigset_t pending;
        sigpending(&pending);
        if (sigismember(&pending, SIGUSR1)) {
            printf("SIGUSR1 is pending\n");
        } else {
            printf("SIGUSR1 is not pending\n");
        }
    }
    
    return 0;
}