#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define PIPE "/tmp/my_pipe"

int main() {
    double a, b;
    int n;

    int pipe_fd = open(PIPE, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Failed to open pipe");
        exit(1);
    }
    else{
        printf("Pipe opened\n");
    }

    printf("Enter the integration limits (a, b): ");
    scanf("%lf %lf", &a, &b);

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    write(pipe_fd, &a, sizeof(double));
    write(pipe_fd, &b, sizeof(double));
    write(pipe_fd, &n, sizeof(int));
    close(pipe_fd);

    pipe_fd = open(PIPE, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Failed to open pipe");
        exit(1);
    }

    double result;
    read(pipe_fd, &result, sizeof(double));
    close(pipe_fd);
    printf("Calculated integral: %.10f\n", result);

    unlink(PIPE);

    return 0;
}
