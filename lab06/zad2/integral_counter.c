#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define PIPE "/tmp/my_pipe"

double rectangle_integration(double start, double end, double width) {
    double sum = 0.0;
    for (double x = start; x < end; x += width) {
        sum += 4.0 / (x * x + 1.0) * width;
    }
    return sum;
}

int main() {

    struct stat st;
    if (stat(PIPE, &st) == 0) {
        unlink(PIPE);
    }

    if (mkfifo(PIPE, 0666) != 0) {
        perror("Failed creating pipe");
        exit(1);
    }

    int pipe_fd = open(PIPE, O_RDONLY);
    if (pipe_fd == -1) {
        perror("Failed to open pipe");
        exit(1);
    }
    
    double a, b;
    int n;
    read(pipe_fd, &a, sizeof(double));
    read(pipe_fd, &b, sizeof(double));
    read(pipe_fd, &n, sizeof(int));
    printf("Recived data, counting...\n");
    close(pipe_fd);

    double width = (b - a) / n;
    double result = rectangle_integration(a, b, width);

    pipe_fd = open(PIPE, O_WRONLY);
    if (pipe_fd == -1) {
        perror("Failed to open pipe");
        exit(1);
    }

    write(pipe_fd, &result, sizeof(double));
    close(pipe_fd);

    unlink(PIPE);

    return 0;
}
