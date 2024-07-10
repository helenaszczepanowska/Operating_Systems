#define _XOPEN_SOURCE 700

#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void SIGUSR1_handler(int signo) {
    printf("Signal received\n");
}

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Usage: %s <signal_pid> <signal_argument>\n", argv[0]);
        return -1;
    }
    
    printf("Sender PID: %d\n", getpid());
    signal(SIGUSR1, SIGUSR1_handler);

    int pid = atoi(argv[1]);
    int argument = atoi(argv[2]);

    union sigval value;
    value.sival_int = argument;

    sigqueue(pid, SIGUSR1, value);
    printf("Signal sent with argument: %d\n", argument);
    return 0;
}