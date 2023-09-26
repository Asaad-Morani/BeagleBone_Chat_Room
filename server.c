#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8081
#define MAX_CLIENTS 5
#define MAX_MESSAGE_SIZE 1024

int client_sockets[MAX_CLIENTS];
int num_clients = 0;
pthread_t threads[MAX_CLIENTS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void sendToAllClients(char *message, int sender) {
    pthread_mutex_lock(&mutex);
    char messageWithSender[MAX_MESSAGE_SIZE + 50]; // we have to make sure the buffer is large enough
    snprintf(messageWithSender, sizeof(messageWithSender), "Client%d: %s", sender, message);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && i != sender) {
            send(client_sockets[i], messageWithSender, strlen(messageWithSender), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *handleClient(void *arg) {
    int client_socket = *((int *)arg);
    int i;
    char buffer[MAX_MESSAGE_SIZE];

    for (i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == 0) {
            client_sockets[i] = client_socket;
            num_clients++;
            break;
        }
    }

    // Handle chat
    char welcome_message[50];
    sprintf(welcome_message, "Welcome to the chat, Client %d!\n", i);
    send(client_socket, welcome_message, strlen(welcome_message), 0);

    while (1) {
        int bytes_received = recv(client_socket, buffer, MAX_MESSAGE_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Client %d has disconnected.\n", i);
            close(client_socket);
            client_sockets[i] = 0;
            num_clients--;
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Received from Client %d: %s", i, buffer);

        // Send the message to all other clients
        sendToAllClients(buffer, i);
    }
    pthread_exit(NULL);
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        exit(1);
    }

    listen(server_socket, 10);
    printf("Server listening on port %d...\n", PORT);

    addr_size = sizeof(new_addr);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    while (1) {
        new_socket = accept(server_socket, (struct sockaddr *)&new_addr, &addr_size);

        // Handle max clients
        if (num_clients >= MAX_CLIENTS) {
            char *message = "Chat Server is full. Try again later.\n";
            send(new_socket, message, strlen(message), 0);
            close(new_socket);
        } else {
            pthread_create(&threads[num_clients], NULL, handleClient, &new_socket);
        }
    }
    return 0;
}
