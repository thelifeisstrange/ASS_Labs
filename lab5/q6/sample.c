#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child %d exiting with code 42\n", getpid());
        exit(42);
    }

    int status;
    wait(&status);
    printf("Parent: child %d finished\n", pid);
    if (WIFEXITED(status)) {
        printf("Exit return code (leftmost byte of status) = %d\n",
               WEXITSTATUS(status));
        printf("Raw status = %d, status >> 8 = %d\n", status, status >> 8);
    }
    return 0;
}
