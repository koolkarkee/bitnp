#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/echo_socket"  // Path to connect to the server
#define BUFFER_SIZE 1024                // Buffer size for sending/receiving

int main() {
    int sock;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // Create a Unix domain socket
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the socket address structure
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Connect to the server socket
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type messages (type 'exit' to quit):\n");

    // Loop to send messages and receive echoes
    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);  // Read user input

        // Exit condition
        if (strncmp(buffer, "exit", 4) == 0)
            break;

        // Send message to server
        write(sock, buffer, strlen(buffer));

        // Receive echoed message from server
        ssize_t bytes = read(sock, buffer, BUFFER_SIZE - 1);
        if (bytes <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes] = '\0';  // Null-terminate the received string
        printf("Echo: %s", buffer);
    }

    // Clean up: close the socket
    close(sock);
    return 0;
}