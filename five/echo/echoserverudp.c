#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>  // For inet functions and sockaddr_in

#define PORT 8080          // Server will listen on this port
#define BUFFER_SIZE 1024   // Size of buffer to hold messages

int main() {
    int sockfd;                          // Socket file descriptor
    char buffer[BUFFER_SIZE];           // Message buffer
    struct sockaddr_in servaddr, cliaddr; // Server and client address structures
    socklen_t len;

    // Create socket of type UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed"); // Print error if socket fails
        exit(EXIT_FAILURE);
    }

    // Clear the server and client address structures
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Assign IP and PORT to server address
    servaddr.sin_family = AF_INET;             // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;     // Any incoming IP address
    servaddr.sin_port = htons(PORT);           // Convert port to network byte order

    // Bind the socket with server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");   // Error if binding fails
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo Server is running on port %d...\n", PORT);

    // Server runs in infinite loop, echoing messages back
    while (1) {
        len = sizeof(cliaddr); // Length of client address
        // Receive message from client
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0'; // Null-terminate the received message
        printf("Received: %s\n", buffer);

        // Send same message back to client
        sendto(sockfd, buffer, n, 0, (const struct sockaddr *)&cliaddr, len);
    }

    // Close the socket (not reached in this infinite loop)
    close(sockfd);
    return 0;
}