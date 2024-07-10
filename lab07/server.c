#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 10

struct msgbuf {
    long mtype;
    int client_id;
    char mtext[200];
};

struct client {
    int id;
    int queue_id;
};

int server_queue_id;
void cleanup(int sig) {
    msgctl(server_queue_id, IPC_RMID, NULL);
    printf("Server queue removed successfully.\n queue_id: %d\n", server_queue_id);
    exit(0);
}

int main() {
    signal(SIGINT, cleanup); 
    signal(SIGHUP, cleanup);  

    key_t server_key = ftok("server", 65);
    server_queue_id = msgget(server_key, 0666 | IPC_CREAT);
    
    if(server_queue_id == -1) {
        perror("msgget");
        exit(1);
    }else{
        printf("Server queue created\n id: %d\n", server_queue_id);
        printf("Waiting for clients...\n");
    }
    struct msgbuf message;
    struct client clients[MAX_CLIENTS];
    int num_clients = 0;

    while (1) {
        msgrcv(server_queue_id, &message, sizeof(message.mtext), 0, 0);
        if (message.mtype == 1) { // INIT
            int client_queue_id = atoi(message.mtext);

            clients[num_clients].id = num_clients;
            clients[num_clients].queue_id = client_queue_id;
            sprintf(message.mtext, "%d", num_clients);
            msgsnd(client_queue_id, &message, sizeof(message.mtext), 0);
            printf("Client %d connected\n", num_clients);
            num_clients++;
        } else {
            int client_id = message.mtype - 2;
            message.client_id = client_id;
            for (int i = 0; i < num_clients; i++) {
                if (i != client_id) {
                    msgsnd(clients[i].queue_id, &message, sizeof(message.mtext), 0);
                }
            }
        }
    }
    msgctl(server_queue_id, IPC_RMID, NULL);
    return 0;
}
