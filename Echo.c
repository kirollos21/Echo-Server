#include "Echo.h"

void initialize_client_sockets(int client_sockets[]) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0; // Initialize all client sockets to 0 (empty)
    }
}

void start_server() {
    int server_socket, new_socket, addr_len, valread;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    int client_sockets[MAX_CLIENTS];
    int max_sd, sd, activity;

    initialize_client_sockets(client_sockets);

    // Create a TCP socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Bind to port %d successful\n", PORT);

    // Listen for incoming connections
    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Listening for connections...\n");

    addr_len = sizeof(address);

    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add the server socket to the set
        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

        // Add client sockets to the set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];

            // If valid socket descriptor, add to read list
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            // Track the highest file descriptor number
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Wait for an activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            printf("Select error\n");
        }

        // If something happened on the server socket, it's an incoming connection
        if (FD_ISSET(server_socket, &readfds)) {
            if ((new_socket = accept(server_socket, (struct sockaddr*)&address, (socklen_t*)&addr_len)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }
            printf("New connection, socket fd is %d, ip is: %s, port: %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            // Add new socket to array of client sockets
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        // Handle IO for each client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];

            if (FD_ISSET(sd, &readfds)) {
                // Check if it was for closing, and also read the incoming message
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0) {
                    // Somebody disconnected, get details and close the socket
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addr_len);
                    printf("Client disconnected, ip: %s, port: %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    // Echo back the message that came in
                    buffer[valread] = '\0';
                    printf("Received message: %s", buffer);
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }
}

int main() {
    start_server();
    return 0;
}
