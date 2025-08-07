#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/echo_socket"  // Path for the Unix domain socket
#define BUFFER_SIZE 1024                // Buffer size for reading data

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // Create a Unix domain socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Remove any existing socket file to avoid bind errors
    unlink(SOCKET_PATH);

    // Set up the socket address structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Bind the socket to the specified path
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming client connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on %s\n", SOCKET_PATH);

    // Main loop to accept and handle client connections
    while (1) {
        // Accept a client connection
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        printf("Client connected.\n");

        // Echo loop: read from client and send back the same data
        while (1) {
            ssize_t bytes = read(client_fd, buffer, BUFFER_SIZE);
            if (bytes <= 0) {
                printf("Client disconnected.\n");
                break;
            }

            buffer[bytes] = '\0';  // Null-terminate the received string
            printf("Received: %s", buffer);

            // Send the same data back to the client
            write(client_fd, buffer, bytes);
        }

        // Close the client socket
        close(client_fd);
    }

    // Clean up: close server socket and remove socket file
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
