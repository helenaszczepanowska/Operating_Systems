#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char** argv) {
    
    if(argc != 2) {
        printf("Not enough arguments");
        return 1;
    }

    int n = atoi(argv[1]);
    
    for(int i = 0; i < n; i++) {
        pid_t pid = fork();

        if(pid == 0) {
            printf("Child process pid %d\n", getpid());
            printf("Parent process pid %d\n", getppid());
            exit(0);
        }
    }
    
    while(wait(NULL) > 0);
    printf("Number of created processes: %d\n", n);
    
    return 0;
}