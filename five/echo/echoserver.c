#include <stdio.h>          // For printf(), perror()
#include <stdlib.h>         // For exit(), EXIT_FAILURE
#include <string.h>         // For memset()
#include <unistd.h>         // For close()
#include <arpa/inet.h>      // For sockaddr_in, htons(), inet_ntoa()
#include <sys/socket.h>     // For socket functions

#define PORT 5344           // The port number the server will listen on
#define BUFFER_SIZE 1024    // The size of the buffer for client messages

int main() {
    // Declare socket file descriptors and address structures
    int server_fd, client_fd;                               // Server and client socket descriptors
    struct sockaddr_in server_addr, client_addr;            // Address information for server and client
    socklen_t client_len = sizeof(client_addr);             // Length of the client address struct
    char buffer[BUFFER_SIZE];                               // Buffer to store data from client

    // Create a socket: IPv4 (AF_INET), TCP (SOCK_STREAM)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {                                  // Check if socket creation failed
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));           // Clear the memory
    
    //fill in the values
    server_addr.sin_family = AF_INET;                       // Use IPv4
    server_addr.sin_port = htons(PORT);                     // Convert port number to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;               // Accept connections from any local IP

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");                              // Display error if binding fails
        close(server_fd);                                   // Close the server socket
        exit(EXIT_FAILURE);                                 // Exit the program
    }

    // Put the server socket into listening mode
    if (listen(server_fd, 5) == -1) {                        // 5 is the backlog queue size
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Echo server is listening on port %d...\n", PORT); // Inform the user that server is ready

    // Server loop: accept and handle clients one at a time
    while (1) {
        // Accept an incoming client connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1) {                              // Check for failed accept()
            perror("Accept failed");
            continue;                                       // Skip to next iteration of loop
        }

        printf("Client connected\n");

        // Loop to receive and echo back messages from the client
        ssize_t bytes_received;
        while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
            buffer[bytes_received] = '\0';                  // Null-terminate the received string
            printf("Received from client: %s", buffer);     // Display the client's message
            send(client_fd, buffer, bytes_received, 0);     // Echo the message back to the client
        }

        printf("Client disconnected\n");                    // Notify when client disconnects
        close(client_fd);                                   // Close the client socket
    }

    close(server_fd);                                       // Close the server socket (never reached)
    return 0;                                               // Exit the program
}
