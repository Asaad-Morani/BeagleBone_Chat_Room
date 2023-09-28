/**
 * @file client.h
 * @brief Header file for the client-side code.
 *
 * This file contains declarations for functions and variables used on the client side.
 */

#ifndef CLIENT_H
#define CLIENT_H

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

/**
 * @brief The client socket descriptor.
 *
 * This variable stores the client's socket descriptor.
 */
extern int client_socket;

/**
 * @brief The buffer for storing messages.
 *
 * This buffer is used to store messages sent and received by the client.
 */
extern char buffer[MAX_MESSAGE_SIZE];

/**
 * @brief Receive messages from the server.
 *
 * This function is responsible for continuously receiving and displaying messages from the server.
 *
 * @param arg The argument (not used in this function).
 * @return NULL.
 */
void *receiveMessages(void *arg);

/**
 * @brief Initialize the client.
 *
 * This function initializes the client by creating a socket and connecting to the server.
 *
 * @return 0 on success, -1 on error.
 */
int initializeClient();

#endif
