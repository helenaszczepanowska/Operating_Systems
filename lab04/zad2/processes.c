#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int global = 0;

int main(int argc, char** argv) {
     
    if(argc < 2) {
        printf("Not enough arguments");
        return 1;
    }

    printf("%s \n", argv[0]);
    int local = 0;

    pid_t pid = fork();

    if (pid == 0) {
       
        printf("child process\n");
        global++;
        local++;

        printf("child pid = %d, parent pid = %d \n", getpid(), getppid());
        printf("child local = %d, child global = %d \n", local, global);

        int status = execl("/bin/ls", "ls", "-l", argv[1], NULL);
        exit(status);
    }
    else{
        int status = 0;
        wait(&status);
        int child_return_status = WEXITSTATUS(status);

        printf("parent process\n");
        printf("parent pid = %d, child pid = %d \n", getpid(), pid);
        printf("Child exit code: %d \n", child_return_status);
        printf("parent local = %d, parent global = %d \n", local, global);
    }
    return 0;
}