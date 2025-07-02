#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Define the sockaddr_in structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear memory

    //Fill in the values
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(5344); // Port number (converted to network byte order)
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket to the IP address and port
    int result = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    if (result == -1) {
        perror("Bind failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Socket bound to 127.0.0.1:5344 successfully!\n");

    close(sockfd);
    return EXIT_SUCCESS;
}
