#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_REINDEERS 9
#define DELIVERIES 4

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

int reindeer_count = 0;
int deliveries_left = 4;

void* santa_thread(void* arg) {
    while (deliveries_left > 0) {
        pthread_mutex_lock(&santa_mutex);
        while (reindeer_count < NUM_REINDEERS) {
            pthread_cond_wait(&santa_cond, &santa_mutex);
        }
        printf("Mikołaj: budzę się\n");
        printf("Mikołaj: dostarczam zabawki\n");
        sleep(rand() % 3 + 2);
        reindeer_count = 0;
        deliveries_left--;
        printf("Mikołaj: zasypiam\n");
        pthread_mutex_unlock(&santa_mutex);
    }
    pthread_exit(NULL);
}

void* reindeer_thread(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (deliveries_left > 0) {
        sleep(rand() % 6 + 5); // Renifer na wakacjach
        pthread_mutex_lock(&santa_mutex);
        reindeer_count++;
        printf("Renifer: czeka %d reniferów na Mikołaja, ID: %d\n", reindeer_count, id);
        if (reindeer_count == NUM_REINDEERS) {
            printf("Renifer: wybudzam Mikołaja, ID: %d\n", id);
            pthread_cond_signal(&santa_cond);
        }
        pthread_mutex_unlock(&santa_mutex);
        sleep(rand() % 3 + 2); // Dostarczanie zabawek 
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t santa;
    pthread_t reindeers[NUM_REINDEERS];

    pthread_create(&santa, NULL, santa_thread, NULL);

    for (int i = 0; i < NUM_REINDEERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&reindeers[i], NULL, reindeer_thread, id);
    }

    pthread_join(santa, NULL);

    for (int i = 0; i < NUM_REINDEERS; i++) {
        pthread_join(reindeers[i], NULL);
    }

    pthread_mutex_destroy(&santa_mutex);
    pthread_cond_destroy(&santa_cond);

    return 0;
}
