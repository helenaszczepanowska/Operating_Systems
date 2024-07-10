#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

double rectangle_integration(double start, double end, double width) {
    double sum = 0.0;
    for (double x = start; x < end; x += width) {
        sum += 4.0 / (x * x + 1.0)*width;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("3 arguments are required\n");
        exit(1);
    }

    double width = atof(argv[1]);
    int n = atoi(argv[2]);
    double interval = 1.0 / n;;

    int fd[n][2]; 

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL); 

    for (int i = 0; i < n; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) { 
            close(fd[i][0]); 

            double start = i * interval;
            double end = (i + 1) * interval;
            double result = rectangle_integration(start, end, width);

            write(fd[i][1], &result, sizeof(double));
            close(fd[i][1]); 
            exit(0);
        } else {
            close(fd[i][1]); 
        }
    }

    double sum = 0.0;
    double part_result;

    for (int i = 0; i < n; i++) {
        wait(NULL); 
        read(fd[i][0], &part_result, sizeof(double));
        sum += part_result;
        close(fd[i][0]); 
    }

    gettimeofday(&end_time, NULL); 
    double execution_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printf("Calculated integral: %.10f\n", sum);
    printf("Execution time: %.6f seconds\n", execution_time);


    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    fprintf(file, "Calculated integral: %.10f Width: %.10f Number of processes: %d Execution time: %.6f seconds\n",sum, width,  n, execution_time);
    fclose(file);

    return 0;
}
