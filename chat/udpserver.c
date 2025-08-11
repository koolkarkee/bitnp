#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080           // Port number for the server
#define BUFFER_SIZE 1024    // Size of the message buffer

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Create a UDP socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    server_addr.sin_family = AF_INET;              // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;      // Accept connections from any IP
    server_addr.sin_port = htons(PORT);            // Convert port to network byte order

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    // Chat loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        int bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr*)&client_addr, &addr_len);
        if (bytes_received < 0) {
            perror("Receive failed");
            continue;
        }
        printf("Client: %s", buffer);

        // Get server's reply
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send reply to client
        sendto(server_fd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&client_addr, addr_len);
    }

    // Close socket
    close(server_fd);
    return 0;
}