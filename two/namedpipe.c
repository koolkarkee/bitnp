#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"
#define MESSAGE "Hello from child process!"

int main() {
    int fd;
    pid_t pid;

    // Create the named pipe (FIFO)
    mkfifo(FIFO_NAME, 0666);

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process
        fd = open(FIFO_NAME, O_WRONLY);
        write(fd, MESSAGE, sizeof(MESSAGE));
        close(fd);
        printf("Child: Message sent to parent.\n");
    } else { // Parent Process
        sleep(1); // Ensure child writes first
        fd = open(FIFO_NAME, O_RDONLY);
        char buffer[100];
        read(fd, buffer, sizeof(buffer));
        close(fd);
        printf("Parent: Received message - %s\n", buffer);

        // Cleanup
        unlink(FIFO_NAME);
    }

    return 0;
}