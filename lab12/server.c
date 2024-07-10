#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define NAME_LEN 32

typedef struct {
    struct sockaddr_in addr;
    socklen_t addr_len;
    char name[NAME_LEN];
    int active;
} client_t;

client_t clients[MAX_CLIENTS];
int client_count = 0;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_message_to_all(int sockfd, char *message, char *sender);
void send_message_to_one(int sockfd, char *message, char *sender, char *recipient);
void list_clients(int sockfd, struct sockaddr_in *addr, socklen_t addr_len);
void add_client(struct sockaddr_in *addr, socklen_t addr_len, char *name);
void remove_client(struct sockaddr_in *addr);
int find_client_by_address(struct sockaddr_in *addr);
void check_alive_clients(int sockfd);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %s\n", argv[1]);

    while (1) {
        int recive = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recive > 0) {
            buffer[recive] = '\0';
            char *command = strtok(buffer, " ");
            char *content = strtok(NULL, "");
            printf("Received [%s]: %s\n", command, content ? content : "no content");

            if (strcmp(command, "HELLO") == 0) {
                add_client(&client_addr, addr_len, content);
            } else if (strcmp(command, "2ALL") == 0) {
                char *name = clients[find_client_by_address(&client_addr)].name;
                send_message_to_all(server_fd, content, name);
            } else if (strcmp(command, "2ONE") == 0) {
                char *recipient = strtok(content, " ");
                char *message = strtok(NULL, "");
                char *name = clients[find_client_by_address(&client_addr)].name;
                send_message_to_one(server_fd, message, name, recipient);
            } else if (strcmp(command, "LIST") == 0) {
                list_clients(server_fd, &client_addr, addr_len);
            } else if (strcmp(command, "STOP") == 0) {
                remove_client(&client_addr);
            }
            else if (strncmp(buffer, "ALIVE", 5) == 0) {
                check_alive_clients(server_fd);
            }
        }
    }

    close(server_fd);
    return 0;
}

void send_message_to_all(int sockfd, char *message, char *sender) {
    char buffer[BUFFER_SIZE];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    snprintf(buffer, BUFFER_SIZE, "[%02d:%02d:%02d] %s: %s\n",
             t->tm_hour, t->tm_min, t->tm_sec, sender, message);

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].active && strcmp(clients[i].name, sender) != 0) {
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&clients[i].addr, clients[i].addr_len);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}


void send_message_to_one(int sockfd, char *message, char *sender, char *recipient) {
    char buffer[BUFFER_SIZE];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    snprintf(buffer, BUFFER_SIZE, "[%02d:%02d:%02d] %s: %s\n",
             t->tm_hour, t->tm_min, t->tm_sec, sender, message);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].name, recipient) == 0) {
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&clients[i].addr, clients[i].addr_len);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients(int sockfd, struct sockaddr_in *addr, socklen_t addr_len) {
    char buffer[BUFFER_SIZE] = "Active clients:\n";

    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            strcat(buffer, clients[i].name);
            strcat(buffer, "\n");
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)addr, addr_len);
}

void add_client(struct sockaddr_in *addr, socklen_t addr_len, char *name) {
    pthread_mutex_lock(&clients_mutex);
    if (client_count < MAX_CLIENTS) {
        clients[client_count].addr = *addr;
        clients[client_count].addr_len = addr_len;
        strncpy(clients[client_count].name, name, NAME_LEN);
        clients[client_count].active = 1;
        client_count++;
        printf("%s has joined\n", name);
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(struct sockaddr_in *addr) {
    pthread_mutex_lock(&clients_mutex);
    int idx = find_client_by_address(addr);
    if (idx != -1) {
        clients[idx].active = 0;
        printf("%s has left\n", clients[idx].name);
    }
    pthread_mutex_unlock(&clients_mutex);
}

int find_client_by_address(struct sockaddr_in *addr) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].active && memcmp(&clients[i].addr, addr, sizeof(struct sockaddr_in)) == 0) {
            return i;
        }
    }
    return -1;
}

void check_alive_clients(int sockfd) {
    char *message = "ALIVE";
    while (1) {
        sleep(10); 

        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < client_count; i++) {
            if (clients[i].active) {
                if (sendto(sockfd, message, strlen(message), 0,
                           (struct sockaddr *)&clients[i].addr, clients[i].addr_len) <= 0) {
                    clients[i].active = 0;
                    printf("%s has been disconnected\n", clients[i].name);
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
}
