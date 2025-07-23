#include <stdio.h>      // Standard I/O functions
#include <stdlib.h>     // Standard library: exit(), EXIT_FAILURE
#include <string.h>     // String handling: memset(), strlen()
#include <unistd.h>     // Unix standard functions: close(), read(), write()
#include <arpa/inet.h>  // Definitions for internet operations: inet_pton(), sockaddr_in

#define SERVER_IP "127.0.0.1"     // IP address of the server (localhost)
#define SERVER_PORT 5344         // Port to connect to
#define BUFFER_SIZE 1024         // Size of the buffer for messages

int main() {
    int sockfd;                          // File descriptor for the socket
    struct sockaddr_in server_addr;     // Struct to hold server address info
    char buffer[BUFFER_SIZE];           // Buffer to hold user input and received messages

    // Create socket: IPv4, TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {                 // Check for socket creation failure
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Clear memory and set up server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                // Use IPv4
    server_addr.sin_port = htons(SERVER_PORT);       // Convert port to network byte order
    // if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {  // Convert IP address from text to binary
    //     perror("Invalid IP address");
    //     close(sockfd);
    //     exit(EXIT_FAILURE);
    // }
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server at %s:%d\n", SERVER_IP, SERVER_PORT);

    // Communicate with the server in a loop
    while (1) {
        printf("Enter message: ");                         // Prompt user
        if (!fgets(buffer, sizeof(buffer), stdin))         // Read user input
            break;

        if (send(sockfd, buffer, strlen(buffer), 0) == -1) {  // Send message to server
            perror("Send failed");
            break;
        }

        // Receive echoed response from server
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {                         // Check if connection closed or error occurred
            perror("Receive failed or connection closed");
            break;
        }

        buffer[bytes_received] = '\0';                     // Null-terminate the received string
        printf("Server echoed: %s", buffer);               // Display echoed message
    }

    // Clean up
    close(sockfd);                                         // Close socket
    printf("Disconnected from the server.\n");
    return 0;                                              // Exit successfully
}
