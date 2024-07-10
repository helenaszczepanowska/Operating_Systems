#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

volatile int status = -1;
volatile int changes = 0;

void SIGUSR1_handler(int signo, siginfo_t *info, void *extra){
   
    int argument = info->si_int;
    int sender_pid = info->si_pid;
    printf("Received status: %d from pid: %d\n", argument, sender_pid);
    changes++;
    status = argument;
    kill(sender_pid, SIGUSR1);
}

int main() {
    printf("Catcher PID: %d\n", getpid());
    struct sigaction action;
    action.sa_sigaction = SIGUSR1_handler;
    action.sa_flags = SA_SIGINFO; 
    sigemptyset(&action.sa_mask);          

    sigaction(SIGUSR1, &action, NULL);  

    while(1) {
        switch(status){
            case 1:
                for(int i = 1; i <= 100; i++){
                    printf("%d, ", i);
                }
                printf("\n");
                status = -1;
                break;
            case 2:
                printf("Status has changed %d times\n", changes);
                status = -1;
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
        }
    } 

    return 0;
}