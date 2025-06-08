#include <stdio.h> /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* _exit, fork */
#include <stdlib.h> /* exit */
#include <errno.h> /* errno */
#include <sys/wait.h>

//Wrapper function
int Execl(const char *__path, const char *_arg, ...){
    //execl call
    int result = execl(__path, _arg, (char *)0);
    if(result == -1)
    {
        perror("execl failed - ");
    }
}

int main(void)
{
    //before calling the fork method
    printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());

    pid_t pid;
    
    pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
         
        //Execl call within the child process
        Execl("/home/koolkarkee/bitnp/two/mybashscript.sh", "mybashscript.sh", (char *)0);
        _exit(0); /* Note that we do not use exit() */
    }
    else if(pid > 0) //parent process
    {
        int status;
        waitpid(pid, &status, 0);  // Wait for child process to finish
        
        printf("Child process finished, returning to parent execution.\n");
    }
    else
    {
        printf("Parent PID: %d\n", getpid());
        exit(0);
    }
    return 0;
}