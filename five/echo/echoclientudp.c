#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>  // For sockaddr_in and inet functions

#define PORT 8080          // Server port to connect to
#define BUFFER_SIZE 1024   // Size of buffer for messages

int main() {
    int sockfd;                          // Socket file descriptor
    char buffer[BUFFER_SIZE];           // Message buffer
    struct sockaddr_in servaddr;        // Server address structure

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");  // If socket fails
        exit(EXIT_FAILURE);
    }

    // Clear server address structure
    memset(&servaddr, 0, sizeof(servaddr));

    // Assign server details
    servaddr.sin_family = AF_INET;                // IPv4
    servaddr.sin_port = htons(PORT);              // Port
    servaddr.sin_addr.s_addr = INADDR_ANY;        // Localhost (use server IP if remote)

    printf("💬 UDP Echo Client started. Type messages to send...\n");

    while (1) {
        // Prompt user for input
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);         // Read input from user
        buffer[strcspn(buffer, "\n")] = 0;         // Remove newline character

        // Send message to server
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

        // Receive echoed message from server
        int n;
        socklen_t len = sizeof(servaddr);
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0'; // Null-terminate echoed message

        // Display echoed message
        printf("🔁 Echoed back: %s\n", buffer);
    }

    // Close socket (never reached in this loop)
    close(sockfd);
    return 0;
}
