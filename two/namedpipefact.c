#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"
#define MESSAGE "Hello from child process!"

int fact(int n){
    if(n <= 1){
        return 1;
    }

    return n * fact(n-1);
}

int main() {
    int fd;
    pid_t pid;

    // Create the named pipe (FIFO)
    mkfifo(FIFO_NAME, 0666);

    // Fork the process
    pid = fork();

    //error
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    //child process
    if (pid == 0) {
        fd = open(FIFO_NAME, O_WRONLY);

        //enter the number
        int number;
        printf("\nEnter the number : \n");
        scanf("%d", &number);

        //caculate the factorial
        int result = fact(number);

        write(fd, &result, sizeof(result));
        close(fd);
        printf("Child: Message sent to parent.\n");
    } 
    
    //parent process
    else {
        sleep(1); // Ensure child writes first
        fd = open(FIFO_NAME, O_RDONLY);

        int receivedResult;
        read(fd, &receivedResult, sizeof(receivedResult));
        close(fd);

        printf("\nParent: The factorial is - %d\n", receivedResult);

        // Cleanup
        unlink(FIFO_NAME);
    }

    return 0;
}