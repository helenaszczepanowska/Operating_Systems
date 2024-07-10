#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct msgbuf {
    long mtype;
    int client_id;
    char mtext[200];
};

int client_queue_id;
void cleanup(int sig) {
    msgctl(client_queue_id, IPC_RMID, NULL);
    printf("Client queue removed successfully.\n queue_id: %d\n", client_queue_id);
    exit(1);
}

int main() {
    signal(SIGINT, cleanup); 
    signal(SIGHUP, cleanup);  

    key_t client_key = ftok("client", getpid());
    client_queue_id = msgget(client_key, 0666 | IPC_CREAT);

    if(client_queue_id == -1) {
        perror("msgget");
        exit(1);
    }else{
        printf("Client queue created\n id: %d\n", client_queue_id);
    }

    key_t server_key = ftok("server", 65);
    int server_queue_id = msgget(server_key, 0);

    struct msgbuf message;
    message.mtype = 1; // INIT
    sprintf(message.mtext, "%d", client_queue_id);
    msgsnd(server_queue_id, &message, sizeof(message.mtext), 0);

    msgrcv(client_queue_id, &message, sizeof(message.mtext), 0, 0);
    int client_id = atoi(message.mtext);
    printf("Client id: %d\n", client_id);

    if (fork() == 0) {
        while (1) {
            msgrcv(client_queue_id, &message, sizeof(message.mtext), 0, 0);
            printf("Received from client %d: %s\n", message.client_id, message.mtext);
        }
    } else {
        while (1) {
            fgets(message.mtext, sizeof(message.mtext), stdin);
            message.mtype = client_id + 2;
            msgsnd(server_queue_id, &message, sizeof(message.mtext), 0);
        }
    }
    msgctl(client_queue_id, IPC_RMID, NULL);
    return 0;
}
