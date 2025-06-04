#include <stdio.h> /* printf, stderr, fprintf */
#include <sys/types.h> /* pid_t */
#include <unistd.h> /* _exit, fork */
#include <stdlib.h> /* exit */
#include <errno.h> /* errno */

int main(void)
{
    //before calling the fork method
    printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());

    pid_t pid;
    /* Output from both the child and the parent process
    * will be written to the standard output 
    * as they both run at the same time.
    */

    pid = fork();
    if (pid == -1)
    {
        /* Error:
        * When fork() returns -1, an error happened
        * (for example, number of processes reached the limit).
        */
        fprintf(stderr, "can't fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }

    else if (pid == 0)
    {
        /* Child process:
        * When fork() returns 0, we are in
        * the child process.
        */

        printf("Child PID: %d, Parent PID: %d\n", getpid(), getppid());
        _exit(0); /* Note that we do not use exit() */
    }

    else
    {
        /* When fork() returns a positive number, we are in the parent process
        * (the fork return value is the PID of the newly created child process)
        */
        printf("Parent PID: %d\n", getpid());
        exit(0);
    }

    return 0;
}