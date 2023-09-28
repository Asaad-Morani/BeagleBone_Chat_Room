
/**
 * @file server.h
 * @brief Header file for the server-side code.
 *
 * This file contains declarations for functions and variables used on the server side.
 */

#ifndef SERVER_H
#define SERVER_H

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
/**
 * @brief Maximum number of clients the server can handle.
 */
#define MAX_CLIENTS 5
#define MAX_MESSAGE_SIZE 1024

/**
 * @brief Array to store client sockets.
 *
 * This array stores the socket descriptors of connected clients.
 */
extern int client_sockets[MAX_CLIENTS];

/**
 * @brief Number of currently connected clients.
 */
extern int num_clients;

/**
 * @brief Array to store thread IDs for client handling threads.
 *
 * This array stores the thread IDs of threads created to handle client interactions.
 */
extern pthread_t threads[MAX_CLIENTS];

/**
 * @brief Mutex used for thread synchronization.
 *
 * This mutex is used to ensure thread safety when accessing shared resources.
 */
extern pthread_mutex_t mutex;


/**
 * @brief Send a message to all clients except the sender.
 *
 * @param message The message to send.
 * @param sender The ID of the sender.
 */
void sendToAllClients(char *message, int sender);

/**
 * @brief Handle communication with a single client.
 *
 * @param arg A pointer to the client socket descriptor.
 * @return NULL.
 */
void *handleClient(void *arg);

/**
 * @brief Initialize the server.
 *
 * This function initializes the server by creating a socket, binding to a port, and listening for connections.
 *
 * @return 0 on success, -1 on error.
 */
int initializeServer();


#endif
