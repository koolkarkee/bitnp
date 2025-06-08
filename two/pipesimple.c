#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <errno.h> 
#include <sys/wait.h>
#include<string.h>

//Wrapper for pipe
int Pipe(int fd[]){
    if(pipe(fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }
}

int main(){
    int pid, fd[2];
    char *buff = (char *)malloc(100); //allocating proper memory    
     
    Pipe(fd); //create a pipe
    pid = fork();

    if(pid == -1){
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){ //child process 
        printf("\nSending message from child - Child PID: %d, Parent PID: %d\n", getpid(), getppid());

        char message[] = "hello from child";
        write(fd[1], message, strlen(message));

        close(fd[0]); // Close read end in child
        close(fd[1]); // Close write end after use
        _exit(0);
    }
    else if(pid > 0){ //parent process 
        int status;
        waitpid(pid, &status, 0); 

        read(fd[0], buff, 100);
        buff[100] = '\0';  //Null-terminate to prevent garbage output
        printf("\nParent Received : %s\n", buff);
    
        close(fd[0]); // Close read end in parent
        close(fd[1]); // Close write end in parent
    }
    else
    {
        printf("Parent PID: %d\n", getpid());
        exit(0);
    }
    return 0;
}