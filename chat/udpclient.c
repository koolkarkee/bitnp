#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080           // Port number to connect to
#define BUFFER_SIZE 1024    // Size of the message buffer

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    server_addr.sin_family = AF_INET;              // IPv4
    server_addr.sin_port = htons(PORT);            // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost

    printf("Connected to UDP server.\n");

    // Chat loop
    while (1) {
        // Get client's message
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr*)&server_addr, addr_len);

        // Receive server's reply
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0,
                                      (struct sockaddr*)&server_addr, &addr_len);
        if (bytes_received < 0) {
            perror("Receive failed");
            continue;
        }
        printf("Server: %s", buffer);
    }

    // Close socket
    close(sock);
    return 0;
}