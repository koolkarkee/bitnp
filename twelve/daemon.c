#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    pid_t pid, sid;

    // Capture the original parent process ID
    printf("Original Parent PID: %d\n", getpid());

    // First fork
    pid = fork();

    if (pid < 0) {
        perror("First fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("First child PID: %d (parent exiting)\n", pid);
        exit(EXIT_SUCCESS);
    }

    // Become session leader
    sid = setsid();
    if (sid < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // Second fork
    pid = fork();

    if (pid < 0) {
        perror("Second fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("Second child PID: %d (intermediate parent exiting)\n", pid);
        exit(EXIT_SUCCESS);
    }

    // This is now the grandchild daemon process
    printf("Daemon PID: %d (running detached)\n", getpid());

    // Daemon setup
    chdir("/");
    umask(0);

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirect them to /dev/null
    open("/dev/null", O_RDONLY);  // stdin
    open("/dev/null", O_WRONLY);  // stdout
    open("/dev/null", O_RDWR);    // stderr

    // Open log file to continue printing
    FILE *log = fopen("/tmp/mydaemon.log", "a+");
    if (!log) exit(EXIT_FAILURE);

    while (1) {
        fprintf(log, "Daemon PID %d running... 🌀\n", getpid());
        fflush(log);
        sleep(30);
    }

    fclose(log);
    exit(EXIT_SUCCESS);
}

//run the following command to view the daemon
//ps -eo pid,tty,cmd | awk '$2 == "?" {print $1, $3}'