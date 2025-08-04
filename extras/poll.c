//a simple chat-like server using poll()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("\nServer listening on port %d...\n", PORT);

    // Create pollfd array
    struct pollfd fds[MAX_CLIENTS + 1];  // +1 for the server socket
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;  // Monitor for incoming connections

    // Initialize client slots
    for (int i = 1; i <= MAX_CLIENTS; i++) {
        fds[i].fd = -1;  // -1 means unused
    }

    while (1) {
        int activity = poll(fds, MAX_CLIENTS + 1, -1);  // Wait indefinitely

        if (activity < 0) {
            perror("\npoll error\n");
            break;
        }

        // Check if there's a new connection on the server socket
        if (fds[0].revents & POLLIN) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0) {
                perror("\naccept failed\n");
                continue;
            }

            printf("\nNew connection from %s:%d\n",
                   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            // Add new socket to poll list
            int added = 0;
            for (int i = 1; i <= MAX_CLIENTS; i++) {
                if (fds[i].fd == -1) {
                    fds[i].fd = new_socket;
                    fds[i].events = POLLIN;
                    added = 1;
                    break;
                }
            }

            if (!added) {
                printf("\nToo many clients. Connection refused.\n");
                close(new_socket);
            }
        }

        // Check for data from clients
        for (int i = 1; i <= MAX_CLIENTS; i++) {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) {
                int bytes_read = read(fds[i].fd, buffer, BUFFER_SIZE);
                if (bytes_read <= 0) {
                    // Client disconnected
                    printf("\nClient on fd %d disconnected.\n", fds[i].fd);
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } else {
                    buffer[bytes_read] = '\0';
                    printf("\nReceived from client %d: %s", fds[i].fd, buffer);

                    // Echo back the message
                    send(fds[i].fd, buffer, bytes_read, 0);
                }
            }
        }
    }

    // Cleanup
    for (int i = 0; i <= MAX_CLIENTS; i++) {
        if (fds[i].fd != -1) {
            close(fds[i].fd);
        }
    }

    return 0;
}