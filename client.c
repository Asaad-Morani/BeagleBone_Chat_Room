#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8081
#define MAX_MESSAGE_SIZE 1024

int client_socket;
char buffer[MAX_MESSAGE_SIZE];

void *receiveMessages(void *arg) {
    while (1) {
        // Receive and display messages from the server
        if (recv(client_socket, buffer, sizeof(buffer), 0) > 0) {
            //printf("Server: %s", buffer);
            printf("%s", buffer);
        } else {
            perror("Receive error");
            exit(1);
        }

        memset(buffer, 0, sizeof(buffer));
    }
}

int main() {
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.15");
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection error");
        exit(1);
    }

    printf("Connected to the chat server. Type 'exit' to quit.\n");

    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receiveMessages, NULL) != 0) {
        perror("Thread creation error");
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
            exit(1);
        }

        memset(buffer, 0, sizeof(buffer));
    }

    close(client_socket);
    return 0;
}
