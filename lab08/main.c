#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define QUEUE_SIZE 10
#define TEXT_SIZE 11 


typedef struct {
    char text[QUEUE_SIZE][TEXT_SIZE];
    int user[QUEUE_SIZE];
    int in;
    int out;
} PrintQueue;


void operation(int sem_id, int sem_num, int sem_op) {
    struct sembuf op;
    op.sem_num = sem_num;
    op.sem_op = sem_op;
    op.sem_flg = 0;
    semop(sem_id, &op, 1);
}

void user(int sem_id, PrintQueue *queue, int user_id) {
    srand(time(NULL) ^ getpid());
    while (1) {

        char text[TEXT_SIZE];
        for (int i = 0; i < TEXT_SIZE - 1; i++) {
            text[i] = 'a' + rand() % 26;
        }
        text[TEXT_SIZE - 1] = '\0';

        operation(sem_id, 0, -1);
        strcpy(queue->text[queue->in], text);
        queue->user[queue->in] = user_id;
        queue->in = (queue->in + 1) % QUEUE_SIZE;
        operation(sem_id, 1, 1);

        sleep(rand() % 5 + 1);
    }
}

void printer(int sem_id, PrintQueue *queue, int printer_id) {
    while (1) {
        operation(sem_id, 1, -1);
        char text[TEXT_SIZE];
        strcpy(text, queue->text[queue->out]);
        int user_id = queue->user[queue->out];
        queue->out = (queue->out + 1) % QUEUE_SIZE;
        operation(sem_id, 0, 1);

        for (int i = 0; i < strlen(text); i++) {
            printf("Printer: %d User: %d %c\n", printer_id, user_id, text[i]);
            fflush(stdout);
            sleep(1);
        }
        printf("Printer: %d done printing for user %d\n", printer_id, user_id);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <number of users> <number of printers>\n", argv[0]);
        return 1;
    }

    int num_users = atoi(argv[1]);
    int num_printers = atoi(argv[2]);

    int sem_id = semget(SEM_KEY, 2, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, QUEUE_SIZE); // semafor na miejsca w kolejce
    semctl(sem_id, 1, SETVAL, 0); // semafor na dostępne zadania

    int shm_id = shmget(IPC_PRIVATE, sizeof(PrintQueue), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }

    PrintQueue *queue = (PrintQueue *)shmat(shm_id, NULL, 0);
    if (queue == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }
    queue->in = 0;
    queue->out = 0;


    for (int i = 0; i < num_users; i++) {
        if (fork() == 0) {
            user(sem_id, queue, i);
            return 0;
        }
    }

    for (int i = 0; i < num_printers; i++) {
        if (fork() == 0) {
            printer(sem_id, queue, i);
            return 0;
        }
    }

    for (;;) {
        pause();
    }

    return 0;
}
