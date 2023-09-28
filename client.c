/**
 * @file client.c
 * @brief Implementation of the client-side code.
 *
 * This file contains the implementation of the client application that connects
 * to the chat server, sends and receives messages, and handles user input.
 */

#include "client.h"

//#define ADDRESS_OF_BBB "192.168.1.15"
#define ADDRESS_OF_BBB "127.0.0.1"

int client_socket;
char buffer[MAX_MESSAGE_SIZE];

//______________________________________________________________________

int initializeClient() {
    // Create a socket for the client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation error");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ADDRESS_OF_BBB); // Replace with the server's IP address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection error");
        return -1;
    }

    printf("Connected to the chat server. Type 'exit' to quit.\n");
    return 0;
}

//______________________________________________________________________

void *receiveMessages(void *arg) {
    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("%s", buffer);
        } else if (bytes_received == 0) {
            printf("Server has closed the connection.\n");
            break;
        } else {
            perror("Receive error");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
    }
    close(client_socket);
    exit(0);
}

//______________________________________________________________________

int main() {
    if (initializeClient() < 0) {
        exit(1);
    }

    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receiveMessages, NULL) != 0) {
        perror("Thread creation error");
        close(client_socket);
        exit(1);
    }

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "exit\n") == 0) {
            send(client_socket, buffer, strlen(buffer), 0);
            break;
        }

        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Send error");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
    }

    close(client_socket);
    return 0;
}

//______________________________________________________________________
