/**
 * @file server.c
 * @brief Implementation of the server-side code.
 *
 * This file contains the implementation of the chat server, which listens for
 * incoming client connections, manages client interactions, and forwards messages.
 */
 
#include "server.h"

int client_sockets[MAX_CLIENTS];
int num_clients = 0;
pthread_t threads[MAX_CLIENTS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//______________________________________________________________________

int initializeServer() {
    // Create a socket for the server
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation error");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server to the port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        return -1;
    }

    // Listen for client connections
    if (listen(server_socket, 10) < 0) {
        perror("Listening error");
        return -1;
    }

    printf("Server listening on port %d...\n", PORT);
    return server_socket;
}

//______________________________________________________________________

void sendToAllClients(char *message, int sender) {
    pthread_mutex_lock(&mutex);
    char messageWithSender[MAX_MESSAGE_SIZE + 50];
    snprintf(messageWithSender, sizeof(messageWithSender), "Client%d: %s", sender, message);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && i != sender) {
            send(client_sockets[i], messageWithSender, strlen(messageWithSender), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

//______________________________________________________________________

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

        sendToAllClients(buffer, i);
    }
    pthread_exit(NULL);
}

//______________________________________________________________________

int main() {
    int server_socket = initializeServer();
    if (server_socket < 0) {
        exit(1);
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    while (1) {
        int new_socket = accept(server_socket, NULL, NULL);

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

//______________________________________________________________________
