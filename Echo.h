#ifndef ECHO_H
#define ECHO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 2000

void initialize_client_sockets(int client_sockets[]);
void start_server();
void handle_client_connections(int server_socket);

#endif // ECHO_H
