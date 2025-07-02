#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int sockfd;
    struct sockaddr_in6 server_addr;

    // Create IPv6 socket
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize socket address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(8085); // Example port
    server_addr.sin6_addr = in6addr_any; // Accept connections on any local IPv6 address

    // Bind the socket to the IPv6 address and port
    int result = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (result == -1) {
        perror("Bind failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Socket bound to 5344 successfully!\n");

    // Close socket
    close(sockfd);
    return 0;
}
